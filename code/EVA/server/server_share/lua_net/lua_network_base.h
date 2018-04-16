#ifndef SERVER_SHARD_LUA_NETWORK_BASE_H
#define SERVER_SHARD_LUA_NETWORK_BASE_H

#include <nel/misc/singleton.h>
#include "lua_network.h"
#include <server_share/game_def.h>
#include <server_share/server_def.h>
#include <server_share/bin_luabind/Public.hpp>

class CLuaBaseNetwork
{
    DECLARE_SCRIPT_CLASS()
public:
    CLuaBaseNetwork( std::string name ) : m_NetName(name)
    {
        LuaNetworkMgr.RegisterNetModule( name, this );
    }

    virtual void Update() = 0;
    virtual void Send( NLNET::TSockId sock_id, std::string& msg_type, std::string& proto_str ) {};



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
};

#endif          // SERVER_SHARD_LUA_NETWORK_BASE_H
