#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <nel/misc/window_displayer.h>
#include <server_share/server_def.h>
#include <server_share/lua/lua_engine.h>
#include <server_share/lua/script_mgr.h>
#include <server_share/lua/lua_thread.h>
#include <server_share/lua_net/lua_network.h>
#include <server_share/i18n_def.h>
#include <server_share/timer.h>

#include <nel/net/naming_client.h>


using namespace NLMISC;
using namespace NLNET;
using namespace std;

CFileDisplayer *Fd = NULL;
CStdDisplayer Sd;

//extern void admin_modules_forceLink();
//void foo()
//{
//    admin_modules_forceLink();
//}

void displayInfo ()
{
    ICommand::execute ("info", *NLMISC::InfoLog);
}

class CPlayerLogicService : public NLNET::IService
{
public:

    /// Init the service, load the universal time.
    void init ()
    {
        TimerManager->init();
        LuaNetworkMgr.Init();
        ScriptMgr.init();
        LuaThreadMgr.Init();
    }


    bool update ()
    {
        NLMISC::TTicks curr_ticks = CTime::getLocalTime();
		LocalTime.SetCurrTime(curr_ticks);
        TimerManager->tickUpdate();
        ScriptMgr.update();
        LuaNetworkMgr.Update();
        LuaThreadMgr.Update();
        return true;
    }

    void release ()
    {
		TimerManager->release();
        ScriptMgr.release();
        LuaNetworkMgr.Release();
        LuaThreadMgr.Release();

        google::protobuf::ShutdownProtobufLibrary();
    }

};

// Service instantiation
NLNET_SERVICE_MAIN (CPlayerLogicService, LogicService.c_str(), "player_logic_service", 0, EmptyCallbackArray, "", "");




