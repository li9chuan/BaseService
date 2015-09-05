#include <server_share/database/db_logic_chunk.h>
#include <server_share/database/dbmsg/db_msg_id64.h>

#include "db_sub_func.h"
#include "db_worker.h"
#include "db_connect.h"
#include "db_main_func.h"
#include "db_struct.h"
#include "db_mgr.h"
#include "db_load_table.h"

using namespace DB_RECORD;


void db_sub_insert_playerinfo( void* data )
{
    GET_DBMSG( DB_RECORD::CRecordPlayer );
    GET_WORKER_AND_STMT( ThreadPlayerInfo, DB_Player, stmt_insert_player_data );

    int i = 0;

    pStmt->set_uint64( i++ , db_msg->pid );
    pStmt->set_uint64( i++ , db_msg->uid );
    pStmt->set_string( i++ , db_msg->name.c_str() , db_msg->name.length() );
    pStmt->set_uint16( i++ , db_msg->level );
    pStmt->set_uint64( i++ , db_msg->money );

    pWorker->execute( pStmt );
}

void db_sub_update_playerinfo( void* data )
{
    GET_DBMSG( DB_RECORD::CRecordPlayer );
    GET_WORKER_AND_STMT( ThreadPlayerInfo, DB_Player, stmt_update_player_data );

    int i = 0;

    pStmt->set_uint64( i++ , db_msg->pid );
    pStmt->set_string( i++ , db_msg->name.c_str() , db_msg->name.length() );
    pStmt->set_uint16( i++ , db_msg->level );
    pStmt->set_uint64( i++ , db_msg->money );

    pWorker->execute( pStmt );
}

void db_sub_delete_playerinfo( void* data )
{
    GET_DBMSG( DBMsgID64 );
    GET_WORKER_AND_STMT( ThreadPlayerInfo, DB_Player, stmt_delete_player_data );

    pStmt->set_uint64( 0, db_msg->id64 );
    pWorker->execute( pStmt );
}

