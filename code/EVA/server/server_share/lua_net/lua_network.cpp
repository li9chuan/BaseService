#include "lua_network.h"
#include "lua_message.h"
#include "lua_callback_server.h"
#include <server_share/lua/script_mgr.h>
#include "server_share/bin_luabind/Public.hpp"

using namespace NLMISC;
using namespace NLNET;

static CLuaMessage* pUnifiedServiceMsg = new CLuaMessage();

void cbLuaUnifiedServiceMsg ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
    uint32 subSize;
    msgin.serial(subSize);
    msgin.lockSubMessage(subSize);

    pUnifiedServiceMsg->m_Msg.clear();
    pUnifiedServiceMsg->m_Msg.assignFromSubMessage(msgin);

    msgin.unlockSubMessage();


    bin::CScriptTable    functbl;
    ScriptMgr.GetScriptHandle()->Get("NetWorkHandler", functbl);

    int nRet = 0;
    functbl.CallFunc<lua_Integer, CLuaMessage*, int>("OnMessage", (lua_Integer)serviceId.get(), pUnifiedServiceMsg, nRet);
}



///  转发到客户端
void cbLuaSendToClientMsg ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
    DEF::UID        client_uid;
    msgin.serial(client_uid);

    uint32 subSize;
    msgin.serial(subSize);
    msgin.lockSubMessage(subSize);

    pUnifiedServiceMsg->m_Msg.clear();
    pUnifiedServiceMsg->m_Msg.assignFromSubMessage(msgin);

    msgin.unlockSubMessage();

    LuaNetworkMgr.SendToClient( client_uid, pUnifiedServiceMsg->m_Msg );
}

NLNET::TUnifiedCallbackItem LuaCallbackArray[] =
{
    { "_LS",                            cbLuaUnifiedServiceMsg          },
    { "_LSC",                           cbLuaSendToClientMsg            },
};

void CLuaNetworkMgr::Init()
{
    NLNET::CUnifiedNetwork::getInstance()->addCallbackArray(LuaCallbackArray, sizeof(LuaCallbackArray)/sizeof(LuaCallbackArray[0]));
}

void CLuaNetworkMgr::RegisterNetModule( std::string name, CLuaCallbackServer* pNet )
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

void CLuaNetworkMgr::SendToClient( uint64 uid, NLNET::CMessage& msgin )
{
    TNetHandle::iterator iter = m_LuaClientNetworkHandle.begin();

    while ( iter != m_LuaClientNetworkHandle.end() )
    {
        TSockId sock_id = iter->second->GetSockId(uid);

        if ( sock_id != InvalidSockId )
        {
            iter->second->Send( sock_id, msgin );
        }

        ++iter;
    }
}

void cbLuaSvrConnect( TSockId from, void *arg )
{
    CLuaCallbackServer* pLuaNetwork = (CLuaCallbackServer*)arg;

    std::string lua_event = pLuaNetwork->GetName();
    lua_event.append("Con");

    uint64          msg_from = (uint64)from;
    std::string     msg_buff;

    LuaParams lua_params( msg_from, lua_event, msg_buff );  
    ScriptMgr.run( "NetWorkHandler", "OnNetEvent", lua_params );
}

void cbLuaSvrDisConnect( TSockId from, void *arg )
{
    CLuaCallbackServer* pLuaNetwork = (CLuaCallbackServer*)arg;

    std::string lua_event = pLuaNetwork->GetName();
    lua_event.append("Dis");

    uint64          msg_from = (uint64)from;
    std::string     msg_buff;


    LuaParams lua_params( msg_from, lua_event, msg_buff );  
    ScriptMgr.run( "NetWorkHandler", "OnNetEvent", lua_params );
}

void cbConnection( const std::string &serviceName, NLNET::TServiceId sid, void *arg )
{
    std::string lua_event = serviceName;
    lua_event.append("Con");
    LuaParams lua_params( (uint64)sid.get(), lua_event, serviceName );  
    ScriptMgr.run( "NetWorkHandler", "OnNetEvent", lua_params );
}

void cbDisconnection( const std::string &serviceName, NLNET::TServiceId sid, void *arg )
{
    std::string lua_event = serviceName;
    lua_event.append("Dis");
    LuaParams lua_params( (uint64)sid.get(), lua_event, serviceName );  
    ScriptMgr.run( "NetWorkHandler", "OnNetEvent", lua_params );
}

namespace bin
{
    BEGIN_SCRIPT_CLASS( LuaCallbackServer, CLuaCallbackServer )

        DEFINE_CLASS_FUNCTION( Send, void, (lua_Integer sock_id, CLuaMessage* lua_msg))
        {
            obj->Send( (TSockId)sock_id, lua_msg->m_Msg );
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

        DEFINE_STATIC_FUNCTION(NewInstance, CLuaCallbackServer*, (CScriptTable& svr_info))
        {
            if( svr_info.IsReferd() )
            {
                std::string     svr_name;
                std::string     svr_protoc;
                lua_Integer     svr_port;

                svr_info.Get(1, svr_name);
                svr_info.Get(2, svr_protoc);
                svr_info.Get(3, svr_port);

                r = new CLuaCallbackServer(svr_name, svr_protoc, svr_port);
                r->GetScriptObject().SetDelByScr(true);
            }

            return 1;
        }

    END_SCRIPT_CLASS()


    ///   
    BEGIN_SCRIPT_MODULE(ServerNet)

        DEFINE_MODULE_FUNCTION(Broadcast, void, (const char* service_name, CLuaMessage* pMsg))
        {
            NLNET::CMessage msgout("_LS");
            msgout.serialMessage(pMsg->m_Msg);
            Network->send( service_name, msgout, false );
            return 1;
        }

        DEFINE_MODULE_FUNCTION(Send, void, (lua_Integer service_id, CLuaMessage* pMsg))
        {
            NLNET::CMessage msgout("_LS");
            msgout.serialMessage(pMsg->m_Msg);
            Network->send( (NLNET::TServiceId)service_id, msgout );
            return 1;
        }

        DEFINE_MODULE_FUNCTION(SendToClient, void, (CLuaMessage* pMsg, CScriptTable& tb_msg))
        {
            if( tb_msg.IsReferd() )
            {
                int             sid;
                lua_Integer     client_uid;

                tb_msg.Get(1, sid);
                tb_msg.Get(2, client_uid);

                CMessage msg_out("_LSC");
                msg_out.serial(client_uid);
                msg_out.serialMessage(pMsg->m_Msg);

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





