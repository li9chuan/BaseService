#include "stdpch.h"

#include <nel/misc/stop_watch.h>
#include <nel/misc/bit_set.h>

#include "fe_types.h"
#include "fe_receive_sub.h"
#include "../frontend_service.h"
#include "../msg_client.h"
#include "../session_mgr.h"
#include "../udp_sender.h"

using namespace std;
using namespace NLNET;
using namespace NLMISC;
using namespace DEF;

bool verbosePacketLost = false;

//extern CVariable<bool>		VerboseFEStatsTime;


string getBadMessageString( uint32 reasons )
{
	string s;
	if ( reasons & InsufficientSize )
		s += " Insufficient size;";
	if ( reasons & NotSystemLoginCode )
		s += " Not system login code;";
	if ( reasons & BadCookie )
		s += " Bad cookie;";
	if ( reasons & BadSystemCode )
		s += " Bad system code;";
	if ( reasons & HackedSizeInBuffer )
		s += " Hacked size in buffer;";
	if ( reasons & AccessClosed )
		s += " Access closed;";
	if ( reasons & IrrelevantSystemMessage )
		s += " Irrelevant System Message;";
	if ( reasons & MalformedAction )
		s += " Malformed action;";
	if ( reasons & UnknownExceptionType )
		s += " Unknown Exception;";
	if ( reasons & UnknownFormatType )
		s += " Unknown format type;";
	if ( reasons & UnauthorizedCharacterSlot )
		s += " Unauthorized character slot;";
	return s;
}

void CFeReceiveSub::init( uint16 firstAcceptableFrontendPort, uint16 lastAcceptableFrontendPort, uint32 dgrammaxlength )
{
	// Preconditions
	nlassert( firstAcceptableFrontendPort != 0 );
	nlassert( lastAcceptableFrontendPort >= firstAcceptableFrontendPort );
	nlassert( dgrammaxlength != 0 );
	nlassert( (_ReceiveTask==NULL) && (_ReceiveThread==NULL) );

	// Start external datagram socket
	nlinfo( "FERECV: Starting external datagram socket" );
	_ReceiveTask = new CFEReceiveTask( firstAcceptableFrontendPort, lastAcceptableFrontendPort, dgrammaxlength );
	_CurrentReadQueue = &_Queue2;
	_ReceiveTask->setWriteQueue( &_Queue1 );
	nlassert( _ReceiveTask != NULL );
	_ReceiveThread = IThread::create( _ReceiveTask );
	nlassert( _ReceiveThread != NULL );
	_ReceiveThread->start();

	// Setup current message placeholder
	_CurrentInMsg = new TReceivedMessage();

	//// Setup connection stat log
	//_ConnectionStatDisp = new CFileDisplayer( string("data_shard_local/connections.stat"), false, "ConnectionStats" );
	//ConnectionStatLog = new CLog( CLog::LOG_NO );
	//ConnectionStatLog->addDisplayer( _ConnectionStatDisp );
}

void CFeReceiveSub::release()
{
	if ( ConnectionStatLog )
		delete ConnectionStatLog;
	if ( _ConnectionStatDisp )
		delete _ConnectionStatDisp;

	nlassert( _ReceiveTask != NULL );
	nlassert( _ReceiveThread != NULL );

	_ReceiveTask->requireExit();
#ifdef NL_OS_UNIX
	// Send dummy data to a bound udp socket, to wake-up the receive thread
	vector<CInetAddress> addrlist = CInetAddress::localAddresses();
	if ( ! addrlist.empty() )
	{
		CUdpSock tempSock;
		uint8 dummyByte = 0;
		addrlist[0].setPort( _ReceiveTask->DataSock->localAddr().port() );
		tempSock.sendTo( &dummyByte, sizeof(dummyByte), addrlist[0] );
	}
#else
	// Under Windows, closing the socket wakes-up recvfrom().
	_ReceiveTask->DataSock->close();
#endif
	_ReceiveThread->wait();

	delete _ReceiveThread;
	delete _ReceiveTask;
	_ReceiveTask = NULL;
	_ReceiveThread = NULL;

	nlassert( _CurrentInMsg != NULL );
	delete _CurrentInMsg;
	_CurrentInMsg = NULL;
}

