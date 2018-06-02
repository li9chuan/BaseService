// NeL - MMORPG Framework <http://dev.ryzom.com/projects/nel/>
// Copyright (C) 2010  Winch Gate Property Limited
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "stdnet.h"

#include "nel/misc/hierarchical_timer.h"

#include "nel/net/buf_server_tcp.h"
#include "nel/net/net_log.h"
#include "nel/net/message.h"

#include "event2/event.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/thread.h"

#ifdef NL_OS_WINDOWS
#	ifndef NL_COMP_MINGW
#		define NOMINMAX
#	endif
#	include <windows.h>
#elif defined NL_OS_UNIX
#	include <unistd.h>
#	include <cerrno>
#	include <sys/types.h>
#	include <sys/time.h>
#endif

#include "nel/net/buf_server_tcp_func.h"


/*
 * On Linux, the default limit of descriptors is usually 1024, you can increase it with ulimit
 */

using namespace NLMISC;
using namespace std;

namespace NLNET {

/***************************************************************************************************
 * User main thread (initialization)
 **************************************************************************************************/

/*
 * Constructor
 */
CBufServerTcp::CBufServerTcp() : _ConnectionCallback(NULL)
{
	nlnettrace( "CBufServerTcp::CBufServerTcp" );

    _WebSocketReceiveTask = new CTcpReceiveTask();
}

/*
 * Listens on the specified port
 */
void CBufServerTcp::init( uint16 port )
{
	nlnettrace( "CBufServerTcp::init" );

    _MaxExpectedBlockSize = maxExpectedBlockSize();

    _WebSocketReceiveTask->init( this, port );
    _WebSocketReceiveTask->start();
}

/*
 * Destructor
 */
CBufServerTcp::~CBufServerTcp()
{
	nlnettrace( "CBufServerTcp::~CBufServerTcp" );

	// Clean listen thread exit


//		((CListenTask*)(_ListenThread->getRunnable()))->requireExit();
//		((CListenTask*)(_ListenThread->getRunnable()))->close();
//#ifdef NL_OS_UNIX
//		_ListenTask->wakeUp();
//#endif
//		_ListenThread->wait();
//		delete _ListenThread;
//		delete _ListenTask;

    _WebSocketReceiveTask->close();

	nlnettrace( "Exiting CBufServerTcp::~CBufServer" );
}


/*
 * Disconnect the specified host
 * Set hostid to NULL to disconnect all connections.
 * If hostid is not null and the socket is not connected, the method does nothing.
 * If quick is true, any pending data will not be sent before disconnecting.
 */
void CBufServerTcp::disconnect( TSockId hostid, bool quick )
{
	nlnettrace( "CBufServerTcp::disconnect" );
    if (hostid != InvalidSockId)
    {
        if (_ConnectedClients.find(hostid) == _ConnectedClients.end())
        {
            // this host is not connected
            return;
        }

        if (hostid->connectedState())
        {
            hostid->advertiseDisconnection(this, hostid);
        }

        //// Disconnect only if physically connected
        //if ( hostid->Sock->connected() )
        //{
        //	if ( ! quick )
        //	{
        //		hostid->flush();
        //	}
        //	hostid->Sock->disconnect(); // the connection will be removed by the next call of update()
        //}
    }
    else
    {
        // Disconnect all

    }
}


/*
 * Send a message to the specified host
 */
void CBufServerTcp::send( const CMemStream& buffer, TSockId hostid )
{
	nlnettrace( "CBufServerTcp::send" );
	nlassert( buffer.length() > 0 );
	nlassertex( buffer.length() <= maxSentBlockSize(), ("length=%u max=%u", buffer.length(), maxSentBlockSize()) );

	// slow down the layer H_AUTO (CBufServer_send);

	if ( hostid != InvalidSockId )
	{
		if (_ConnectedClients.find(hostid) != _ConnectedClients.end())
		{
			hostid->SendToLibEvent(buffer);
		}
	}
	else
	{
		// Push into all send queues
        // 全服广播在逻辑层用定时器实现
	}
}

/*
 * Checks if there are some data to receive
 */
bool CBufServerTcp::dataAvailable()
{
	// slow down the layer H_AUTO (CBufServer_dataAvailable);
	{
		/* If no data available, enter the 'while' loop and return false (1 volatile test)
		 * If there are user data available, enter the 'while' and return true immediately (1 volatile test + 1 short locking)
		 * If there is a connection/disconnection event (rare), call the callback and loop
		 */
		while ( dataAvailableFlag() )
		{
			// Because _DataAvailable is true, the receive queue is not empty at this point
			vector<uint8> buffer;
			uint8 val;
			{
				CFifoAccessor recvfifo( &receiveQueue() );
				val = recvfifo.value().frontLast();
				if ( val != CBufNetBase::User )
				{
					recvfifo.value().front( buffer );
				}
			}

			/*sint32 mbsize = recvfifo.value().size() / 1048576;
			if ( mbsize > 0 )
			{
			  nlwarning( "The receive queue size exceeds %d MB", mbsize );
			}*/

			/*vector<uint8> buffer;
			recvfifo.value().front( buffer );*/

			// Test if it the next block is a system event
			//switch ( buffer[buffer.size()-1] )
			switch ( val )
			{

			// Normal message available
			case CBufNetBase::User:
				{
					return true; // return immediately, do not extract the message
				}

			// Process disconnection event
			case CBufNetBase::Disconnection:
				{
                    TSockId sockid = *((TSockId*)(&*buffer.begin()));

                    if (_ConnectedClients.count(sockid) > 0)
                    {
                        LNETL1_DEBUG("LNETL1: Disconnection event for %p %s", sockid, sockid->asString().c_str());
                        sockid->setConnectedState(false);

                        // Call callback if needed
                        if (disconnectionCallback() != NULL)
                        {
                            disconnectionCallback()(sockid, argOfDisconnectionCallback());
                        }

                        // Add socket object into the synchronized remove list
                        //LNETL1_DEBUG( "LNETL1: Adding the connection to the remove list" );
                        //nlassert( ((CServerBufSock*)sockid)->ownerTask() != NULL );
                        //((CServerBufSock*)sockid)->ownerTask()->addToRemoveSet( sockid );


                        //  自动close套接字和free读写缓冲区 
                        bufferevent_free(sockid->m_BEVHandle);

                        if (sockid->m_Ssl != NULL)
                        {
                            nlwarning("tcp not setup ssl");
                            //SSL_free(sockid->m_Ssl);
                            //sockid->m_Ssl = NULL;
                        }

                        LNETL1_DEBUG("LNETL1: Remove the connection");
                        delete sockid;


                        // remove from the list of valid client
                        nlverify(_ConnectedClients.erase(sockid) == 1);
                    }

					break;
				}
			// Process connection event
			case CBufNetBase::Connection:
				{
					TSockId sockid = *((TSockId*)(&*buffer.begin()));
					LNETL1_DEBUG( "LNETL1: Connection event for %p %s", sockid, sockid->asString().c_str());

					// add this socket in the list of client
					nlverify(_ConnectedClients.insert(sockid).second);

					sockid->setConnectedState( true );

					// Call callback if needed
					if ( connectionCallback() != NULL )
					{
						connectionCallback()( sockid, argOfConnectionCallback() );
					}
					break;
				}
			default: // should not occur
				LNETL1_INFO( "LNETL1: Invalid block type: %hu (should be = to %hu", (uint16)(buffer[buffer.size()-1]), (uint16)(val) );
				LNETL1_INFO( "LNETL1: Buffer (%d B): [%s]", buffer.size(), stringFromVector(buffer).c_str() );
				LNETL1_INFO( "LNETL1: Receive queue:" );
				{
					CFifoAccessor recvfifo( &receiveQueue() );
					recvfifo.value().display();
				}
				nlerror( "LNETL1: Invalid system event type in server receive queue" );

			}

			// Extract system event
			{
				CFifoAccessor recvfifo( &receiveQueue() );
				recvfifo.value().pop();
				setDataAvailableFlag( ! recvfifo.value().empty() );
			}
		}
		// _DataAvailable is false here
		return false;
	}
}


#ifdef NL_OS_UNIX
/* Wait until the receive queue contains something to read (implemented with a select()).
 * This is where the connection/disconnection callbacks can be called.
 * \param usecMax Max time to wait in microsecond (up to 1 sec)
 */
void	CBufServerTcp::sleepUntilDataAvailable( uint usecMax )
{
	// Prevent looping infinitely if the system time was changed
	if ( usecMax > 999999 ) // limit not told in Linux man but here: http://docs.hp.com/en/B9106-90009/select.2.html
		usecMax = 999999;

	fd_set readers;
	timeval tv;
	do
	{
		FD_ZERO( &readers );
		FD_SET( _DataAvailablePipeHandle[PipeRead], &readers );
		tv.tv_sec = 0;
		tv.tv_usec = usecMax;
		int res = ::select( _DataAvailablePipeHandle[PipeRead]+1, &readers, NULL, NULL, &tv );
		if ( res == -1 )
			nlerror( "LNETL1: Select failed in sleepUntilDataAvailable (code %u)", CSock::getLastError() );
	}
	while ( ! dataAvailable() ); // will loop if only a connection/disconnection event was read
}
#endif


/*
 * Receives next block of data in the specified. The length and hostid are output arguments.
 * Precond: dataAvailable() has returned true, phostid not null
 */
void CBufServerTcp::receive( CMemStream& buffer, TSockId* phostid )
{
	nlnettrace( "CBufServerTcp::receive" );
	//nlassert( dataAvailable() );
	nlassert( phostid != NULL );

	{
		CFifoAccessor recvfifo( &receiveQueue() );
		nlassert( ! recvfifo.value().empty() );
		recvfifo.value().front( buffer );
		recvfifo.value().pop();
		setDataAvailableFlag( ! recvfifo.value().empty() );
	}

	// Extract hostid (and event type)
	*phostid = *((TSockId*)&(buffer.buffer()[buffer.size()-sizeof(uint64)-1]));
	nlassert( buffer.buffer()[buffer.size()-1] == CBufNetBase::User );

	buffer.resize( buffer.size()-sizeof(uint64)-1 );

	// TODO OPTIM remove the nldebug for speed
	//commented for optimisation LNETL1_DEBUG( "LNETL1: Read buffer (%d+%d B) from %s", buffer.size(), sizeof(TSockId)+1, /*stringFromVector(buffer).c_str(), */(*phostid)->asString().c_str() );

	// Statistics
	_BytesPoppedIn += buffer.size() + sizeof(TBlockSize);
}


/*
 * Update the network (call this method evenly)
 */
void CBufServerTcp::update()
{
	//nlnettrace( "CBufServer::update-BEGIN" );

	_NbConnections = 0;

	// For each thread


	//nlnettrace( "CBufServer::update-END" );
}

uint32 CBufServerTcp::getSendQueueSize( TSockId destid )
{
	if ( destid != InvalidSockId )
	{
		if (_ConnectedClients.find(destid) == _ConnectedClients.end())
		{
			// this host is not connected
			return 0;
		}

		return destid->SendFifo.size();
	}
	else
	{
		// add all client buffers

		uint32 total = 0;

		// For each thread

		return total;
	}
}

void CBufServerTcp::displayThreadStat (NLMISC::CLog *log)
{
	//// For each thread
	//CThreadPool::iterator ipt;
	//{
	//	CSynchronized<CThreadPool>::CAccessor poolsync( &_ThreadPool );
	//	for ( ipt=poolsync.value().begin(); ipt!=poolsync.value().end(); ++ipt )
	//	{
	//		// For each thread of the pool
	//		CServerReceiveTask *task = receiveTask(ipt);
	//		// For each socket of the thread, update sending
	//		log->displayNL ("server receive thread %p nbloop %d", task, task->NbLoop);
	//	}
	//}

	//log->displayNL ("server listen thread %p nbloop %d", _ListenTask, _ListenTask->NbLoop);
}

void CBufServerTcp::setTimeFlushTrigger( TSockId destid, sint32 ms )
{
	nlassert( destid != InvalidSockId );
	if (_ConnectedClients.find(destid) != _ConnectedClients.end())
		destid->setTimeFlushTrigger( ms );
}

void CBufServerTcp::setSizeFlushTrigger( TSockId destid, sint32 size )
{
	nlassert( destid != InvalidSockId );
	if (_ConnectedClients.find(destid) != _ConnectedClients.end())
		destid->setSizeFlushTrigger( size );
}

bool CBufServerTcp::flush( TSockId destid, uint *nbBytesRemaining)
{
	nlassert( destid != InvalidSockId );
	if (_ConnectedClients.find(destid) != _ConnectedClients.end())
		return destid->flush( nbBytesRemaining );
	else
		return true;
}
const CInetAddress& CBufServerTcp::hostAddress( TSockId hostid )
{
	nlassert( hostid != InvalidSockId );
	if (_ConnectedClients.find(hostid) != _ConnectedClients.end())
		return hostid->Sock->remoteAddr();

	static CInetAddress nullAddr;
	return nullAddr;
}

void CBufServerTcp::displaySendQueueStat (NLMISC::CLog *log, TSockId destid)
{

}


/*
 * Returns the number of bytes received since the previous call to this method
 */
uint64 CBufServerTcp::newBytesReceived()
{
	uint64 b = bytesReceived();
	uint64 nbrecvd = b - _PrevBytesPoppedIn;
	//nlinfo( "b: %" NL_I64 "u   new: %" NL_I64 "u", b, nbrecvd );
	_PrevBytesPoppedIn = b;
	return nbrecvd;
}

/*
 * Returns the number of bytes sent since the previous call to this method
 */
uint64 CBufServerTcp::newBytesSent()
{
	uint64 b = bytesSent();
	uint64 nbsent = b - _PrevBytesPushedOut;
	//nlinfo( "b: %" NL_I64 "u   new: %" NL_I64 "u", b, nbsent );
	_PrevBytesPushedOut = b;
	return nbsent;
}



void CTcpReceiveTask::init( CBufServerTcp *server, uint16 port )
{
	_Server = server;

    struct sockaddr_in sin;  
    memset(&sin, 0, sizeof(struct sockaddr_in));  
    sin.sin_family = AF_INET;  
    sin.sin_port = htons(port); 

    evthread_use_windows_threads();

    pEventBase = event_base_new();
    TcpListenArgs* pListenArgs = new struct TcpListenArgs(pEventBase,_Server);
    

    pEvListener = evconnlistener_new_bind(  pEventBase, tcp_listener_cb, (void*)pListenArgs, 
                                            LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, 10, 
                                            (struct sockaddr*)&sin, sizeof(struct sockaddr_in));  

}

void CTcpReceiveTask::run()
{
	nlnettrace( "CTcpReceiveTask::run" );
    event_base_dispatch(pEventBase);  
}

bool CTcpReceiveTask::start()
{
    _ProcTableThread = NLMISC::IThread::create( this );

    if ( _ProcTableThread != NULL )
    {
        _ProcTableThread->start();
        _ExitRequired = false;
    }

    return ( NULL != _ProcTableThread );
}

void CTcpReceiveTask::close()
{
    if ( NULL != _ProcTableThread )
    {
        _ExitRequired = true;
        _ProcTableThread->wait();
        evconnlistener_free(pEvListener);  
        event_base_free(pEventBase); 
    }
}




} // NLNET
