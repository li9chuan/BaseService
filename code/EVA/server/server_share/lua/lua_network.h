#ifndef SERVER_SHARD_LUA_NETWORK_H
#define SERVER_SHARD_LUA_NETWORK_H

#include <nel/misc/singleton.h>

class CLuaWebSocketNetwork; 

class CLuaNetworkMgr : public NLMISC::CSingleton<CLuaNetworkMgr>
{
public:

    void Init();

    void RegisterNetModule( std::string name, CLuaWebSocketNetwork* pNet );


    void Update();


    void Release();



private:

    typedef std::map<std::string, CLuaWebSocketNetwork*>    TNetHandle;
    TNetHandle          m_LuaWebSocketNetworkHandle;
};

#define  LuaNetworkMgr  CLuaNetworkMgr::instance()


#endif
