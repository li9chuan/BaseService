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



    uint32              GetHandle() { return m_MyHandle; }

    uint32              m_MyHandle;

private:

    std::string                                             m_Protocal;

    NLNET::CCallbackNetBase*                                m_CallbackClientHandle;

    std::string                                             m_StrBuffer;
    std::string                                             m_SaveEventStr;
};


class CLuaClientMgr : public NLMISC::CSingleton<CLuaClientMgr>
{
public:

    void    Init() { m_ClientHandle = 0; }

    uint32  RegisterClient(CLuaCallbackClient* pNet);
    void    RemoveClient(uint32 client_handle);

    void    Update();

    void    Release();



private:

    typedef std::map<int, CLuaCallbackClient*>    TNetHandle;
    TNetHandle          m_LuaClientNetworkHandle;

    int                 m_ClientHandle;
};

#define  LuaClientMgr  CLuaClientMgr::instance()

#endif          // SERVER_SHARD_LUA_CALLBACK_CLIENT_H
