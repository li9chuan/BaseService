#include "process_msg_fes.h"
#include <game_share/proto_msg/msg_c2s.pb.h>
#include <server_share/server_def.h>
#include <server_share/database/def_db.h>
#include <server_share/msg_struct/msg_struct_pls.h>
#include <player_logic_service/lua/lua_function.h>

#include "../player/player_mgr.h"
#include "../login/data_sync.h"
#include "../event_trigger.h"
#include "../chat/chat_logic.h"


using namespace DEF;
using namespace NLNET;
using namespace PLS;
using namespace std;

void cbSyncChecksum( NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId sid )
{
    DEF::PID    pid=0;
    msgin.serial(pid);

    CPlayer* pPlayer = PlayerMgr.findPlayer( pid );

    if ( NULL != pPlayer )
    {
        MsgSyncDataCheckSum  check_sum = DataSyncMgr.GetCheckSum(pPlayer);

        CMessage  msg_out("SCS");
        msg_out.serial(&check_sum);
        SendToClient( msgin.session(), msg_out );
    }
}

void cbSyncPlayerInfo( NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId sid )
{
    DEF::PID    pid=0;
    msgin.serial(pid);
    DataSyncMgr.SyncToClientPlayerInfo(pid);
}

void cbGlobleChat( NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId sid )
{
    DEF::PID    pid=0;
    msgin.serial(pid);
    MsgChat    msg_chat;
    msgin.serial(&msg_chat);

    ChatLogic.GlobleChat( pid, msg_chat );
}

