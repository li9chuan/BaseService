#ifndef SERVER_SHARD_LUA_NETWORK_WEBSOCKET_H
#define SERVER_SHARD_LUA_NETWORK_WEBSOCKET_H

#include <nel/net/message.h>
#include <nel/net/callback_server_websocket.h>
#include "server_share/bin_luabind/Public.hpp"
#include "lua_network_base.h"

class CLuaNetworkWebSocket : public CLuaBaseNetwork
{
    DECLARE_SCRIPT_SUB_CLASS(CLuaBaseNetwork)
public:

    CLuaNetworkWebSocket( std::string name, uint16 port );

    ~CLuaNetworkWebSocket()
    {
        LuaNetworkMgr.RemoveNetModule(m_NetName);
        delete m_CallbackServerHandle; 
    }

    void Update()
    {
        if( m_CallbackServerHandle->connected() )
        {
            m_CallbackServerHandle->update();
        }
    }

    void Send( NLNET::TSockId sock_id, const NLNET::CMessage &buffer )
    {
        m_CallbackServerHandle->send( buffer, sock_id );
    }

private:

    NLNET::CCallbackServerWebSocket*           m_CallbackServerHandle;
};


#endif  //  SERVER_SHARD_LUA_NETWORK_WEBSOCKET_H

