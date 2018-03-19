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

#include "nel/net/buf_server_websocket.h"
#include "nel/net/net_log.h"
#include "nel/net/message.h"

#include "libevent/event2/event.h"
#include "libevent/event2/listener.h"
#include "libevent/event2/bufferevent.h"
//#include "libevent/event2/thread.h"

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

#include "nel/misc/sha1.h"
#include "nel/misc/base64.h"


/*
 * On Linux, the default limit of descriptors is usually 1024, you can increase it with ulimit
 */

using namespace NLMISC;
using namespace std;

namespace NLNET {

struct SListenArgs
{
    event_base*             pEventBase;
    CBufServerWebsocket*    pServer;
    SListenArgs( event_base* eventbase, CBufServerWebsocket* bufsvr ) : pEventBase(eventbase), pServer(bufsvr) {}
};

struct SEventArgs
{
    CBufServerWebsocket*    pServer;
    CServerBufSock*         pBufSock;
    bool                    bHandshake;
    SEventArgs( CServerBufSock* bufsock, CBufServerWebsocket* bufsvr ) : pBufSock(bufsock), pServer(bufsvr), bHandshake(false) {}
};



CSString generate_key(const CSString &key) 
{
    CSString tmp = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    CHashKey hash_key = getSHA1( (const uint8 *)tmp.c_str(), tmp.size() );
    return base64_encode( hash_key.HashKeyString );
}

CSString generate_websocket_response( CSString& sec_websocket_key ) 
{
    CSString resp;

    resp += "HTTP/1.1 101 WebSocket Protocol HandShake\r\n";
    resp += "Connection: Upgrade\r\n";
    resp += "Upgrade: WebSocket\r\n";
    //resp += "Server: WebChat Demo Server\r\n";
    resp += "Sec-WebSocket-Accept: " + generate_key(sec_websocket_key) + "\r\n";
    resp += "\r\n";

    return resp;
}

struct WebSocketFrame
{
    uint8   fin;
    uint8   opcode;
    uint8   mask;
    uint64  payload_len;
    unsigned char masking_key[4];
};

sint32 parse_frame_header( const uint8 *buf, WebSocketFrame& frame )
{
    if (!buf)
    {
        return -1;
    }

    unsigned char c1 = *buf;
    unsigned char c2 = *(buf + 1);
    frame.fin = (c1 >> 7) & 0xff;
    frame.opcode = c1 & 0x0f;
    frame.mask = (c2 >> 7) & 0xff;
    frame.payload_len = c2 & 0x7f;

    return 0;
}

inline void unmask_payload_data( WebSocketFrame& frame, uint8* buff )
{
    for ( uint32 i = 0; i < frame.payload_len; ++i )
    {
        *(buff + i) = *(buff + i) ^ *(frame.masking_key + i % 4);
    }
}




void socket_read_cb(bufferevent *bev, void *args)  
{  
    char read_buffer[4096];  
    size_t len = bufferevent_read(bev, read_buffer, sizeof(read_buffer) );  /* -1 */ 

    //printf("server read the data %s\n", read_buffer);  

    //char reply[] = "I has read your data";  
    //bufferevent_write(bev, reply, strlen(reply) );  

    

    SEventArgs* pEventArgs = (SEventArgs*)args;

    if( !pEventArgs->bHandshake )
    {
        if( len < sizeof(read_buffer) )
        {
            read_buffer[len] = '\0';
            CSString  http_head(read_buffer);

            CVectorSString  split_req;
            http_head.splitLines( split_req );

            for ( uint i=0; i<split_req.size(); ++i )
            {
                CVectorSString  split_key;
                split_req[i].splitBySeparator(':', split_key);

                if ( split_key.size() == 2 && split_key[0] == "Sec-WebSocket-Key"  )
                {
                    CSString res_key = generate_websocket_response( split_key[1].strip() );
                    bufferevent_write(bev, res_key.c_str(), res_key.size() );
                    pEventArgs->bHandshake = true;

                    nlinfo( "WebSocket Handshake ." );
                }
            }
        }
    }
    else
    {
        //read_buffer[len] = '\0';
        //CSString  http_head(read_buffer);

        uint32 buff_len = pEventArgs->pBufSock->appendToBuffer( (const uint8*)read_buffer, len );


        while( buff_len >= 2 )
        {
            WebSocketFrame frame;

            uint8* buff = pEventArgs->pBufSock->getBuffer();
            parse_frame_header( buff, frame );


            uint32 offset = 2;
            if (frame.payload_len == 126)
            {
                frame.payload_len = ntohs(*(uint16*)(buff+offset));
                offset = 4;
            }
            else if (frame.payload_len == 127)
            {
                frame.payload_len = ntohs(*(uint64*)(buff+offset));
                offset = 10;
            }
            else if( frame.fin == 1 && frame.payload_len < 126 )
            {
                if( frame.opcode == 0x8 )
                {
                    if( frame.mask == 1 )  {  offset += 4;  }

                    //0x8 denotes a connection close
                    buff_len = pEventArgs->pBufSock->leftShiftBuffer( offset + frame.payload_len );

                    nlinfo( "WebSocket Connection Close ." );
                    continue;
                }
                else if( frame.opcode == 0x9 )
                {
                    //0x9 denotes a ping
                    //TODO
                    //make a pong
                    if( frame.mask == 1 )  {  offset += 4;  }
                    buff_len = pEventArgs->pBufSock->leftShiftBuffer( offset + frame.payload_len );
                    continue;
                }
            }

            uint32 data_len = frame.payload_len + offset + 4/*mask*/;

            if( buff_len >= data_len )
            {
                ///  如果缓存中已经收够了一个完整的消息
                
                if( frame.mask == 1 )
                {
                    // load mask
                    memcpy( frame.masking_key, buff+offset, 4 );
                    offset += 4;

                    // unmask
                    unmask_payload_data( frame, buff+offset );
                }
                 

                 
                if( frame.fin==0 || frame.fin==1 || frame.fin==2 )
                {
                    CSString msg_buff;
                    msg_buff.assign( buff+offset, buff+offset+frame.payload_len );


                    if( msg_buff.size() >= 2 )
                    {
                        //uint8 msg_type_len = (uint8)msg_buff[0];


                        //if( msg_type_len <= msg_buff.size()-1 )
                        {

                            //CSString msg_type = msg_buff.substr( 1, msg_type_len );
                            //CMessage msg(  msg_type );
                            CMessage msg("TestWebsock");

                            //if( msg_buff.size()-msg_type_len-1 > 0 )
                            if( msg_buff.size() > 0 )
                            {
                                //msg.serialBufferWithSize( (uint8*)&*msg_buff.begin(), msg_buff.size()-msg_type_len-1);
                                msg.serialBufferWithSize( (uint8*)&*msg_buff.begin(), msg_buff.size());
                            }

                            uint8 event_type    = CBufNetBase::User;
                            uint64 sockid       = (uint64)pEventArgs->pBufSock;
                            msg.serial( sockid );
                            msg.serial( event_type );

                            pEventArgs->pServer->pushMessageIntoReceiveQueue( msg.buffer(), msg.length() );
                        }
                    }

                    nlinfo( "WebSocketRecive: %s", msg_buff.c_str() );
                }




                buff_len = pEventArgs->pBufSock->leftShiftBuffer(data_len);
            }
            else
            {
                break;
            }

        }



        //memcpy( read_buffer, pEventArgs->pBufSock, sizeof(TSockId) );
        //read_buffer[sizeof(TSockId)] = (uint8)CBufNetBase::User;

        //pEventArgs->pBufSock->fillSockIdAndEventType()
        //
        //pEventArgs->pBufSock->advertiseSystemEvent( pEventArgs->pServer, )
        //pEventArgs->pServer->pushMessageIntoReceiveQueue( read_buffer, len+sizeof(TSockId)+1 );


        //    _Server->pushMessageIntoReceiveQueue( msg, len );
    }



}  


void socket_event_cb(bufferevent *bev, short events, void *args)  
{  
    if (events & BEV_EVENT_EOF)  
        nlinfo("connection closed\n");  
    else if (events & BEV_EVENT_ERROR)  
        nlinfo("some other error\n");

    nlinfo( "socket_event_cb:%d", events );

    SEventArgs* pEventArgs = (SEventArgs*)args;

    pEventArgs->pBufSock->advertiseDisconnection( pEventArgs->pServer, pEventArgs->pBufSock );

    delete pEventArgs;

    //这将自动close套接字和free读写缓冲区  
    bufferevent_free(bev);  
}

//一个新客户端连接上服务器了  
//当此函数被调用时，libevent已经帮我们accept了这个客户端。该客户端的
//文件描述符为fd
void listener_cb(   evconnlistener *listener, evutil_socket_t fd,  
                    struct sockaddr *sock, int socklen, void *args )  
{  
    printf("accept a client %d\n", fd);  

