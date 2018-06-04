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

CLuaCallbackClient::CLuaCallbackClient( std::string& protocal, sint32 thd_handle/*=-1*/)
    : m_Protocal(protocal), m_MyHandle(0)
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

    m_MyHandle = LuaClientMgr.RegisterClient(this);
}

CLuaCallbackClient::~CLuaCallbackClient()
{
    LuaClientMgr.RemoveClient(m_MyHandle);
    delete m_CallbackClientHandle;
}

void CLuaCallbackClient::Connect(std::string & url)
{
    CInetAddress addr(url);
    CCallbackClient* pClient = (CCallbackClient*)m_CallbackClientHandle;

    pClient->connect(addr);

}

//////////

uint32 CLuaClientMgr::RegisterClient(CLuaCallbackClient * pNet)
{
    ++m_ClientHandle;
    pNet->m_MyHandle = m_ClientHandle;
    m_LuaClientNetworkHandle[m_ClientHandle] = pNet;
    return m_ClientHandle;
}

void CLuaClientMgr::RemoveClient(uint32 client_handle)
{
    m_LuaClientNetworkHandle.erase(client_handle);
}

void CLuaClientMgr::Update()
{
    TNetHandle::iterator iter = m_LuaClientNetworkHandle.begin();

    while (iter != m_LuaClientNetworkHandle.end())
    {
        iter->second->Update();
        iter++;
    }
}

void CLuaClientMgr::Release()
{
}

///////////////////
namespace bin
{
    BEGIN_SCRIPT_CLASS(LuaCallbackClient, CLuaCallbackClient)

    DEFINE_CLASS_FUNCTION(Send, void, (CLuaMessage* lua_msg))
    {
        obj->Send(lua_msg->m_Msg);
        return 1;
    }

    DEFINE_CLASS_FUNCTION(DisConnect, void, ())
    {
        return 1;
    }

    DEFINE_CLASS_FUNCTION(Connect, void, (std::string& url))
    {
        obj->Connect(url);
        return 1;
    }

    DEFINE_CLASS_FUNCTION(GetHandle, lua_Integer, ())
    {
        r = obj->GetHandle();
        return 1;
    }

    DEFINE_STATIC_FUNCTION(NewInstance, CLuaCallbackClient*, (std::string& protoc))
    {
        r = new CLuaCallbackClient(protoc);
        r->GetScriptObject().SetDelByScr(true);
        return 1;
    }

    END_SCRIPT_CLASS()


}


