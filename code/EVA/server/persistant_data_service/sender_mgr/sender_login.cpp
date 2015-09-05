#include "sender_login.h"

#include <server_share/database/def_db.h>
#include <server_share/server_def.h>
#include <game_share/tools.h>

#include "../db_logic.h"

using namespace NLNET;
using namespace DEF;
using namespace std;
using namespace DB_RECORD;
using namespace PDS;


void PDS::CSenderLogin::SendPlayerNull( RPC_SESSION session )
{
    CMessage msgout("PLY_NULL");
    SendToClient( session, msgout );
}

void PDS::CSenderLogin::SendLoadPlayerSucess( DEF::RPC_SESSION session, NLNET::TServiceId pls, DB_RECORD::CDBLogicPlayer& DBPlayer, std::string room_type )
{
    CMessage msgout( session, "EnterGame_1");
    msgout.serial( DBPlayer );
    msgout.serial( room_type );
    
    Network->send( pls, msgout );
}

void PDS::CSenderLogin::SendPreLoadPlayerFPSEnd( NLNET::TServiceId sid, DEF::PID pid )
{
    CMessage msgout("FpsEnd");
    msgout.serial(pid);
    Network->send( sid , msgout );
}

void PDS::CSenderLogin::SendPreLoadPlayerDone( NLNET::TServiceId sid )
{
    CMessage msgout("LPD");
    Network->send( sid , msgout );
}