    SListenArgs* pListenArgs = (SListenArgs*)args;


	SOCKET newSock = (SOCKET)fd;

	if ( newSock == INVALID_SOCKET )
	{
        throw ESocket( "Accept returned an invalid socket");
	}

	// Construct and save a CTcpSock object
	CInetAddress addr;
	addr.setSockAddr( (struct sockaddr_in*)sock );
	//LNETL0_DEBUG( "LNETL0: Socket %d accepted an incoming connection from %s, opening socket %d", _Sock, addr.asString().c_str(), newsock );
	CTcpSock *pTcpSock = new CTcpSock( newSock, addr );
    CServerBufSock *pBufSock = new CServerBufSock( pTcpSock );
    LNETL1_DEBUG( "LNETL1: New connection : %s", pBufSock->asString().c_str() );

    // Notify the new connection
    pBufSock->advertiseConnection( pListenArgs->pServer );

    SEventArgs* pEventArgs = new SEventArgs( pBufSock, pListenArgs->pServer );

    //为这个客户端分配一个bufferevent  
    bufferevent *bev =  bufferevent_socket_new(pListenArgs->pEventBase, fd, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);
    bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, (void*)pEventArgs);
    bufferevent_enable(bev, EV_READ | EV_PERSIST); 
}  



/***************************************************************************************************
 * User main thread (initialization)
 **************************************************************************************************/

