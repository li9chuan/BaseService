#include "lua_callback_client.h"
#include "lua_message.h"
#include <server_share/lua/script_mgr.h>
#include <nel/net/buf_client.h>
#include <nel/net/callback_client.h>

using namespace bin;
using namespace NLNET;

static CLuaMessage* pLuaMsg = new CLuaMessage();
    
void cbLuaClientMsg(CMessage &msgin, TSockId from, CCallbackNetBase &netbase)
{
    CLuaCallbackClient* pClient = (CLuaCallbackClient*)netbase.getUserData();

    CScriptTable    functbl;
    pClient->GetScriptHandle().Get("NetWorkHandler", functbl);

    int nRet = 0;
    pLuaMsg->m_Msg.swap(msgin);

    functbl.CallFunc<lua_Integer, CLuaMessage*, int>("OnMessage", (lua_Integer)from, pLuaMsg, nRet);
}

CLuaCallbackClient::CLuaCallbackClient( std::string& name, std::string& protocal, sint32 thd_handle/*=-1*/)
    : m_NetName(name), m_Protocal(protocal)
{
    if ( protocal=="tcp" )
    {
        CCallbackClient* pClient = new CCallbackClient();
        //pClient->setConnectionCallback( cbLuaSvrConnect, this );
        //pClient->setDisconnectionCallback( cbLuaSvrDisConnect, this );
        pClient->setDefaultCallback(cbLuaClientMsg);

        m_CallbackClientHandle = pClient;
    }

    m_CallbackClientHandle->setUserData(this);

    nlassert(m_CallbackClientHandle !=NULL);


}

CLuaCallbackClient::~CLuaCallbackClient()
{
    LuaNetworkMgr.RemoveNetModule(m_NetName);
    delete m_CallbackClientHandle;
}

void CLuaCallbackClient::Connect(std::string & url)
{
    CInetAddress addr(url);
    CCallbackClient* pClient = (CCallbackClient*)m_CallbackClientHandle;

    pClient->connect(addr);

}
