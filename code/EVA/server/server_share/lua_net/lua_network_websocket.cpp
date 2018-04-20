#include "lua_network_websocket.h"
#include "lua_network.h"
#include <server_share/lua/script_mgr.h>

using namespace NLNET;

extern void cbLuaServiceMsg ( CMessage &msgin, TSockId from, CCallbackNetBase &netbase );

CLuaNetworkWebSocket::CLuaNetworkWebSocket( std::string name, uint16 port ) : CLuaBaseNetwork(name)
{
    m_CallbackServerHandle = new CCallbackServerWebSocket();
    //m_CallbackServerHandle->addCallbackArray(LuaWebsocketCallbackArray, sizeof(LuaWebsocketCallbackArray)/sizeof(LuaWebsocketCallbackArray[0]));

    m_CallbackServerHandle->setConnectionCallback( cbLuaSvrConnect, this );
    m_CallbackServerHandle->setDisconnectionCallback( cbLuaSvrDisConnect, this );

    m_CallbackServerHandle->setDefaultCallback(cbLuaServiceMsg);

    m_CallbackServerHandle->init (port);
}

