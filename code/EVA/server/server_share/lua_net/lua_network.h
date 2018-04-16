#ifndef SERVER_SHARD_LUA_NETWORK_H
#define SERVER_SHARD_LUA_NETWORK_H

#include <nel/misc/singleton.h>
#include <nel/net/unified_network.h>
#include <nel/net/buf_net_base.h>

class CLuaBaseNetwork; 

void cbLuaSvrConnect( NLNET::TSockId from, void *arg );
void cbLuaSvrDisConnect( NLNET::TSockId from, void *arg );
void cbConnection( const std::string &serviceName, NLNET::TServiceId sid, void *arg );
void cbDisconnection( const std::string &serviceName, NLNET::TServiceId sid, void *arg );

class CLuaNetworkMgr : public NLMISC::CSingleton<CLuaNetworkMgr>
{
public:

    void Init();

    void RegisterNetModule( std::string name, CLuaBaseNetwork* pNet );
    void RemoveNetModule( std::string name );

    void SendToClient( uint64 uid, std::string& msg_type, std::string& pb_buff );

    void Update();


    void Release();



private:

    typedef std::map<std::string, CLuaBaseNetwork*>    TNetHandle;
    TNetHandle          m_LuaClientNetworkHandle;
};

#define  LuaNetworkMgr  CLuaNetworkMgr::instance()


#endif  // SERVER_SHARD_LUA_NETWORK_H
