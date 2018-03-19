#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <nel/misc/window_displayer.h>
#include <server_share/server_def.h>
#include <server_share/lua_engine.h>
#include <server_share/script_mgr.h>
#include <server_share/i18n_def.h>
#include <game_share/static_table/static_table_mgr.h>
#include <game_share/singleton_registry.h>

#include "process_msg/process_msg_pls.h"
#include "process_msg/process_msg_fes.h"

#include "player/player_mgr.h"
#include "player/player_timer_event.h"
#include "lua/lua_event_callback.h"
#include <nel/net/naming_client.h>

using namespace NLMISC;
using namespace NLNET;
using namespace std;
using namespace PLS;

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

NLNET::TUnifiedCallbackItem PLSCallbackArray[] =
{
    ///   msg.xml
    { "SCS",                        cbSyncChecksum          },     // 客户端请求数据校验和
    { "SPLY",                       cbSyncPlayerInfo        },
    { "GC",                         cbGlobleChat            },


    ///     
    { "LOGOUT",                     cbPlayerLogout          },
};

class CPlayerLogicService : public NLNET::IService
{
public:

    /// Init the service, load the universal time.
    void init ()
    {
        CUnifiedNetwork::getInstance()->setServiceUpCallback("FES", cbFESConnection);
        CUnifiedNetwork::getInstance()->setServiceDownCallback("FES", cbFESDisconnection);

        ///////////////////////////////    注册监听事件
        CLuaEventCallback::getInstance().RegisterMoniterEvent();


        ///////////////////////////////
        CI18N::load( Config.getVar("Language").asString() );

        ///////////////////////////////

        TimerManager->init();
        ScriptMgr.init();

        ICommand::execute ("loadconfig", *InfoLog);
    }


    bool update ()
    {
        NLMISC::TTicks curr_ticks = CTime::getLocalTime();
		LocalTime.SetCurrTime(curr_ticks);
        PlayerMgr.update(curr_ticks);
        TimerManager->tickUpdate();

        return true;
    }

    void release ()
    {
        ScriptMgr.release();
		TimerManager->release();
    }

};

// Service instantiation
NLNET_SERVICE_MAIN (CPlayerLogicService, LogicService.c_str(), "player_logic_service", 0, PLSCallbackArray, "", "");

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

NLMISC_COMMAND (loadconfig, "reload config file.", "")
{
    if(args.size() != 0) return false;

    log.displayNL ("PLS Load config files...");

    StaticTableMgr.init();

    log.displayNL ("PLS Load config done.");
    return true;
}

NLMISC_COMMAND ( playerinfo, "player info", "")
{
    if(args.size() != 1) return false;

    CSString pid_str = args[0];
    PLS::CPlayer* pPlayer = PlayerMgr.findPlayer( pid_str.atoi64() );

    if ( pPlayer != NULL )
    {
        log.displayNL( "Player Base Info:" );
        log.displayNL( "Name:%s       UID:%d", pPlayer->getPlayerHelper().getName().c_str(), (uint32)pPlayer->getUID() );
    }
    else
    {
        log.displayNL( "Player Not Found." );
    }

    return true;
}

