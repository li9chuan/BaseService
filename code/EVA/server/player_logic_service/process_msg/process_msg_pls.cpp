#include "process_msg_pls.h"
#include "../player/player_mgr.h"
#include "../login/recv_login.h"

#include <server_share/database/record_player.h>
#include <server_share/server_def.h>
#include <server_share/database/def_db.h>
#include <server_share/msg_struct/msg_struct_pls.h>


using namespace DEF;
using namespace NLNET;
using namespace PLS;

void cbFESConnection( const std::string &serviceName, NLNET::TServiceId sid, void *arg )
{
    //PlayerMgr.SaveAllPlayer();
}

void cbFESDisconnection( const std::string &serviceName, NLNET::TServiceId sid, void *arg )
{
    //PlayerMgr.RemoveFES(sid);
}

void cbPlayerLogout( NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId sid )
{
    PID pid = 0;
    msgin.serial(pid);

    PlayerMgr.removePlayer(pid);
}

