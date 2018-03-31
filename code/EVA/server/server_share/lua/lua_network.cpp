#include "lua_network.h"
#include <nel/net/callback_server_websocket.h>
#include <server_share/server_def.h>
#include "script_mgr.h"
#include "server_share/bin_luabind/Public.hpp"

using namespace NLMISC;
using namespace NLNET;

void cbLuaServiceMsg ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
    uint64          msg_from = serviceId.get();
    std::string     msg_type;
    std::string     msg_buff;

    msgin.serial(msg_type);
    msgin.serial(msg_buff);

    LuaParams lua_params( msg_from, msg_type, msg_buff );  
    ScriptMgr.run( "NetWorkHandler", "OnMessage", lua_params);
}

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

const TCallbackItem LuaClientCallbackArray[] =
{
    { "_LC",                         cbLuaClientMsg          },
};

NLNET::TUnifiedCallbackItem LuaCallbackArray[] =
{
    { "_LS",                         cbLuaServiceMsg         },
};

static void cbWSConnect( TSockId from, void *arg );
static void cbWSDisConnect( TSockId from, void *arg );

class CLuaWebSocketNetwork
{
    DECLARE_SCRIPT_CLASS();
public:

    CLuaWebSocketNetwork( std::string name, uint16 port )
    {
        m_NetName = name;
        m_CallbackServerHandle = new CCallbackServerWebSocket();
        m_CallbackServerHandle->addCallbackArray(LuaClientCallbackArray, sizeof(LuaClientCallbackArray)/sizeof(LuaClientCallbackArray[0]));

        m_CallbackServerHandle->setConnectionCallback( cbWSConnect, this );
        m_CallbackServerHandle->setDisconnectionCallback( cbWSDisConnect, this );

        m_CallbackServerHandle->init (port);
        
        LuaNetworkMgr.RegisterNetModule( name, this );
    }

    ~CLuaWebSocketNetwork()
    {
        LuaNetworkMgr.RemoveNetModule(m_NetName);
        delete m_CallbackServerHandle; 
    }

    void Update()
    {
        if( m_CallbackServerHandle->connected() )
        {
            m_CallbackServerHandle->update();
        }
    }

    void Send( TSockId sock_id, std::string& msg_type, std::string& proto_str )
    {
        CMemStream mem_out;

        //uint32 buff_len = proto_str.size() + msg_type.size() + 1 + 4;
        //mem_out.serial(buff_len);
        uint8 msg_type_len = msg_type.size();
        mem_out.serial(msg_type_len);
        mem_out.serialBuffer( (uint8*)msg_type.c_str(), msg_type_len );
        mem_out.serialBuffer( (uint8*)proto_str.c_str(), proto_str.size() );

        m_CallbackServerHandle->send_buffer( mem_out, sock_id );
    }

    std::string         GetName()   { return m_NetName; }
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

    if ( iter == m_LuaWebSocketNetworkHandle.end() )
    {
        m_LuaWebSocketNetworkHandle.insert( make_pair(name, pNet) ); 
    }
    else
    {
        nlstop;
    }
}

