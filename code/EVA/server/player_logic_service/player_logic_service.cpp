#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <nel/misc/window_displayer.h>
#include <server_share/server_def.h>
#include <server_share/lua/lua_engine.h>
#include <server_share/lua/script_mgr.h>
#include <server_share/lua_net/lua_network.h>
#include <server_share/i18n_def.h>
#include <server_share/timer.h>
#include <nel/net/naming_client.h>


using namespace NLMISC;
using namespace NLNET;
using namespace std;

CFileDisplayer *Fd = NULL;
CStdDisplayer Sd;

NLMISC::CVariable<uint32>	VAR_PLAYER_NUM("pls", "NbPlayers"  , "memo", 0);
NLMISC::CVariable<uint32>	VAR_PLAYER_MAX("pls", "MaxPlayer"  , "memo", 100000, 0, true );

extern void admin_modules_forceLink();
void foo()
{
    admin_modules_forceLink();
}

void displayInfo ()
{
    ICommand::execute ("info", *NLMISC::InfoLog);
}

#include <server_share/id_generate.h>
class CPlayerLogicService : public NLNET::IService
{
public:

    /// Init the service, load the universal time.
    void init ()
    {
        ///////////////////////////////
        CI18N::load( Config.getVar("Language").asString() );

        ///////////////////////////////

        TimerManager->init();
        LuaNetworkMgr.Init();
        ScriptMgr.init();
    }


    bool update ()
    {
        NLMISC::TTicks curr_ticks = CTime::getLocalTime();
		LocalTime.SetCurrTime(curr_ticks);
        TimerManager->tickUpdate();
        ScriptMgr.update();
        LuaNetworkMgr.Update();
        return true;
    }

    void release ()
    {
		TimerManager->release();
        ScriptMgr.release();
        LuaNetworkMgr.Release();
    }

};

// Service instantiation
NLNET_SERVICE_MAIN (CPlayerLogicService, LogicService.c_str(), "player_logic_service", 0, EmptyCallbackArray, "", "");

//
// Commands
//

NLMISC_COMMAND (info, "service information.", "")
{
    if(args.size() != 0) return false;
    log.displayNL ("service information.");

    log.displayNL ("ClientNumber:%u", VAR_PLAYER_NUM.get());
    log.displayNL ("class count.");
    log.displayNL ("PlayerBarUpdateTimerEvent:%d", NL_GET_LOCAL_INSTANCE_COUNTER(CTimerEvent));
    //log.displayNL ("PlayerBarUpdateTimerEvent:%d", NL_GET_INSTANCE_COUNTER(CTimerEvent));

    return true;
}