//CStopWatch	RecvWatch;
NLMISC::CVariable<uint32>	VAR_FPS_PROCESS_MSG("fes", "FPSProcessMsg", "", 0);

void CFeReceiveSub::readIncomingData()
{
	//CFrontEndService::instance()->ReceiveWatch.start();
	//RecvWatch.start();

    H_AUTO(UDPReadIncomingData);

    VAR_FPS_PROCESS_MSG.set( _CurrentReadQueue->size(), false );

	// Read queue of messages received from clients
	while ( ! _CurrentReadQueue->empty() )
	{
		//nlinfo( "Read queue size = %u", _CurrentReadQueue->size() );
		_CurrentReadQueue->front( _CurrentInMsg->data() );
		_CurrentReadQueue->pop();
		nlassert( ! _CurrentReadQueue->empty() );
		_CurrentReadQueue->front( _CurrentInMsg->VAddrFrom );
		_CurrentReadQueue->pop();
		_CurrentInMsg->vectorToAddress();

        handleIncomingMsg();
	}

	//// Measure and display rate evenly (at a low frequency)
	//static TTime lastdisplay = CTime::getLocalTime();
	//if ( CTime::getLocalTime() - lastdisplay > 5000 )
	//{
	//	// Message stats
	//	if ( VerboseFEStatsTime.get() )
	//		displayDatagramStats( lastdisplay );

	//	lastdisplay = CTime::getLocalTime();
	//	_RcvCounter = 0;
	//	CFrontEndService::instance()->sendSub()->sendCounter() = 0;

	//	// Hacking detection: message size
	//	uint nbrej = _ReceiveTask->nbNewRejectedDatagrams();
	//	if ( nbrej != 0 )
	//	{
	//		nlinfo( "FEHACK: Rejected big messages: %u", nbrej );
	//	}

	//	// Hacking detection: bad identification
	//	if ( ! _UnidentifiedFlyingClients.empty() )
	//	{
	//		nldebug( "FEHACK: Received messages from %u unidentified clients:", _UnidentifiedFlyingClients.size() );
	//		THackingAddrSet::iterator ias;
	//		for ( ias=_UnidentifiedFlyingClients.begin(); ias!=_UnidentifiedFlyingClients.end(); ++ias )
	//		{
	//			nlinfo( "FEHACK: * User %u %s --> %u msg, reasons:%s", (*ias).second.UserId, (*ias).first.asString().c_str(), (*ias).second.InvalidMsgCounter, getBadMessageString( (*ias).second.Reasons ).c_str() );
	//		}
	//		_UnidentifiedFlyingClients.clear();
	//	}
	//}

	//RecvWatch.stop();

    //if ( VerboseFEStatsTime.get() && (RecvWatch.getDuration() > 20) ) // does not include the vision updating
	//{
	//	nlinfo( "FETIME: Time of receiving: AllButVision peak=%u", RecvWatch.getDuration() );
	//}
	//CFrontEndService::instance()->ReceiveWatch.stop();
}

void CFeReceiveSub::swapReadQueues()
{
	if ( _CurrentReadQueue == &_Queue1 )
	{
		_CurrentReadQueue = &_Queue2;
		_ReceiveTask->setWriteQueue( &_Queue1 );
	}
	else
	{
		_CurrentReadQueue = &_Queue1;
		_ReceiveTask->setWriteQueue( &_Queue2 );
	}
}

void CFeReceiveSub::handleIncomingMsg()
{
    if ( ! _AccessOpen || ! FrontEndService->AcceptClients )
    {
        rejectReceivedMessage( AccessClosed );
        return;
    }

    if ( _CurrentInMsg->eventType()==TReceivedMessage::User )
    {
        if ( _CurrentInMsg->isProtoCMD() )
        {
            handleReceivedProtoCommand();
        }
        else if ( _CurrentInMsg->userSize()>5 )
        {
            handleReceivedMsg();
        }
        else
        {
            nldebug("error %s",_CurrentInMsg->AddrFrom.asString().c_str());
        }
    }
}

//NLMISC::CBitMemStream Msgin( true ); // global to avoid useless realloc

