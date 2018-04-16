#include "lua_network.h"
#include "lua_message.h"
#include "lua_network_base.h"
#include "lua_network_tcp.h"
#include "lua_network_websocket.h"
#include <server_share/lua/script_mgr.h>
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



///  转发到客户端
void cbLuaSendToClientMsg ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
    DEF::UID        client_uid;
    std::string     msg_type;
    std::string     pb_buff;

    msgin.serial(client_uid);
    msgin.serial(msg_type);
    msgin.serial(pb_buff);

    LuaNetworkMgr.SendToClient( client_uid, msg_type, pb_buff );
}



NLNET::TUnifiedCallbackItem LuaCallbackArray[] =
{
    { "_LS",                            cbLuaServiceMsg             },
    { "_LSC",                           cbLuaSendToClientMsg        },
};

void CLuaNetworkMgr::Init()
{
    NLNET::CUnifiedNetwork::getInstance()->addCallbackArray(LuaCallbackArray, sizeof(LuaCallbackArray)/sizeof(LuaCallbackArray[0]));
}

void CLuaNetworkMgr::RegisterNetModule( std::string name, CLuaBaseNetwork* pNet )
{
    TNetHandle::iterator iter = m_LuaClientNetworkHandle.find(name);

    if ( iter == m_LuaClientNetworkHandle.end() )
    {
        m_LuaClientNetworkHandle.insert( make_pair(name, pNet) ); 
    }
    else
    {
        nlstop;
    }
}

void CLuaNetworkMgr::RemoveNetModule( std::string name )
{
    m_LuaClientNetworkHandle.erase(name);
}

void CLuaNetworkMgr::Update()
{
    TNetHandle::iterator iter = m_LuaClientNetworkHandle.begin();

    while (iter!=m_LuaClientNetworkHandle.end())
    {

        iter->second->Update();
        ++iter;
    }
}

void CLuaNetworkMgr::Release()
{
    TNetHandle::iterator iter = m_LuaClientNetworkHandle.begin();

    while (iter!=m_LuaClientNetworkHandle.end())
    {

        delete iter->second;
        ++iter;
    }

    m_LuaClientNetworkHandle.clear();
}

void CLuaNetworkMgr::SendToClient( uint64 uid, std::string& msg_type, std::string& pb_buff )
{
    TNetHandle::iterator iter = m_LuaClientNetworkHandle.begin();

    if ( iter != m_LuaClientNetworkHandle.end() )
    {
        TSockId sock_id = iter->second->GetSockId(uid);

        if ( sock_id != InvalidSockId )
        {
            iter->second->Send( sock_id, msg_type, pb_buff );
        }
    }
}

void cbLuaSvrConnect( TSockId from, void *arg )
{
    CLuaBaseNetwork* pLuaNetwork = (CLuaBaseNetwork*)arg;

    std::string lua_event = pLuaNetwork->GetName();
    lua_event.append("Connection");

    uint64          msg_from = (uint64)from;
    std::string     msg_buff;


    LuaParams lua_params( msg_from, lua_event, msg_buff );  
    ScriptMgr.run( "NetWorkHandler", "OnMessage", lua_params );
}

void cbLuaSvrDisConnect( TSockId from, void *arg )
{
    CLuaBaseNetwork* pLuaNetwork = (CLuaBaseNetwork*)arg;

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
    BEGIN_SCRIPT_CLASS( NetworkWebSocket, CLuaNetworkWebSocket )

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

        DEFINE_CLASS_FUNCTION( SetUIDMap, void, (lua_Integer uid, lua_Integer sock_id))
        {
            obj->SetUIDMap( uid, (TSockId)sock_id );
            return 1;
        }

        DEFINE_CLASS_FUNCTION( RemoveUIDMap, void, (lua_Integer uid))
        {
            obj->RemoveUIDMap( uid );
            return 1;
        }

        DEFINE_CLASS_FUNCTION( ClearUIDMap, void, ())
        {
            obj->ClearUIDMap();
            return 1;
        }

        DEFINE_STATIC_FUNCTION(NewInstance, CLuaNetworkWebSocket*, (std::string name, int port))
        {
            r = new CLuaNetworkWebSocket(name, port);
            r->GetScriptObject().SetDelByScr(true);

            return 1;
        }

    END_SCRIPT_CLASS()



    BEGIN_SCRIPT_CLASS( NetworkTcp, CLuaNetworkTcp )

    DEFINE_CLASS_FUNCTION( Send, void, (lua_Integer sock_id, CLuaMessage* lua_msg))
    {
        obj->Send( lua_msg->m_Msg, (TSockId)sock_id );
        return 1;
    }

    DEFINE_CLASS_FUNCTION( SetUIDMap, void, (lua_Integer uid, lua_Integer sock_id))
    {
        obj->SetUIDMap( uid, (TSockId)sock_id );
        return 1;
    }

    DEFINE_CLASS_FUNCTION( RemoveUIDMap, void, (lua_Integer uid))
    {
        obj->RemoveUIDMap( uid );
        return 1;
    }

    DEFINE_CLASS_FUNCTION( ClearUIDMap, void, ())
    {
        obj->ClearUIDMap();
        return 1;
    }

    DEFINE_STATIC_FUNCTION(NewInstance, CLuaNetworkTcp*, (std::string name, int port))
    {
        r = new CLuaNetworkTcp(name, port);
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

        DEFINE_MODULE_FUNCTION(SendToClient, void, (const char* proto_buf, CScriptTable& tb_msg))
        {
            if( tb_msg.IsReferd() )
            {
                int             sid;
                std::string     msg_type;
                int             buf_len;
                lua_Integer     client_uid;

                tb_msg.Get(1, sid);
                tb_msg.Get(2, msg_type);
                tb_msg.Get(3, buf_len);
                tb_msg.Get(4, client_uid);

                std::string     pb_str(proto_buf, buf_len);

                CMessage msg_out("_LSC");
                msg_out.serial(client_uid);
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

        DEFINE_MODULE_FUNCTION(GetServiceID, int, ())
        {
            r = NLNET::IService::getInstance()->getServiceId().get();
            return 1;
        }

        DEFINE_MODULE_FUNCTION(GetServiceName, std::string, ())
        {
            r = NLNET::IService::getInstance()->getServiceShortName();;
            return 1;
        }




    END_SCRIPT_MODULE()


}





