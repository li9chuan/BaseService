#include "lua_callback_client.h"
#include "lua_message.h"
#include <server_share/lua/script_mgr.h>
#include <nel/net/buf_client.h>
#include <nel/net/callback_client.h>

using namespace bin;
using namespace NLNET;

void forLuaCallbackClientForceLink()
{
    nlwarning("forLuaCallbackClientForceLink");
}

static CLuaMessage* pLuaMsg = new CLuaMessage();
    
void cbLuaClientMsg(CMessage &msgin, TSockId from, CCallbackNetBase &netbase)
{
    CLuaCallbackClient* pClient = (CLuaCallbackClient*)netbase.getUserData();

    CScriptTable    functbl;
    pClient->GetScriptHandle().Get("NetWorkHandler", functbl);

    int nRet = 0;
    pLuaMsg->m_Msg.swap(msgin);

    functbl.CallFunc<lua_Integer, CLuaMessage*, int>("OnMessage", (lua_Integer)pClient->GetHandle(), pLuaMsg, nRet);
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
}

CLuaCallbackClient::~CLuaCallbackClient()
{
    delete m_CallbackClientHandle;
}

void CLuaCallbackClient::Connect(std::string & url)
{
    CInetAddress addr(url);
    CCallbackClient* pClient = (CCallbackClient*)m_CallbackClientHandle;
    pClient->connect(addr);
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

    DEFINE_CLASS_FUNCTION(SetHandle, void, (lua_Integer client_handle))
    {
        obj->SetHandle(client_handle);
        return 1;
    }

    DEFINE_CLASS_FUNCTION(GetHandle, lua_Integer, ())
    {
        r = obj->GetHandle();
        return 1;
    }

    DEFINE_CLASS_FUNCTION(Connected, bool, ())
    {
        r = obj->Connected();
        return 1;
    }

    DEFINE_CLASS_FUNCTION(Update, void, ())
    {
        obj->Update();
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


