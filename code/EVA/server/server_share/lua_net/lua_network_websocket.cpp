#include "lua_network_websocket.h"
#include "lua_network.h"
#include <server_share/lua/script_mgr.h>

using namespace NLNET;

void cbLuaClientMsg ( CMessage &msgin, TSockId from, CCallbackNetBase &netbase)
{
    uint64          msg_from = (uint64)from;
    std::string     msg_type;
    std::string     msg_buff;

    msgin.serial(msg_type);
    msgin.serial(msg_buff);

    LuaParams lua_params( msg_from, msg_type, msg_buff );  
    ScriptMgr.run( "NetWorkHandler", "OnMessage", lua_params );
}

const TCallbackItem LuaWebsocketCallbackArray[] =
{
    { "_LC",                            cbLuaClientMsg              },
};



CLuaNetworkWebSocket::CLuaNetworkWebSocket( std::string name, uint16 port ) : CLuaBaseNetwork(name)
{
    m_CallbackServerHandle = new CCallbackServerWebSocket();
    m_CallbackServerHandle->addCallbackArray(LuaWebsocketCallbackArray, sizeof(LuaWebsocketCallbackArray)/sizeof(LuaWebsocketCallbackArray[0]));

    m_CallbackServerHandle->setConnectionCallback( cbLuaSvrConnect, this );
    m_CallbackServerHandle->setDisconnectionCallback( cbLuaSvrDisConnect, this );

    m_CallbackServerHandle->init (port);
}

