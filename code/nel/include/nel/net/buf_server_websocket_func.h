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

#ifndef NL_WEBSOCKET_FUN_H
#define NL_WEBSOCKET_FUN_H

#include "nel/misc/types_nl.h"
#include "nel/misc/sstring.h"
#include "nel/misc/object_vector.h"

#include "event2/event.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"

namespace NLNET {

#define WEBSOCK_FRAME_HAVE_NEXT         (0x0)           ///     表示一个延续帧。当Opcode为0时，表示本次数据传输采用了数据分片，当前收到的数据帧为其中一个数据分片。
#define WEBSOCK_FRAME_TEXT	            (0x1)           ///     %x1：表示这是一个文本帧（frame）    %x2：表示这是一个二进制帧（frame）
#define WEBSOCK_FRAME_BIN	            (0x2)
#define WEBSOCK_FRAME_DIS_CONNECT	    (0x8)
#define WEBSOCK_FRAME_PING	            (0x9)
#define WEBSOCK_FRAME_PONG	            (0xa)


    inline uint16 myhtons(uint16 n) {
        return ((n & 0xff00) >> 8) | ((n & 0x00ff) << 8);
    }

    inline uint16 myntohs(uint16 n) {
        return ((n & 0xff00) >> 8) | ((n & 0x00ff) << 8);
    }

    inline uint32 myhtonl(uint32 n) {
        return ((n & 0xff000000) >> 24) | ((n & 0x00ff0000) >> 8) | ((n & 0x0000ff00) << 8) | ((n & 0x000000ff) << 24);
    }

    inline uint32 myntohl(uint32 n) {
        return ((n & 0xff000000) >> 24) | ((n & 0x00ff0000) >> 8) | ((n & 0x0000ff00) << 8) | ((n & 0x000000ff) << 24);
    }

    inline uint64 myhtonll(uint64 n) {
        return (uint64)myhtonl(n >> 32) | ((uint64)myhtonl(n) << 32);
    }

    inline uint64 myntohll(uint64 n) {
        return (uint64)myhtonl(n >> 32) | ((uint64)myhtonl(n) << 32);
    }

    class CBufServerWebsocket;
    class CServerBufSock;

    struct WSListenArgs
    {
        event_base*             pEventBase;
        CBufServerWebsocket*    pServer;
        void*                   pSslCtx;
        WSListenArgs( event_base* eventbase, CBufServerWebsocket* bufsvr ) : pEventBase(eventbase), pServer(bufsvr), pSslCtx(NULL) {}
    };

    struct WebSocketFrame
    {
        uint8   fin;
        uint8   opcode;
        uint8   mask;
        uint64  payload_len;
        unsigned char masking_key[4];
    };

    NLMISC::CSString generate_key(const NLMISC::CSString &key);


    NLMISC::CSString generate_websocket_response( NLMISC::CSString& sec_websocket_key ); 


    sint32  parse_frame_header( const uint8 *buf, WebSocketFrame& frame );


    inline void unmask_payload_data( WebSocketFrame& frame, uint8* buff )
    {
        for ( uint32 i = 0; i < frame.payload_len; ++i )
        {
            *(buff + i) = *(buff + i) ^ *(frame.masking_key + i % 4);
        }
    }

    void    fill_frame_buffer( const uint8* payload_data, uint32 payload_len, NLMISC::CObjectVector<uint8>& out_frame, uint8 opcode, uint8 fin=1 );

    void    ws_socket_event_cb( bufferevent *bev, short events, void *args );

    void    ws_socket_read_cb( bufferevent *bev, void *args ); 

    //一个新客户端连接上服务器了  
    //当此函数被调用时，libevent已经帮我们accept了这个客户端。该客户端的
    //文件描述符为fd
    void    ws_listener_cb( evconnlistener *listener, evutil_socket_t fd, sockaddr *sock, int socklen, void *args );  
}


#endif // NL_WEBSOCKET_FUN_H

/* End of web_sock_fun.h */
