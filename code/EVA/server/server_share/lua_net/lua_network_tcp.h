#ifndef SERVER_SHARD_LUA_NETWORK_TCP_H
#define SERVER_SHARD_LUA_NETWORK_TCP_H

#include <nel/net/message.h>
#include <nel/net/callback_server_tcp.h>
#include "lua_network_base.h"

class CLuaNetworkTcp : public CLuaBaseNetwork
{
    DECLARE_SCRIPT_SUB_CLASS(CLuaBaseNetwork)
public:

    CLuaNetworkTcp( std::string name, uint16 port );

    ~CLuaNetworkTcp()
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

    void Send( const NLNET::CMessage &buffer, NLNET::TSockId sock_id )
    {
        m_CallbackServerHandle->send( buffer, sock_id );
    }


private:

    NLNET::CCallbackServerTcp*              m_CallbackServerHandle;
};


#endif  //  SERVER_SHARD_LUA_NETWORK_TCP_H

