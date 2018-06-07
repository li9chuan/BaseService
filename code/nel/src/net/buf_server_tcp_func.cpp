#include "nel/net/buf_server_tcp_func.h"
#include "nel/misc/sha1.h"
#include "nel/misc/base64.h"
#include "nel/net/buf_sock.h"
#include "nel/net/callback_server_tcp.h"
#include "nel/net/buf_server_tcp.h"

using namespace std;
using namespace NLMISC;
using namespace NLNET;


void NLNET::tcp_socket_read_cb( bufferevent *bev, void *args )
{  
    char read_buffer[4096];  
    size_t  len = bufferevent_read(bev, read_buffer, sizeof(read_buffer) );
    CServerBufSock* pBufSock = (CServerBufSock*)args;
    uint32 buff_len = pBufSock->appendToBuffer( (const uint8*)read_buffer, len );

    while( buff_len >= sizeof(TBlockSize) )
    {
        uint8* buff         = pBufSock->getBuffer();
        uint32 data_len     = ntohl(*(uint32*)(buff));

        /// 如果缓存中已经收够了一个完整的消息
        if ( buff_len >= data_len+sizeof(TBlockSize) )
        {
            NLMISC::CMemStream& msg=pBufSock->CompleteMsg;

            if (msg.isReading())
            {
                msg.invert();
            }

            msg.fill( buff+sizeof(TBlockSize), data_len );

            uint8 event_type    = CBufNetBase::User;
            uint64 sockid       = (uint64)pBufSock;
            msg.serial( sockid );
            msg.serial( event_type );
            msg.invert();

            pBufSock->m_BufNetHandle->pushMessageIntoReceiveQueue( msg.buffer(), msg.size() );
            buff_len = pBufSock->leftShiftBuffer(data_len+sizeof(TBlockSize));
        }
        else
        {
            break;
        }
    }
}  

void NLNET::tcp_socket_event_cb( bufferevent *bev, short events, void *args )
{  
    if (events & BEV_EVENT_EOF)  
        LNETL1_DEBUG("connection closed\n");  
    else if (events & BEV_EVENT_ERROR)  
        LNETL1_DEBUG("some other error\n");

    LNETL1_DEBUG( "tcp_socket_event_cb:%d", events );

    CServerBufSock* pBufSock = (CServerBufSock*)args;
    pBufSock->advertiseDisconnection( pBufSock->m_BufNetHandle, pBufSock );
}

void NLNET::tcp_listener_cb( evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sock, int socklen, void *args )
{  
    TcpListenArgs*  pListenArgs = (TcpListenArgs*)args;
    SOCKET          newSock = (SOCKET)fd;

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

    //为这个客户端分配一个bufferevent  
    bufferevent *bev =  bufferevent_socket_new(pListenArgs->pEventBase, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE | BEV_OPT_DEFER_CALLBACKS);

    pBufSock->m_BufNetHandle    = pListenArgs->pServer;
    pBufSock->m_BEVHandle       = bev;

    bufferevent_setcb(bev, tcp_socket_read_cb , NULL, tcp_socket_event_cb, (void*)pBufSock);
    bufferevent_enable(bev, EV_READ | EV_PERSIST); 
}  


