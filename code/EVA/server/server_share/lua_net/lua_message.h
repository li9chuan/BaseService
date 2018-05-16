#ifndef SERVER_SHARD_LUA_NETWORK_MESSAGE_H
#define SERVER_SHARD_LUA_NETWORK_MESSAGE_H

#include <nel/net/message.h>
#include "server_share/bin_luabind/Public.hpp"

class CLuaMessage
{
    DECLARE_SCRIPT_CLASS();
public:

    CLuaMessage() {}

    CLuaMessage( std::string& name )
    {
        m_Msg.setType(name);
    }

    NLNET::CMessage             m_Msg;
};


#endif
