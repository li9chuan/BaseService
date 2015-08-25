#include "session_mgr.h"
#include "frontend_service.h"
#include "client_mgr.h"
#include "msg_timeout_event.h"

#include <nel/misc/time_nl.h>
#include <nel/misc/mem_stream.h>
#include <nel/misc/bit_set.h>

#include <game_share/tools.h>
#include <server_share/server_def.h>

// for htonl
#ifdef NL_OS_WINDOWS
#	include <winsock2.h>
#elif defined NL_OS_UNIX
#	include <arpa/inet.h>
#endif

namespace FES
{
using namespace DEF;
using namespace NLMISC;
using namespace NLNET;

void CSessionMgr::init( )
{
    _curTicks = 0;
    _preTicks = 0;
    _preConnectTimeOutTicks = 0;

    _AcceptConnectMax            = Config.getVar ("AcceptConnectMax").asInt();
    _ConnectTimeOut              = Config.getVar ("LoginMsgTimeOut").asInt();
    _ConnectUpdateInterval       = Config.getVar ("LoginMsgUpdateInterval").asInt();
    _SessionTimeOut              = Config.getVar ("SessionTimeOut").asInt();
    _SessionUpdateInterval       = Config.getVar ("SessionUpdateInterval").asInt();
    _MsgBufferTimeOut            = Config.getVar ("MsgBufferTimeOut").asInt();
    _MsgBufferMaxNum             = Config.getVar ("MsgBufferMaxNum").asInt();

    _Random.srand( NLMISC::CTime::getSecondsSince1970() );
    _Sid = FrontEndService->getServiceId().get();

    ReSendMsgIdx.resize(MAX_RECV_ACK_IDX+1);
    
    nlassert( FrontEndService->getServiceId().get()<=0x7f );
}

bool CSessionMgr::addConnectTimeOut( NLNET::TSockId sock )
{
    if ( _connectMap.size() < _AcceptConnectMax )
    {
        _connectMap[sock] = _curTicks;
        return true;
    }
    FrontEndService->DisconnectClient( sock );
    return false;
}

bool CSessionMgr::removeConnectTimeOut( NLNET::TSockId sock )
{
    bool res = false;
    TConnectMap::iterator iter = _connectMap.find(sock);
    if ( iter != _connectMap.end() )
    {
        res = true;
        _connectMap.erase(iter);
    }
    return res;
}

CSessionData* CSessionMgr::findSession( DEF::RPC_SESSION session )
{
    CSessionData* pSessionData = NULL;
    TSessionMap::iterator iter = _sessionMap.find( session );

    if(iter != _sessionMap.end())
    {
        pSessionData = &iter->second;
    }
    return pSessionData;
}

bool CSessionMgr::addSession( DEF::RPC_SESSION session, DEF::UID uid, NLNET::TSockId sock_id )
{
    bool res = false;
    if ( _sessionMap.size() < _AcceptConnectMax )
    {
        CSessionData session_data;
        session_data.sock_id  = sock_id;
        session_data.ticks    = LocalTime.GetCurrTime();
        session_data.uid      = uid;

        _sessionMap[session] = session_data;
    //	sock_id->setAppId( session );
        res = true;
    }
    return res;
}

void CSessionMgr::update( NLMISC::TTicks ticks )
{
    _curTicks = ticks;

    if( ticks - _preTicks > _SessionUpdateInterval )
    {
        for( TSessionMap::iterator iter=_sessionMap.begin(); iter!=_sessionMap.end(); )
        {
            if( ticks - (*iter).second.ticks > _SessionTimeOut )
            {
                if ( iter->second.sock_id!=NLNET::InvalidSockId )
                {
                    FrontEndService->DisconnectClient( iter->second.sock_id );
                }
                _sessionMap.erase( iter++ );
            }
            else
            {
                ++iter;
            }
        }

        _preTicks = ticks;
    }

    if( ticks - _preConnectTimeOutTicks > _ConnectUpdateInterval )
    {
        for( TConnectMap::iterator iter=_connectMap.begin(); iter!=_connectMap.end(); )
        {
            if( ticks - (*iter).second > _ConnectTimeOut )
            {
                FrontEndService->DisconnectClient( iter->first );
                _connectMap.erase( iter++ );
            }
            else
            {
                ++iter;
            }
        }

        _preConnectTimeOutTicks = ticks;
    }
}

bool CSessionMgr::send( DEF::RPC_SESSION session, CMessage& msgout, bool disconnect/*=true*/ )
{
    CSessionData* pSessionData = findSession( session );
    if(pSessionData==NULL)  {  return false;  }

    //nldebug("loglog name:%s   session:%d  size:%d  tick:%"NL_I64"u", msgout.getName().c_str(), (uint16)session , msgout.length() , (uint64)CTime::getLocalTime());

    CClient* pClient = ClientMgr.findClient(pSessionData->uid);

    if ( IsTCPSession(session) )
    {
        if ( pClient!=NULL )
        {
            pClient->sendMsgBufferToSock(pSessionData->sock_id);
        }

        FrontEndService->SendToClient( msgout, pSessionData->sock_id );

        if (disconnect)
        {
            FrontEndService->DisconnectClient(pSessionData->sock_id);
        }

        //nldebug( " CSessionMgr::send IsTCPSession " );
    }
    else
    {
        //nldebug( " CSessionMgr::send IsTCPSession [ %d ] " , ( int )disconnect );

        if ( disconnect )
        {
            removeSession(session);
            //return sendudp( pClient, msgout, true );
        }

        return sendudp( pClient, msgout, true );
        //else
        //{
        //    if ( pClient!=NULL )
        //    {
        //        pClient->addMsgBuffer( new(std::nothrow) CMessage(msgout) );
        //    }
        //}
    }

    return false;
}


UDPToClientMsg udp_stream;

bool CSessionMgr::sendudp( CClient* pClient, NLNET::CMessage& msgout, bool need_ack )
{
    if ( pClient!=NULL )
    {
        if ( pClient->state != CClient::ONLINE )
        {
            return false;
        }

        if ( pClient->addr_from.isValid() )
        {
            if ( need_ack )
            {
                CMsgTimeoutTimerEvent* pTimerEvent = new(std::nothrow) CMsgTimeoutTimerEvent();

                if ( pTimerEvent!=NULL )
                {
                    (_ACKIdx>=MAX_RECV_ACK_IDX)?_ACKIdx=0:++_ACKIdx;

                    UDPToClientMsg& mem_stream = pTimerEvent->mem_stream;

                    mem_stream.SetControlFlag(PROTO_NEED_ACK);
                    mem_stream.SetAckIdx(_ACKIdx);
                    pClient->FillUDPStream(mem_stream);
                    mem_stream.AddBuffer( (uint8*)msgout.buffer(), msgout.length() );

                    FrontEndService->SendUDP(mem_stream.buffer(), mem_stream.length(), pClient->addr_from);

                    pTimerEvent->address = pClient->addr_from;
                    pTimerEvent->resend_idx = _ACKIdx;
                    ReSendMsgIdx.clear(_ACKIdx);

                    CTimer*  pTimer = new(std::nothrow) CTimer();
                    if ( pTimer==NULL )
                    {
                        delete pTimerEvent;
                        return false;
                    }
                    pTimer->setRemaining( 300, pTimerEvent );
                    return true;
                }
            }
            else
            {
                udp_stream.reset();
                //udp_stream.ClearControlFlag(PROTO_NEED_ACK);
                udp_stream.AddBuffer( (uint8*)msgout.buffer(), msgout.length() );

                FrontEndService->SendUDP(udp_stream.buffer(), udp_stream.length(), pClient->addr_from);
                return true;
            }
        }
    }
    return false;
}

void CSessionMgr::RecvACK( DEF::ACK_IDX ack )
{
    ReSendMsgIdx.set(ack&MAX_RECV_ACK_IDX);
}

void CSessionMgr::RecvHeartBeat( MsgSession& proto_session, NLNET::CInetAddress& AddrFrom )
{
    CClient* pClient = ClientMgr.findClientBySession( proto_session.session() );

    if ( pClient!=NULL )
    {
        if ( pClient->CheckMsgValid( proto_session.index(), proto_session.check() ) )
        {
            pClient->addr_from = AddrFrom;
            pClient->last_recv_msg = LocalTime.GetCurrTime();
        }
    }
}














}

