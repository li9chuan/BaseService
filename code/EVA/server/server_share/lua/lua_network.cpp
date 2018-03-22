#include "lua_network.h"
#include <nel/net/callback_server_websocket.h>
#include <server_share/server_def.h>
#include "script_mgr.h"
#include "server_share/bin_luabind/Public.hpp"

using namespace NLMISC;
using namespace NLNET;

void cbLuaMsg ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
    uint64          msg_from = serviceId.get();
    std::string     msg_type;
    std::string     msg_body;

    msgin.serial(msg_from);
    msgin.serial(msg_type);
    msgin.serial(msg_body);

    LuaParams lua_params( msg_from, msg_type, msg_body );  
    ScriptMgr.run( "NetWorkHandler", "OnMessage", lua_params);
}

NLNET::TUnifiedCallbackItem LuaCallbackArray[] =
{
    { "_L",                        cbLuaMsg              },
};

class CLuaWebSocketNetwork
{
    DECLARE_SCRIPT_CLASS();
public:

    void Hello()
    {
        nldebug("CLuaWebSocketNetwork::Hello");


        //ScriptMgr.on_event()    
        //NetWorkHandler.OnMessage( data )

    }

    CLuaWebSocketNetwork( std::string name, uint16 port )
    {
        m_NetName = name;
        CCallbackServerWebSocket* pCallbackServerHandle = new CCallbackServerWebSocket();
        pCallbackServerHandle->init (port);

    }

    ~CLuaWebSocketNetwork()
    {
        //delete m_CallbackServerHandle; 
    }

    void Update()
    {
        m_CallbackServerHandle->update();
    }

    void Say(const std::string& msg)
    {
        printf(msg.c_str());
    }

    std::string Want()
    {
        return "World";
    }

private:

    std::string                         m_NetName;
    CCallbackServerWebSocket*           m_CallbackServerHandle;
};


void CLuaNetworkMgr::Init()
{
    NLNET::CUnifiedNetwork::getInstance()->addCallbackArray(LuaCallbackArray, sizeof(LuaCallbackArray)/sizeof(LuaCallbackArray[0]));
}

void CLuaNetworkMgr::RegisterNetModule( std::string name, CLuaWebSocketNetwork* pNet )
{
    TNetHandle::iterator iter = m_LuaWebSocketNetworkHandle.find(name);

    if ( iter != m_LuaWebSocketNetworkHandle.end() )
    {
        m_LuaWebSocketNetworkHandle.insert( make_pair(name,pNet) ); 
    }
    else
    {
        nlstop;
    }
}

void CLuaNetworkMgr::Update()
{
    TNetHandle::iterator iter = m_LuaWebSocketNetworkHandle.begin();

    while (iter!=m_LuaWebSocketNetworkHandle.end())
    {

        iter->second->Update();
        ++iter;
    }
}

void CLuaNetworkMgr::Release()
{
    TNetHandle::iterator iter = m_LuaWebSocketNetworkHandle.begin();

    while (iter!=m_LuaWebSocketNetworkHandle.end())
    {

        delete iter->second;
        ++iter;
    }

    m_LuaWebSocketNetworkHandle.clear();
}





namespace bin
{
    BEGIN_SCRIPT_CLASS( WebSocketNetwork, CLuaWebSocketNetwork )
        DEFINE_CLASS_FUNCTION(hello, void, ())
    {
        obj->Hello();
        return 1;
    }

    DEFINE_CLASS_FUNCTION(say, void, (const std::string& message))
    {
        obj->Say(message);
        return 1;
    }

    DEFINE_CLASS_FUNCTION(want , std::string, ())
    {
        r = obj->Want();
        return 1;
    }

    DEFINE_STATIC_FUNCTION(newInstance, CLuaWebSocketNetwork*, (std::string name, int port))
    {
        r = new CLuaWebSocketNetwork(name, port);
        r->GetScriptObject().SetDelByScr(true);

        return 1;
    }
    END_SCRIPT_CLASS()


    ///   
    BEGIN_SCRIPT_MODULE(ServerNet)

    DEFINE_MODULE_FUNCTION(Broadcast, void, (std::string service_name, CScriptTable& tb_msg))
    {
        if( tb_msg.IsReferd() )
        {
            std::string  msg_name;
            std::string  msg_body;

            tb_msg.Get(1, msg_name);
            tb_msg.Get(2, msg_body);

            CMessage msg_out("_L");
            msg_out.serial(msg_name);
            msg_out.serial(msg_body);

            Network->send( service_name, msg_out );
        }

        return 1;
    }

    DEFINE_MODULE_FUNCTION(Send, void, (int sid, CScriptTable& tb_msg))
    {
        if( tb_msg.IsReferd() )
        {
            std::string  msg_name;
            std::string  msg_body;

            tb_msg.Get(1, msg_name);
            tb_msg.Get(2, msg_body);

            CMessage msg_out("_L");
            msg_out.serial(msg_name);
            msg_out.serial(msg_body);

            Network->send( (NLNET::TServiceId)sid, msg_out );
        }

        return 1;
    }

    END_SCRIPT_MODULE()
}





