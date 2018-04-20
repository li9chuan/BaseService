#ifndef SERVER_SHARD_LUA_CALLBACK_SERVER_H
#define SERVER_SHARD_LUA_CALLBACK_SERVER_H

#include <nel/misc/singleton.h>
#include "lua_network.h"
#include <server_share/game_def.h>
#include <server_share/server_def.h>
#include <server_share/bin_luabind/Public.hpp>

class CLuaCallbackServer
{
    DECLARE_SCRIPT_CLASS()
public:

    CLuaCallbackServer( std::string& name, std::string& protocal, uint16 port );
    ~CLuaCallbackServer();

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



    void SetUIDMap( DEF::UID uid, NLNET::TSockId sock_id )  { m_UIDMap[uid] = sock_id; }
    void RemoveUIDMap( DEF::UID uid )                       { m_UIDMap.erase(uid); }
    void ClearUIDMap()                                      { m_UIDMap.clear(); }

    NLNET::TSockId     GetSockId( DEF::UID uid )           
    {
        TUIDMap::iterator iter = m_UIDMap.find(uid);
        if ( iter != m_UIDMap.end() )
        {
            return iter->second;
        }

        return NLNET::InvalidSockId;
    }

    std::string         GetName()   { return m_NetName; }

protected:

    std::string                                     m_NetName;

    typedef     std::map<DEF::UID, NLNET::TSockId>  TUIDMap;
    TUIDMap                                         m_UIDMap;

    NLNET::CCallbackNetBase*                        m_CallbackServerHandle;
};

#endif          // SERVER_SHARD_LUA_CALLBACK_SERVER_H
