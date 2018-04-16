#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <map>
#include <utility>
#include <server_share/tools.h>
#include <server_share/server_def.h>
#include <server_share/client_msg_desc.h>
#include <server_share/lua/script_mgr.h>
#include <server_share/lua_net/lua_network.h>
#include <nel/misc/window_displayer.h>
#include <nel/net/naming_client.h>

#include "frontend_service.h"
#include <server_share/timer.h>
#include <server_share/lua/lua_thread.h>

#ifdef NL_OS_WINDOWS
#include <Windows.h>
#endif

using namespace std;
using namespace NLMISC;
using namespace NLNET;
using namespace DEF;

extern void admin_modules_forceLink();
void foo()  {   admin_modules_forceLink();  }

NLMISC::CVariable<uint32>	VAR_PLAYER_NUM("fes", "NbPlayers"  , "memo", 0);

void CFrontEndService::init()
{
    LocalTime.SetCurrTime( CTime::getLocalTime() );
    AcceptClients  = false;

    //string fn = IService::getInstance()->SaveFilesDirectory.toString();
    //fn += ConfigFile.getVar("LogDir").asString();
    //fn += "frontend_service.stat";
    //nlinfo("Frontend stat in directory '%s'", fn.c_str());
    //NLMISC::CFileDisplayer *Fd = new NLMISC::CFileDisplayer(fn);
    //Loger().addDisplayer(Fd);
    //if (WindowDisplayer) Loger().addDisplayer (WindowDisplayer);

    ///////////////////////////////////////////////////

    TimerManager->init();
    LuaThreadMgr.Init();
    LuaNetworkMgr.Init();
    ScriptMgr.init();

    //////////////////////////////////////////////////
    // TCP 相关
    //m_Clients = new NLNET::CCallbackServer();
    //nlassert (m_Clients != NULL);
    //m_Clients->addCallbackArray (ClientCallbackArray, sizeof(ClientCallbackArray)/sizeof(ClientCallbackArray[0]));
    //m_Clients->setDefaultCallback(TCPForwarding);
    //m_Clients->setConnectionCallback ( onConnectionClient, NULL );
    //m_Clients->setDisconnectionCallback ( onDisconnectClient, NULL );
    //m_Clients->setMaxExpectedBlockSize(MaxUDPPacketSize);
    //m_Clients->init (_inetPort);

    /////////////////////////////////////////////////

    //CUnifiedNetwork::getInstance()->setServiceUpCallback("LS", cbLSConnection);
    //CUnifiedNetwork::getInstance()->setServiceDownCallback(LogicService, cbPLSDisconnection);
    //CUnifiedNetwork::getInstance()->setServiceDownCallback(EntitiesServer, cbEGSDisconnection);

    //////////////////////////////////////////////////////////////////////////

    AcceptClients = true;
}

bool CFrontEndService::update()
{
    //AcceptClients = CNamingClient::IsCanAccess( _WaitServiceAccess );

    _CurrTicks = CTime::getLocalTime();
    LocalTime.SetCurrTime(_CurrTicks);

    ///////////////////////////////////////

    TimerManager->tickUpdate();
    ScriptMgr.update();
    LuaNetworkMgr.Update();
    LuaThreadMgr.Update();

    return true;
}

void CFrontEndService::release()
{
    TimerManager->release();
    ScriptMgr.release();
    LuaNetworkMgr.Release();
    LuaThreadMgr.Release();

    google::protobuf::ShutdownProtobufLibrary();
}

void CFrontEndService::IncReceiveMsgCount( std::string msg_name )
{
    TReceiveMsgCount::iterator iter = m_ReceiveMsgCount.find(msg_name);

    if ( iter!=m_ReceiveMsgCount.end() )
    {
        iter->second += 1;
    }
    else
    {
        m_ReceiveMsgCount.insert( make_pair(msg_name, 1) );
    }
}

/****************************************************************************   
 * FRONTEND SERVICE MAIN Function
 *
 * This call create a main function for a service:
 *
 *    - based on the "CFrontEndService" class
 *    - having the short name "FES"
 *    - having the long name "frontend_service"
 *    - listening on the port "0" (dynamically determined)
 *    - and shard callback set to "CallbackArray"
 *
 ****************************************************************************/
NLNET_SERVICE_MAIN (CFrontEndService, "FES", "frontend_service", 0, EmptyCallbackArray, "", "")

NLMISC_COMMAND (info, "service information.", "")
{
    if(args.size() != 0) return false;

    log.displayNL ("ClientNumber:%u", VAR_PLAYER_NUM.get());
    
    if ( FrontEndService->AcceptClients )
    {
        log.displayNL ("AcceptClients:true");
    }
    else
    {
        log.displayNL ("AcceptClients:false");
    }

    return true;
}

NLMISC_COMMAND (topmsg, "", "")
{
    if(args.size() != 0) return false;

    CFrontEndService::TReceiveMsgCount::iterator iter,it_end;
    iter    = FrontEndService->m_ReceiveMsgCount.begin();
    it_end  = FrontEndService->m_ReceiveMsgCount.end();

    std::multimap<uint64, std::string>  sortmap;

    while ( iter!=it_end )
    {
        sortmap.insert( make_pair( iter->second, iter->first ) );
        ++iter;
    }

    std::multimap<uint64, std::string>::reverse_iterator riter,rit_end;
    riter    = sortmap.rbegin();
    rit_end  = sortmap.rend();

    while ( riter!=rit_end )
    {
        log.displayNL("Count: %"NL_I64"u       Msg: %s", riter->first, riter->second.c_str() );
        ++riter;
    }

    return true;
}

NLMISC_COMMAND (loadmsg, "", "")
{
    if(args.size() != 0) return false;

    MsgDesc.LoadMsgXml();
    log.displayNL ("ReLoadMsgXml Done.");

    return true;
}

/* end of file */
