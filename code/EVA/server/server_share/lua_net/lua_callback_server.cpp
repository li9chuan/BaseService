#include "lua_callback_server.h"
#include "lua_message.h"
#include <server_share/lua/script_mgr.h>
#include <nel/net/callback_server_websocket.h>
#include <nel/net/callback_server_tcp.h>


using namespace bin;
using namespace NLNET;

static CLuaMessage* pLuaMsg = new CLuaMessage();

void cbLuaServiceMsg ( CMessage &msgin, TSockId from, CCallbackNetBase &netbase )
{
    CScriptTable    functbl;
    ScriptMgr.GetScriptHandle()->Get("NetWorkHandler", functbl);

    int nRet = 0;
    pLuaMsg->m_Msg.swap(msgin);

    functbl.CallFunc<lua_Integer, CLuaMessage*, int>("OnMessage", (lua_Integer)from, pLuaMsg, nRet);
}


CLuaCallbackServer::CLuaCallbackServer( std::string& name, std::string& protocal, uint16 port ) : m_NetName(name)
{
    if( protocal=="ws" || protocal=="websocket" )
    {
        CCallbackServerWebSocket* pServer = new CCallbackServerWebSocket();
        pServer->setConnectionCallback( cbLuaSvrConnect, this );
        pServer->setDisconnectionCallback( cbLuaSvrDisConnect, this );
        pServer->setDefaultCallback(cbLuaServiceMsg);
        pServer->init (port);

        m_CallbackServerHandle = pServer;
    }
    else if ( protocal=="tcp" )
    {
        CCallbackServerTcp* pServer = new CCallbackServerTcp();
        pServer->setConnectionCallback( cbLuaSvrConnect, this );
        pServer->setDisconnectionCallback( cbLuaSvrDisConnect, this );
        pServer->setDefaultCallback(cbLuaServiceMsg);
        pServer->init (port);

        m_CallbackServerHandle = pServer;
    }

    nlassert(m_CallbackServerHandle!=NULL);
    LuaNetworkMgr.RegisterNetModule( name, this );
}

CLuaCallbackServer::~CLuaCallbackServer()
{
    LuaNetworkMgr.RemoveNetModule(m_NetName);
    delete m_CallbackServerHandle; 
}













