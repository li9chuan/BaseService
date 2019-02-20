#ifndef SERVER_SHARD_LUA_NETWORK_H
#define SERVER_SHARD_LUA_NETWORK_H

#include <nel/misc/singleton.h>
#include <nel/net/unified_network.h>
#include <nel/net/buf_net_base.h>

class CLuaCallbackServer; 

void cbLuaSvrConnect( NLNET::TSockId from, void *arg );
void cbLuaSvrDisConnect( NLNET::TSockId from, void *arg );
void cbConnection( const std::string &serviceName, NLNET::TServiceId sid, void *arg );
void cbDisconnection( const std::string &serviceName, NLNET::TServiceId sid, void *arg );

class CLuaNetworkMgr : public NLMISC::CSingleton<CLuaNetworkMgr>
{
public:

    void Init();

    void RegisterNetModule( std::string name, CLuaCallbackServer* pNet );
    void RemoveNetModule( std::string name );

    void SendToClient( uint64 uid, NLNET::CMessage& msgin );
    void IncReceiveMsgCount(std::string msg_name);

    void Update();


    void Release();

    typedef std::map<std::string, uint64>                   TMsgCount;

    TMsgCount                                               m_ReceiveMsgCount;
    TMsgCount                                               m_SendMsgCount;

private:

    typedef std::map<std::string, CLuaCallbackServer*>    TNetHandle;
    TNetHandle          m_LuaClientNetworkHandle;
};

#define  LuaNetworkMgr  CLuaNetworkMgr::instance()


#endif  // SERVER_SHARD_LUA_NETWORK_H
