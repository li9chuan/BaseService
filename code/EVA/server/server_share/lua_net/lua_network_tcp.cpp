#include "lua_network_tcp.h"

using namespace NLNET;

extern void cbLuaServiceMsg ( CMessage &msgin, TSockId from, CCallbackNetBase &netbase );

CLuaNetworkTcp::CLuaNetworkTcp( std::string name, uint16 port ) : CLuaBaseNetwork(name)
{
    m_CallbackServerHandle = new CCallbackServerTcp();
    //m_CallbackServerHandle->addCallbackArray(LuaTcpCallbackArray, sizeof(LuaTcpCallbackArray)/sizeof(LuaTcpCallbackArray[0]));

    m_CallbackServerHandle->setConnectionCallback( cbLuaSvrConnect, this );
    m_CallbackServerHandle->setDisconnectionCallback( cbLuaSvrDisConnect, this );

    m_CallbackServerHandle->setDefaultCallback(cbLuaServiceMsg);

    m_CallbackServerHandle->init (port);
}



