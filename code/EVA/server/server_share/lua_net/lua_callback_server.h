#ifndef SERVER_SHARD_LUA_CALLBACK_SERVER_H
#define SERVER_SHARD_LUA_CALLBACK_SERVER_H

#include <nel/misc/singleton.h>
#include "lua_network.h"
#include <server_share/game_def.h>
#include <server_share/server_def.h>
#include <server_share/bin_luabind/Public.hpp>

struct  ClientData
{
    DEF::UID            uid;
    NLNET::TSockId      sid;
    NLNET::TServiceId   pls_sid;
};

struct MsgLeaf;

class CLuaCallbackServer
{
    DECLARE_SCRIPT_CLASS()
public:

    CLuaCallbackServer( std::string& name, std::string& protocal, uint16 port );
    ~CLuaCallbackServer();

    void    Update()
    {
        if( m_CallbackServerHandle->connected() )
        {
            m_CallbackServerHandle->update();
        }
    }

    void    Send( NLNET::TSockId sock_id, const NLNET::CMessage &buffer )
    {
        m_CallbackServerHandle->send( buffer, sock_id );
    }

    void    IncReceiveMsgCount( std::string msg_name );
    bool    ForwardingMsg( ClientData* pClient, NLNET::CMessage& msgin, MsgLeaf* pMsgLeaf );

    void    SetClientData( ClientData& client_data )
    {
        TClientMap::iterator iter = m_ClientMap.find( client_data.uid );

        if ( iter != m_ClientMap.end() )
        {
            m_SockClient.erase(iter->second.sid);
            m_SockClient[client_data.sid] = client_data;

            iter->second.sid        = client_data.sid;
            iter->second.pls_sid    = client_data.pls_sid;
        }
        else
        {
            m_ClientMap[client_data.uid]    = client_data;
            m_SockClient[client_data.sid]   = client_data;
        }
    }
    void    RemoveClientData( DEF::UID uid )
    {
        TClientMap::iterator iter = m_ClientMap.find( uid );

        if ( iter != m_ClientMap.end() )
        {
            m_SockClient.erase(iter->second.sid);
            m_ClientMap.erase(uid); 
        }
    }

    void    ClearClientData()
    {
        m_SockClient.clear();
        m_ClientMap.clear(); 
    }

    NLNET::TSockId     GetSockId( DEF::UID uid )           
    {
        TClientMap::iterator iter = m_ClientMap.find(uid);
        if ( iter != m_ClientMap.end() )
        {
            return iter->second.sid;
        }

        return NLNET::InvalidSockId;
    }

    ClientData*         GetClientData( NLNET::TSockId sock_id )
    {
        ClientData* pClient = NULL;
        TSockClient::iterator iter = m_SockClient.find(sock_id);

        if ( iter != m_SockClient.end() )
        {
            pClient = &iter->second;
        }

        return pClient;
    }

    std::string         GetName()   { return m_NetName; }

    typedef std::map<std::string, uint64>                   TMsgCount;

    TMsgCount                                               m_ReceiveMsgCount;
    TMsgCount                                               m_SendMsgCount;

private:

    std::string                                             m_NetName;

    typedef     std::map<DEF::UID, ClientData>              TClientMap;
    TClientMap                                              m_ClientMap;

    typedef     std::map<NLNET::TSockId, ClientData>        TSockClient;
    TSockClient                                             m_SockClient;

    NLNET::CCallbackNetBase*                                m_CallbackServerHandle;

    std::string                                             m_StrBuffer;
    std::string                                             m_SaveEventStr;
};

#endif          // SERVER_SHARD_LUA_CALLBACK_SERVER_H
