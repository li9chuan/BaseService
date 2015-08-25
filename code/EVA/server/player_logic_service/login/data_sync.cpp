#include "data_sync.h"
#include <server_share/database/db_logic_chunk.h>
#include <server_share/server_def.h>
#include <player_logic_service/player/player_mgr.h>

using namespace std;
using namespace NLNET;

DEF::CHECK_SUM  PLS::CDataSyncMgr::CheckSum( google::protobuf::Message* pMessage )
{
    string check_buffer;
    pMessage->SerializeToString(&check_buffer);
    DEF::CHECK_SUM check_sum = crc32( 0, (const uint8 *)check_buffer.c_str(), check_buffer.length() );
    return check_sum;
}

MsgSyncDataCheckSum PLS::CDataSyncMgr::GetCheckSum( CPlayer* pPlayer )
{
    nlassert(pPlayer!=NULL);

    MsgSyncDataCheckSum  check_sum;


    return check_sum;
}

void PLS::CDataSyncMgr::SyncToClientPlayerInfo( DEF::PID pid )
{
    CPlayer* pPlayer = PlayerMgr.findPlayer( pid );

    if ( NULL != pPlayer )
    {
        MsgPlayerInfo  player_info;
        pPlayer->getPlayerHelper().getMsgBase( player_info );

        CMessage  msg_out("SPLY");
        msg_out.serial(&player_info);

        SendUDP( pPlayer->getConFES(), pPlayer->getUID(), msg_out, true );
    }
}

void PLS::CDataSyncMgr::CheckSumToClientBuffer( CPlayer* pPlayer )
{
    if ( NULL != pPlayer )
    {
        MsgSyncDataCheckSum  check_sum = GetCheckSum(pPlayer);
        AddToClientBuffer( pPlayer->getConFES(), pPlayer->getUID(), "SCS", &check_sum );
    }
}