/*
 * Constructor
 */
CBufServerWebsocket::CBufServerWebsocket() : _ConnectionCallback(NULL)
{
	nlnettrace( "CBufServerWebsocket::CBufServerWebsocket" );

    _WebSocketReceiveTask = new CWebSocketReceiveTask();
}


/*
 * Listens on the specified port
 */
void CBufServerWebsocket::init( uint16 port )
{
	nlnettrace( "CBufServerWebsocket::init" );



    _MaxExpectedBlockSize = maxExpectedBlockSize();

    _WebSocketReceiveTask->init( this, port );
    _WebSocketReceiveTask->start();





}

/*
 * Destructor
 */
CBufServerWebsocket::~CBufServerWebsocket()
{
	nlnettrace( "CBufServerWebsocket::~CBufServerWebsocket" );

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

	nlnettrace( "Exiting CBufServerWebsocket::~CBufServer" );
}


/*
 * Disconnect the specified host
 * Set hostid to NULL to disconnect all connections.
 * If hostid is not null and the socket is not connected, the method does nothing.
 * If quick is true, any pending data will not be sent before disconnecting.
 */
void CBufServerWebsocket::disconnect( TSockId hostid, bool quick )
{
	nlnettrace( "CBufServerWebsocket::disconnect" );
	//if ( hostid != InvalidSockId )
	//{
	//	if (_ConnectedClients.find(hostid) == _ConnectedClients.end())
	//	{
	//		// this host is not connected
	//		return;
	//	}

	//	// Disconnect only if physically connected
	//	if ( hostid->Sock->connected() )
	//	{
	//		if ( ! quick )
	//		{
	//			hostid->flush();
	//		}
	//		hostid->Sock->disconnect(); // the connection will be removed by the next call of update()
	//	}
	//}
	//else
	//{
	//	// Disconnect all

	//}
}


/*
 * Send a message to the specified host
 */
void CBufServerWebsocket::send( const CMemStream& buffer, TSockId hostid )
{
	nlnettrace( "CBufServerWebsocket::send" );
	nlassert( buffer.length() > 0 );
	nlassertex( buffer.length() <= maxSentBlockSize(), ("length=%u max=%u", buffer.length(), maxSentBlockSize()) );

	// slow down the layer H_AUTO (CBufServer_send);

	//if ( hostid != InvalidSockId )
	//{
	//	if (_ConnectedClients.find(hostid) == _ConnectedClients.end())
	//	{
	//		// this host is not connected
	//		return;
	//	}

	//	pushBufferToHost( buffer, hostid );
	//}
	//else
	//{
	//	// Push into all send queues
	//	CThreadPool::iterator ipt;
	//	{
	//		CSynchronized<CThreadPool>::CAccessor poolsync( &_ThreadPool );
	//		for ( ipt=poolsync.value().begin(); ipt!=poolsync.value().end(); ++ipt )
	//		{
	//			CServerReceiveTask *task = receiveTask(ipt);
	//			CConnections::iterator ipb;
	//			{
	//				CSynchronized<CConnections>::CAccessor connectionssync( &task->_Connections );
	//				for ( ipb=connectionssync.value().begin(); ipb!=connectionssync.value().end(); ++ipb )
	//				{
	//					// Send only if the socket is logically connected
	//					if ( (*ipb)->connectedState() )
	//					{
	//						pushBufferToHost( buffer, *ipb );
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}


