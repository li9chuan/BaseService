#include "db_manager_messages.h"

#include <persistant_data_service/receiver_mgr/recv_login.h>
#include <persistant_data_service/db_logic.h>
#include <nel/misc/hierarchical_timer.h>
#include <server_share/database/def_db.h>
#include <server_share/server_def.h>
#include <game_share/tools.h>

using namespace NLNET;
using namespace DEF;
using namespace std;
using namespace DB_RECORD;
using namespace PDS;

void cbPut  (NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId  sid)
{
    DBLogic.putData( msgin );
}

void cbLoadPlayer( NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId sid )
{
	RecvLogin.LoadPlayer(msgin);
}

void cbCreatePlayer( NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId sid )
{
    RecvLogin.RequestCreatePlayer(msgin);
}

void cbEnterGame( NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId sid )
{
    DEF::PID  pid;
    NLNET::TServiceId  pls_id;
    std::string  room_type;

    msgin.serial(pid);
    msgin.serial(pls_id);
    msgin.serial(room_type);

    RecvLogin.LoadPlayer( pid, pls_id, msgin.session(), room_type );
}

void cbPlayerBaseToClient( NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId sid )
{
    DEF::UID uid;
    msgin.serial(uid);

    RecvLogin.SendPlayerBaseToClient( msgin.session(), uid );
}

void cbRequestNextPlayer( NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId sid )
{
    DEF:PID pid=0;
    msgin.serial( pid );

    DBLogic.PreLoadNextPlayerInfo(sid, pid);
}

