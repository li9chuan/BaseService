#include "lua_network.h"
#include "lua_message.h"
#include "lua_callback_server.h"
#include <server_share/lua/script_mgr.h>
#include "server_share/bin_luabind/Public.hpp"
#include "curl/curl.h"

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

    sint32 nRet = 0;
    sint32 _sid = serviceId.get();
    functbl.CallFunc<sint32, CLuaMessage*, sint32>("OnMessage", _sid, pUnifiedServiceMsg, nRet);
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

static size_t WriteDataFromCurl(char* buffer, size_t size, size_t nmemb, NLMISC::CSString* pWriterData)
{
    if (pWriterData == NULL)
        return 0;

    pWriterData->append(buffer, size*nmemb);
    return size * nmemb;
}

std::string HttpPost(std::string& url, std::string& params)
{
    std::string m_WriterData;

    try
    {
        CURL* pCurl = curl_easy_init();

        if (pCurl != NULL)
        {
            curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);

            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteDataFromCurl);
            curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &m_WriterData);

            /* Now specify the POST data */
            curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, params.c_str());

            /* Perform the request, res will get the return code */
            CURLcode res = curl_easy_perform(pCurl);

            curl_easy_cleanup(pCurl);
            /* Check for errors */
            if (res != CURLE_OK)
            {
                nlwarning("CLuaHttpRequest Post() failed: %s\n", curl_easy_strerror(res));
            }
        }
    }
    catch (...)
    {

    }

    return m_WriterData;
}

std::string HttpGet(std::string& url)
{
    std::string m_WriterData;

    try
    {
        CURL* pCurl = curl_easy_init();

        if (pCurl != NULL)
        {
            curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);

            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteDataFromCurl);
            curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &m_WriterData);

            /* Perform the request, res will get the return code */
            CURLcode res = curl_easy_perform(pCurl);

            curl_easy_cleanup(pCurl);
            /* Check for errors */
            if (res != CURLE_OK)
            {
                nlwarning("CLuaHttpRequest Get() failed: %s\n", curl_easy_strerror(res));
            }
        }
    }
    catch (...)
    {

    }

    return m_WriterData;
}

namespace bin
{
    BEGIN_SCRIPT_CLASS( LuaCallbackServer, CLuaCallbackServer )

        DEFINE_CLASS_FUNCTION( Send, void, (sint64 sock_id, CLuaMessage* lua_msg))
        {
            obj->Send( (TSockId)sock_id, lua_msg->m_Msg );
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetClientData, void, (CScriptTable& uid_data))
        {
            if( uid_data.IsReferd() )
            {
                ClientData      cdata;
                sint64          int64_val;

                uid_data.Get(1, int64_val);         cdata.uid   = int64_val;
                uid_data.Get(2, int64_val);         cdata.sid   = (NLNET::TSockId)int64_val;
                uid_data.Get(3, int64_val);         cdata.pls_sid.set(int64_val);

                obj->SetClientData( cdata );
            }
            return 1;
        }

        DEFINE_CLASS_FUNCTION( DisConnect, void, (sint64 sock_id) )
        {
            obj->DisConnect((NLNET::TSockId)sock_id);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( RemoveClientData, void, (sint64 uid))
        {
            obj->RemoveClientData( uid );
            return 1;
        }

        DEFINE_CLASS_FUNCTION( ClearClientData, void, ())
        {
            obj->ClearClientData();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( Listen, void, (sint32 port))
        {
            obj->Listen(port);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( LoadSslCA, void, (std::string& ssl_ca))
        {
            obj->LoadSslCA(ssl_ca);
            return 1;
        }
        DEFINE_CLASS_FUNCTION( LoadSslCrt, void, (std::string& ssl_crt))
        {
            obj->LoadSslCrt(ssl_crt);
            return 1;
        }
        DEFINE_CLASS_FUNCTION( LoadSslPrivateKey, void, (std::string& ssl_pk))
        {
            obj->LoadSslPrivateKey(ssl_pk);
            return 1;
        }

        DEFINE_STATIC_FUNCTION(NewInstance, CLuaCallbackServer*, (std::string& svr_name, std::string& svr_protoc))
        {
            r = new CLuaCallbackServer(svr_name, svr_protoc);
            r->GetScriptObject().SetDelByScr(true);
            return 1;
        }

    END_SCRIPT_CLASS()


    ///   
    BEGIN_SCRIPT_MODULE(Net)

        DEFINE_MODULE_FUNCTION(Broadcast, void, (const char* service_name, CLuaMessage* pMsg))
        {
            if( pMsg != NULL )
            {
                NLNET::CMessage msgout("_LS");
                msgout.serialMessage(pMsg->m_Msg);
                Network->send(service_name, msgout, false);
            }
            else
            {
                lua_Debug ar;
                lua_getstack(lua.GetHandle(), 2, &ar);
                lua_getinfo(lua.GetHandle(), "Sln", &ar);

                NLMISC::createDebug();
                NLMISC::INelContext::getInstance().getWarningLog()->setPosition(ar.currentline, ar.short_src, ar.name);
                NLMISC::INelContext::getInstance().getWarningLog()->displayNL("Broadcast %s  msg is NULL.", service_name);
            }
            return 1;
        }

        DEFINE_MODULE_FUNCTION(Send, void, (sint32 service_id, CLuaMessage* pMsg))
        {
            if (pMsg != NULL)
            {
                NLNET::CMessage msgout("_LS");
                msgout.serialMessage(pMsg->m_Msg);
                Network->send( (NLNET::TServiceId)service_id, msgout );
            }
            else
            {
                lua_Debug ar;
                lua_getstack(lua.GetHandle(), 2, &ar);
                lua_getinfo(lua.GetHandle(), "Sln", &ar);

                NLMISC::createDebug();
                NLMISC::INelContext::getInstance().getWarningLog()->setPosition(ar.currentline, ar.short_src, ar.name);
                NLMISC::INelContext::getInstance().getWarningLog()->displayNL("Send msg is NULL.");
            }
            return 1;
        }

        DEFINE_MODULE_FUNCTION(SendToClient, void, (CLuaMessage* pMsg, CScriptTable& tb_msg))
        {
            if( tb_msg.IsReferd() )
            {
                if (pMsg != NULL)
                {
                    int             sid;
                    sint64          client_uid;

                    tb_msg.Get(1, sid);
                    tb_msg.Get(2, client_uid);

                    CMessage msg_out("_LSC");
                    msg_out.serial(client_uid);
                    msg_out.serialMessage(pMsg->m_Msg);

                    Network->send((NLNET::TServiceId)sid, msg_out);
                }
                else
                {
                    lua_Debug ar;
                    lua_getstack(lua.GetHandle(), 2, &ar);
                    lua_getinfo(lua.GetHandle(), "Sln", &ar);

                    NLMISC::createDebug();
                    NLMISC::INelContext::getInstance().getWarningLog()->setPosition(ar.currentline, ar.short_src, ar.name);
                    NLMISC::INelContext::getInstance().getWarningLog()->displayNL("SendToClient msg is NULL.");
                }
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

        DEFINE_MODULE_FUNCTION(HttpPost, std::string, (std::string& url, std::string& params))
        {
            r = HttpPost(url, params);
            return 1;
        }

        DEFINE_MODULE_FUNCTION(HttpGet, std::string, (std::string& url))
        {
            r = HttpGet(url);
            return 1;
        }


    END_SCRIPT_MODULE()


}