void CLuaNetworkMgr::RemoveNetModule( std::string name )
{
    m_LuaWebSocketNetworkHandle.erase(name);
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

static void cbWSConnect( TSockId from, void *arg )
{
    CLuaWebSocketNetwork* pLuaNetwork = (CLuaWebSocketNetwork*)arg;

    std::string lua_event = pLuaNetwork->GetName();
    lua_event.append("Connection");

    uint64          msg_from = (uint64)from;
    std::string     msg_buff;


    LuaParams lua_params( msg_from, lua_event, msg_buff );  
    ScriptMgr.run( "NetWorkHandler", "OnMessage", lua_params );
}

static void cbWSDisConnect( TSockId from, void *arg )
{
    CLuaWebSocketNetwork* pLuaNetwork = (CLuaWebSocketNetwork*)arg;

    std::string lua_event = pLuaNetwork->GetName();
    lua_event.append("DisConnection");

    uint64          msg_from = (uint64)from;
    std::string     msg_buff;


    LuaParams lua_params( msg_from, lua_event, msg_buff );  
    ScriptMgr.run( "NetWorkHandler", "OnMessage", lua_params );
}

void cbConnection( const std::string &serviceName, NLNET::TServiceId sid, void *arg )
{
    std::string lua_event = serviceName;
    lua_event.append("Connection");
    LuaParams lua_params( (uint64)sid.get(), lua_event, serviceName );  
    ScriptMgr.run( "NetWorkHandler", "OnMessage", lua_params );
}

void cbDisconnection( const std::string &serviceName, NLNET::TServiceId sid, void *arg )
{
    std::string lua_event = serviceName;
    lua_event.append("DisConnection");
    LuaParams lua_params( (uint64)sid.get(), lua_event, serviceName );  
    ScriptMgr.run( "NetWorkHandler", "OnMessage", lua_params );
}

namespace bin
{
    BEGIN_SCRIPT_CLASS( WebSocketNetwork, CLuaWebSocketNetwork )

        DEFINE_CLASS_FUNCTION( Send, void, (const char* proto_buf, CScriptTable& tb_msg))
        {
            if( tb_msg.IsReferd() )
            {
                lua_Integer     sock_id;
                std::string     msg_type;
                int             buf_len;

                tb_msg.Get(1, sock_id);
                tb_msg.Get(2, msg_type);
                tb_msg.Get(3, buf_len);

                std::string     pb_str(proto_buf,buf_len);
                obj->Send( (TSockId)sock_id, msg_type, pb_str );
            }

            return 1;
        }

        DEFINE_STATIC_FUNCTION(NewInstance, CLuaWebSocketNetwork*, (std::string name, int port))
        {
            r = new CLuaWebSocketNetwork(name, port);
            r->GetScriptObject().SetDelByScr(true);

            return 1;
        }

    END_SCRIPT_CLASS()


    ///   
    BEGIN_SCRIPT_MODULE(ServerNet)

        DEFINE_MODULE_FUNCTION(Broadcast, void, (const char* proto_buf, CScriptTable& tb_msg))
        {
            if( tb_msg.IsReferd() )
            {
                std::string     service_name;
                std::string     msg_type;
                int             buf_len;

                tb_msg.Get(1, service_name);
                tb_msg.Get(2, msg_type);
                tb_msg.Get(3, buf_len);

                std::string      pb_str(proto_buf,buf_len);

                CMessage msg_out("_LS");
                msg_out.serial(msg_type);
                msg_out.serial(pb_str);

                Network->send( service_name, msg_out, false );
            }

            return 1;
        }

        DEFINE_MODULE_FUNCTION(Send, void, (const char* proto_buf, CScriptTable& tb_msg))
        {
            if( tb_msg.IsReferd() )
            {
                int             sid;
                std::string     msg_type;
                int             buf_len;

                tb_msg.Get(1, sid);
                tb_msg.Get(2, msg_type);
                tb_msg.Get(3, buf_len);

                std::string     pb_str(proto_buf,buf_len);

                CMessage msg_out("_LS");
                msg_out.serial(msg_type);
                msg_out.serial(pb_str);

                Network->send( (NLNET::TServiceId)sid, msg_out );
            }

            return 1;
        }

        DEFINE_MODULE_FUNCTION(SetConnectionCallback, void, (std::string service_name))
        {
            CUnifiedNetwork::getInstance()->setServiceUpCallback(service_name, cbConnection);
            return 1;
        }

        DEFINE_MODULE_FUNCTION(SetDisConnectionCallback, void, (std::string service_name))
        {
            CUnifiedNetwork::getInstance()->setServiceDownCallback(service_name, cbDisconnection);
            return 1;
        }



    END_SCRIPT_MODULE()


}





