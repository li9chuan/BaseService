#include "lua_network_tcp.h"
#include "lua_message.h"
#include <server_share/lua/script_mgr.h>

using namespace NLNET;
using namespace bin;

static CLuaMessage* pLuaMsg = new CLuaMessage();

void cbLuaServiceMsg ( CMessage &msgin, TSockId from, CCallbackNetBase &netbase )
{
    CScriptTable    functbl;
    ScriptMgr.GetScriptHandle()->Get("NetWorkHandler", functbl);

    int nRet = 0;
    pLuaMsg->m_Msg.swap(msgin);

    functbl.CallFunc<lua_Integer, CLuaMessage*, int>("OnTestMessage", (lua_Integer)from, pLuaMsg, nRet);
}

CLuaNetworkTcp::CLuaNetworkTcp( std::string name, uint16 port ) : CLuaBaseNetwork(name)
{
    m_CallbackServerHandle = new CCallbackServerTcp();
    //m_CallbackServerHandle->addCallbackArray(LuaTcpCallbackArray, sizeof(LuaTcpCallbackArray)/sizeof(LuaTcpCallbackArray[0]));

    m_CallbackServerHandle->setConnectionCallback( cbLuaSvrConnect, this );
    m_CallbackServerHandle->setDisconnectionCallback( cbLuaSvrDisConnect, this );

    m_CallbackServerHandle->setDefaultCallback(cbLuaServiceMsg);

    m_CallbackServerHandle->init (port);
}



