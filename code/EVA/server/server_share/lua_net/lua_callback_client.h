#ifndef SERVER_SHARD_LUA_CALLBACK_CLIENT_H
#define SERVER_SHARD_LUA_CALLBACK_CLIENT_H

#include <nel/misc/singleton.h>
#include "lua_network.h"
#include <server_share/game_def.h>
#include <server_share/server_def.h>
#include <server_share/bin_luabind/Public.hpp>

namespace bin {
    class CScriptHandle;
}

class CLuaMessage;

class CLuaCallbackClient
{
    DECLARE_SCRIPT_CLASS()
public:

    CLuaCallbackClient( std::string& protocal, sint32 thd_handle=-1 );
    ~CLuaCallbackClient();

    void    Connect( std::string& url );


    void    Update()
    {
        if(m_CallbackClientHandle->connected() )
        {
            m_CallbackClientHandle->update();
        }
    }

    void    Send( const NLNET::CMessage &buffer )
    {
        if (m_CallbackClientHandle->connected())
        {
            m_CallbackClientHandle->send(buffer);
        }
    }

    bool                Connected() { return m_CallbackClientHandle->connected(); }
    uint32              GetHandle() { return m_MyHandle; }
    void                SetHandle(uint32 handle) { m_MyHandle = handle; }

    uint32              m_MyHandle;
    CLuaMessage*        m_LuaTmpMsg;

private:

    std::string                                             m_Protocal;

    NLNET::CCallbackNetBase*                                m_CallbackClientHandle;

    std::string                                             m_StrBuffer;
    std::string                                             m_SaveEventStr;
};

#endif          // SERVER_SHARD_LUA_CALLBACK_CLIENT_H
