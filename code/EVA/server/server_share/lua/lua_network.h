#ifndef SERVER_SHARD_LUA_NETWORK_H
#define SERVER_SHARD_LUA_NETWORK_H

#include <nel/misc/singleton.h>

class CLuaClientNetwork; 

class CLuaNetworkMgr : public NLMISC::CSingleton<CLuaNetworkMgr>
{
public:

    void Init();

    void RegisterNetModule( std::string name, CLuaClientNetwork* pNet );
    void RemoveNetModule( std::string name );

    void SendToClient( uint64 uid, std::string& msg_type, std::string& pb_buff );

    void Update();


    void Release();



private:

    typedef std::map<std::string, CLuaClientNetwork*>    TNetHandle;
    TNetHandle          m_LuaClientNetworkHandle;
};

#define  LuaNetworkMgr  CLuaNetworkMgr::instance()


#endif
