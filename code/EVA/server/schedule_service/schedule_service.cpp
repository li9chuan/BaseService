#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <server_share/tools.h>
#include <server_share/server_def.h>
#include <server_share/client_msg_desc.h>
#include <server_share/lua/script_mgr.h>
#include <server_share/lua_net/lua_network.h>
#include <nel/misc/window_displayer.h>
#include <nel/net/naming_client.h>

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

class CScheduleService : public NLNET::IService
{
public:
    void init();
    bool update();
    void release();
};

void CScheduleService::init()
{
    LocalTime.SetCurrTime( CTime::getLocalTime() );

    TimerManager->init();
    LuaThreadMgr.Init();
    LuaNetworkMgr.Init();
    ScriptMgr.init();
}

bool CScheduleService::update()
{
    TimerManager->tickUpdate();
    ScriptMgr.update();
    LuaNetworkMgr.Update();
    LuaThreadMgr.Update();

    return true;
}

void CScheduleService::release()
{
    TimerManager->release();
    ScriptMgr.release();
    LuaNetworkMgr.Release();
    LuaThreadMgr.Release();

    google::protobuf::ShutdownProtobufLibrary();
}


/****************************************************************************   
 * SCHEDULE SERVICE MAIN Function
 *
 * This call create a main function for a service:
 *
 *    - based on the "CScheduleService" class
 *    - having the short name "SCH"
 *    - having the long name "schedule_service"
 *    - listening on the port "0" (dynamically determined)
 *    - and shard callback set to "CallbackArray"
 *
 ****************************************************************************/
NLNET_SERVICE_MAIN (CScheduleService, "SCH", "schedule_service", 49971, EmptyCallbackArray, "", "")

NLMISC_COMMAND (info, "service information.", "")
{
    if(args.size() != 0) return false;
    log.displayNL ("service information.");

	ScriptMgr.ExecString("ServiceInfo()");
    return true;
}

/* end of file */
