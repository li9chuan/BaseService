#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <server_share/server_def.h>
#include <server_share/client_msg_desc.h>
#include <server_share/lua/script_mgr.h>
#include <server_share/lua_net/lua_network.h>
#include <nel/misc/window_displayer.h>
#include <nel/net/naming_client.h>

#include "frontend_service.h"
#include <server_share/timer.h>
#include <server_share/lua/lua_thread.h>
#include <server_share/client_msg_desc.h>

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

    //string fn = IService::getInstance()->SaveFilesDirectory.toString();
    //fn += ConfigFile.getVar("LogDir").asString();
    //fn += "frontend_service.stat";
    //nlinfo("Frontend stat in directory '%s'", fn.c_str());
    //NLMISC::CFileDisplayer *Fd = new NLMISC::CFileDisplayer(fn);
    //Loger().addDisplayer(Fd);
    //if (WindowDisplayer) Loger().addDisplayer (WindowDisplayer);

    ///////////////////////////////////////////////////

    MsgDesc.LoadMsgXml();

    TimerManager->init();
    LuaThreadMgr.Init();
    LuaNetworkMgr.Init();
    ScriptMgr.init();
}

bool CFrontEndService::update()
{
    LocalTime.SetCurrTime( CTime::getLocalTime() );

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
    log.displayNL ("service information.");

	ScriptMgr.ExecString("ServiceInfo()");
    return true;
}

/* end of file */