/*
 * Checks if there are some data to receive
 */
bool CBufServerWebsocket::dataAvailable()
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
					LNETL1_DEBUG( "LNETL1: Disconnection event for %p %s", sockid, sockid->asString().c_str());

					sockid->setConnectedState( false );

					// Call callback if needed
					if ( disconnectionCallback() != NULL )
					{
						disconnectionCallback()( sockid, argOfDisconnectionCallback() );
					}

					// remove from the list of valid client
					nlverify(_ConnectedClients.erase(sockid) == 1);

					// Add socket object into the synchronized remove list
					LNETL1_DEBUG( "LNETL1: Adding the connection to the remove list" );
					//nlassert( ((CServerBufSock*)sockid)->ownerTask() != NULL );
					//((CServerBufSock*)sockid)->ownerTask()->addToRemoveSet( sockid );

                    delete sockid;
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
void	CBufServerWebsocket::sleepUntilDataAvailable( uint usecMax )
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
void CBufServerWebsocket::receive( CMemStream& buffer, TSockId* phostid )
{
	nlnettrace( "CBufServerWebsocket::receive" );
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
void CBufServerWebsocket::update()
{
	//nlnettrace( "CBufServer::update-BEGIN" );

	_NbConnections = 0;

	// For each thread


	//nlnettrace( "CBufServer::update-END" );
}

uint32 CBufServerWebsocket::getSendQueueSize( TSockId destid )
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

void CBufServerWebsocket::displayThreadStat (NLMISC::CLog *log)
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

void CBufServerWebsocket::setTimeFlushTrigger( TSockId destid, sint32 ms )
{
	nlassert( destid != InvalidSockId );
	if (_ConnectedClients.find(destid) != _ConnectedClients.end())
		destid->setTimeFlushTrigger( ms );
}

void CBufServerWebsocket::setSizeFlushTrigger( TSockId destid, sint32 size )
{
	nlassert( destid != InvalidSockId );
	if (_ConnectedClients.find(destid) != _ConnectedClients.end())
		destid->setSizeFlushTrigger( size );
}

bool CBufServerWebsocket::flush( TSockId destid, uint *nbBytesRemaining)
{
	nlassert( destid != InvalidSockId );
	if (_ConnectedClients.find(destid) != _ConnectedClients.end())
		return destid->flush( nbBytesRemaining );
	else
		return true;
}
const CInetAddress& CBufServerWebsocket::hostAddress( TSockId hostid )
{
	nlassert( hostid != InvalidSockId );
	if (_ConnectedClients.find(hostid) != _ConnectedClients.end())
		return hostid->Sock->remoteAddr();

	static CInetAddress nullAddr;
	return nullAddr;
}

void CBufServerWebsocket::displaySendQueueStat (NLMISC::CLog *log, TSockId destid)
{

}


/*
 * Returns the number of bytes received since the previous call to this method
 */
uint64 CBufServerWebsocket::newBytesReceived()
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
uint64 CBufServerWebsocket::newBytesSent()
{
	uint64 b = bytesSent();
	uint64 nbsent = b - _PrevBytesPushedOut;
	//nlinfo( "b: %" NL_I64 "u   new: %" NL_I64 "u", b, nbsent );
	_PrevBytesPushedOut = b;
	return nbsent;
}



void CWebSocketReceiveTask::init( CBufServerWebsocket *server, uint16 port )
{
	_Server = server;

    struct sockaddr_in sin;  
    memset(&sin, 0, sizeof(struct sockaddr_in));  
    sin.sin_family = AF_INET;  
    sin.sin_port = htons(port);  

    pEventBase = event_base_new();
    SListenArgs* pListenArgs = new struct SListenArgs(pEventBase,_Server);
    

    pEvListener = evconnlistener_new_bind(  pEventBase, listener_cb, (void*)pListenArgs, 
                                            LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, 10, 
                                            (struct sockaddr*)&sin, sizeof(struct sockaddr_in));  

}

void CWebSocketReceiveTask::run()
{
	nlnettrace( "CWebSocketReceiveTask::run" );
    event_base_dispatch(pEventBase);  
}

bool CWebSocketReceiveTask::start()
{
    _ProcTableThread = NLMISC::IThread::create( this );

    if ( _ProcTableThread != NULL )
    {
        _ProcTableThread->start();
        _ExitRequired = false;
    }

    return ( NULL != _ProcTableThread );
}

void CWebSocketReceiveTask::close()
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
