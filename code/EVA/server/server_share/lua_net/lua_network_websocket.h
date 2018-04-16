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

    void Send( NLNET::TSockId sock_id, std::string& msg_type, std::string& proto_str )
    {
        NLMISC::CMemStream mem_out;

        //uint32 buff_len = proto_str.size() + msg_type.size() + 1 + 4;
        //mem_out.serial(buff_len);
        uint8 msg_type_len = msg_type.size();
        mem_out.serial(msg_type_len);
        mem_out.serialBuffer( (uint8*)msg_type.c_str(), msg_type_len );
        mem_out.serialBuffer( (uint8*)proto_str.c_str(), proto_str.size() );

        m_CallbackServerHandle->send_buffer( mem_out, sock_id );
    }


private:

    NLNET::CCallbackServerWebSocket*           m_CallbackServerHandle;
};


#endif  //  SERVER_SHARD_LUA_NETWORK_WEBSOCKET_H

