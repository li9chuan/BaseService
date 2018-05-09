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

#ifndef NL_BUF_SERVER_TCP_FUNC_H
#define NL_BUF_SERVER_TCP_FUNC_H

#include "nel/misc/types_nl.h"
#include "nel/misc/sstring.h"

#include "event2/event.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"


namespace NLNET {

    class CBufServerTcp;
    class CServerBufSock;

    struct TcpListenArgs
    {
        event_base*             pEventBase;
        CBufServerTcp*          pServer;
        TcpListenArgs( event_base* eventbase, CBufServerTcp* bufsvr ) : pEventBase(eventbase), pServer(bufsvr) {}
    };


    void    tcp_socket_event_cb( bufferevent *bev, short events, void *args );

    void    tcp_socket_read_cb( bufferevent *bev, void *args ); 

    void    tcp_listener_cb( evconnlistener *listener, evutil_socket_t fd, sockaddr *sock, int socklen, void *args );  


}


#endif // NL_BUF_SERVER_TCP_FUNC_H

/* End of buf_server_tcp_func.h */
