#include "recv_login.h"

#include <persistant_data_service/sender_mgr/sender_login.h>
#include <persistant_data_service/db_logic.h>
#include <persistant_data_service/db_mgr.h>

#include <game_share/define_pro.pb.h>
#include <game_share/proto_msg/msg_c2s.pb.h>
#include <server_share/server_def.h>

#include "../db_connect.h"
#include "../db_sub_func.h"
#include "../db_struct.h"

using namespace NLNET;
using namespace DEF;
using namespace std;
using namespace DB_RECORD;
using namespace PDS;

void PDS::CRecvLogin::RequestCreatePlayer( NLNET::CMessage& msgin )
{
    UID uid;
    TServiceId logic_id;
    MsgCreatePlayer _MsgCreatePlayer;

    msgin.serial(uid);
    msgin.serial(logic_id);
    msgin.serial(&_MsgCreatePlayer);

    // 检查是否存在角色;
    CDBLogicPlayer* pRoleInst = DBMgr.find_player_uid( uid , 0 );
    if ( NULL == pRoleInst ) {
        pRoleInst = DBMgr.create_player( uid , _MsgCreatePlayer.name() );
    }

    if ( NULL == pRoleInst ) {
        return;
    }

    SendPlayerBaseToClient( msgin.session(), uid );
}

void PDS::CRecvLogin::LoadPlayer( NLNET::CMessage& msgin )
{
    UID uid;
    TServiceId logic_id;

    msgin.serial(uid);
    msgin.serial(logic_id);

    DEF::PID unRoleID = DBMgr.find_pid( uid , 0 );

    CDBLogicPlayer* pRoleInst = DBMgr.find_player_pid( unRoleID );
    if ( NULL == pRoleInst ) {
        SenderLogin.SendPlayerNull( msgin.session() );
    }
    else {
        LoadPlayer( unRoleID , logic_id, msgin.session(), "" );
    }
}

void PDS::CRecvLogin::LoadPlayer( DEF::PID pid, NLNET::TServiceId logic_id, DEF::RPC_SESSION session, std::string room_type )
{
    DB_RECORD::CDBLogicPlayer* pPlayer = DBMgr.find_player_pid(pid);

    if ( pPlayer!=NULL )
    {
        SenderLogin.SendLoadPlayerSucess( session, logic_id, *pPlayer, room_type );
    }
}

void PDS::CRecvLogin::SendPlayerBaseToClient( DEF::RPC_SESSION rpc_session, DEF::UID uid )
{
    DEF::PID pid = DBMgr.find_pid( uid , 0 );

    CDBLogicPlayer* pPlayer = DBMgr.find_player_pid( pid );

    if ( NULL == pPlayer )
    {
        SenderLogin.SendPlayerNull( rpc_session );
    }
    else
    {
        // egs;
        CMessage egsMsg(rpc_session, "SPB");
        egsMsg.serial( pPlayer->RecordPlayer );
        Network->send( "EGS", egsMsg );

        // mail;
        CMessage mailMsg("SPB");
        mailMsg.serial( pPlayer->RecordPlayer );
        Network->send( "MAIL" , mailMsg );
    }
}
