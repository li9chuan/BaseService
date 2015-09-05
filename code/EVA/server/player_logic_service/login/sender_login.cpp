#include "sender_login.h"

#include <server_share/database/def_db.h>
#include <server_share/server_def.h>
#include <game_share/tools.h>

#include <player_logic_service/lua/lua_event.h>
#include <server_share/script_mgr.h>
#include "data_sync.h"

using namespace NLNET;
using namespace DEF;
using namespace std;
using namespace DB_RECORD;
using namespace PLS;

void PLS::CSenderLogin::SendPlayerInfo( DEF::RPC_SESSION session, CPlayer* pPlayer )
{
	//pPlayer->getPlayerHelper().getRecordPlayer().last_login = CTime::getSecondsSince1970();
    if ( NULL == pPlayer )
    {
        nldebug( " load player info faile " );
        return;
    }

    CMessage  msg_out("SyncData");

    MsgSyncDataCheckSum  check_sum = DataSyncMgr.GetCheckSum(pPlayer);

    MsgPlayerInfo player_info;
    pPlayer->getPlayerHelper().getMsgPlayerInfo(player_info);

    msg_out.serial(&check_sum);
    msg_out.serial(&player_info);

    SendToClient( session, msg_out );
}