void CFeReceiveSub::handleReceivedMsg()
{
	try
	{
        CMessage msgin;
        msgin.fill( _CurrentInMsg->userDataR(), _CurrentInMsg->userSize() );
        msgin.invert();
        UDPSender.Forwarding( msgin, _CurrentInMsg->AddrFrom, _CurrentInMsg->isProtoNeedACK() );
	}
	catch(const EStreamOverflow& )
	{
		rejectReceivedMessage( InsufficientSize );
	}
	catch(const EInvalidDataStream& )
	{
		rejectReceivedMessage( HackedSizeInBuffer );
	}
	catch (const Exception& )
	{
		rejectReceivedMessage( UnknownExceptionType );
	}
	catch ( ... )
	{
		rejectReceivedMessage( UnknownFormatType );
	}
}

void CFeReceiveSub::handleReceivedProtoCommand( )
{
    uint32 currentsize = _CurrentInMsg->userSize();
    uint8  cmd = 0;

    if ( currentsize>1 )
    {
        CMemStream  buff_in(true);
        buff_in.fill( _CurrentInMsg->userDataR(), _CurrentInMsg->userSize() );
        buff_in.serial(cmd);

        switch (cmd)
        {
        case PROTO_CMD_ACK:
            if ( currentsize==1+4 )
            {
                ACK_IDX ack;
                buff_in.serial(ack);
                SessionMgr.RecvACK(ack);

                //nldebug("PROTO_CMD_ACK   %d   %s", ack, _CurrentInMsg->AddrFrom.asString().c_str());
            }
            break;
        case PROTO_CMD_HEARTBEAT:
            {
                ///  ConsoleFlag(8bit)  ACK_CMD(8bit)  Session(64bit)
                if ( currentsize<30 )
                {
                    MsgSession  _ProtoSession;

                    try
                    {
                        buff_in.serial(&_ProtoSession);
                        SessionMgr.RecvHeartBeat(_ProtoSession, _CurrentInMsg->AddrFrom);
                    }
                    catch (Exception &) {
                        return;
                    }


                    //DEF::CLIENT_SESSION  client_session=0;
                    //buff_in.serial(client_session);
                    //SessionMgr.RecvHeartBeat(client_session,_CurrentInMsg->AddrFrom);
                }
                break;
            }
        default:
            nldebug("control cmd error %s",_CurrentInMsg->AddrFrom.asString().c_str());
            break;
        }
    }
}

/*
 * Do not accept the current message (hacking detection)
 */
void CFeReceiveSub::rejectReceivedMessage( TBadMessageFormatType bmft )
{
	THackingDesc& desc = _UnidentifiedFlyingClients[_CurrentInMsg->AddrFrom];
	
	++desc.InvalidMsgCounter;
	desc.Reasons |= bmft;
}

/*
 * Display datagram loss statistics
 */
void CFeReceiveSub::displayDatagramStats( TTime lastdisplay )
{
	uint32 pcsum=0, pcnb=0;

	_PrevRcvBytes = _RcvBytes;
}

//uint32	 CFeReceiveSub::getNbClient()
//{
//	return (uint32)_ClientMap.size();
//}

NLMISC_COMMAND( openAccess, "Open/close the access for new clients", "<1/0>" )
{
	bool b, get;
	if ( args.size() == 0 )
	{
		b = FrontEndService->ReceiveSub().accessOpen();
		get = true;
	}
	else
	{
		b = (args[0] == "1");
		FrontEndService->ReceiveSub().openAccess( b );
		get = false;
	}
	log.displayNL( "Access is %s %s", get?"currently":"now", b?"open":"closed" );
	return true;
}

NLMISC_COMMAND( verbosePacketLost, "Turn on/off or check the state of verbose logging of packet lost", "" )
{
	if ( args.size() == 1 )
	{
		if ( args[0] == string("on") || args[0] == string("1") )
			verbosePacketLost=true;
		else if ( args[0] == string("off") || args[0] == string("0") )
			verbosePacketLost=false;
	}

	log.displayNL( "verbosePacketLost is %s", verbosePacketLost ? "on" : "off" );
	return true;
}
