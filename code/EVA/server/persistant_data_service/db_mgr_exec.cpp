#include <nel/misc/variable.h>
#include <server_share/database/dbmsg/db_msg_id64.h>
#include "db_mgr.h"
#include "db_connect.h"
#include "db_sub_func.h"
#include "db_struct.h"
#include "db_main_func.h"

using namespace DB_RECORD;

NLMISC::CVariable<uint32>	VAR_UPDATE_ACCOUNT_NUM("db", "UpdateAccountNum" , "memo", 2, 0, true);
NLMISC::CVariable<uint32>	VAR_SQL_EXEC_LIMIT("db", "SQLExecEventLimit", "memo", 100, 0, true);


void CDBMgr::load_player_info( void )
{
    db_main_load_all_data();
    db_main_range_value_data();
}

bool CDBMgr::update_player( TDBSaveOPT _opt, NLNET::CMessage& _msg )
{
    if ( _opt!=SAVE_OPT_UPDATE )
    {
        return false;
    }

    DEF::PID pid;
    DB_RECORD::CDBLogicPlayer logic_player;

    _msg.serial( pid );
    _msg.serial( logic_player );

    DB_RECORD::CDBLogicPlayer* pLogicPlayer = find_player_pid( pid );

    if ( pLogicPlayer!=NULL )
    {
        *pLogicPlayer = logic_player;
    }
    else
    {
        //logic_player.RecordPlayer.set_insert();
        //save_player_detail( &logic_player );
    }

    return true;
}

void CDBMgr::need_update_players( std::list< DEF::PID >& update_players )
{
    if ( m_DBPlayers.empty() ) {
        return;
    }

    update_players.clear();
    TDBLogicPlayer::iterator player_iter = m_DBPlayers.find( m_SaveCurrPID );

    while ( player_iter!=m_DBPlayers.end()  &&
        update_players.size()<VAR_UPDATE_ACCOUNT_NUM )
    {
        update_players.push_back( player_iter->first );
        ++player_iter;
    }

    if ( player_iter!=m_DBPlayers.end() )
    {
        m_SaveCurrPID = player_iter->first;
    }
    else
    {
        m_SaveCurrPID = m_DBPlayers.begin()->first;
    }
}

void CDBMgr::db_frame_move( void )
{
    if ( is_exec_limit() )  {   return;     }

    std::list< DEF::PID > _list;
    need_update_players( _list );
    DB_RECORD::CDBLogicPlayer* pPlayer = NULL;

    for ( std::list< DEF::PID >::iterator _it = _list.begin() ; _it != _list.end() ; ++_it )
    {
        TDBLogicPlayer::iterator _entity_it = m_DBPlayers.find( *_it );

        if ( _entity_it == m_DBPlayers.end() || !_entity_it->second )
        {
            continue;
        }

        pPlayer = _entity_it->second;

        ///     每个表的数据处理
        execute_player_table( pPlayer->RecordPlayer );


    }
}

void CDBMgr::execute_player_table( DB_RECORD::CRecordPlayer& _rhs )
{
    if ( _rhs.is_insert() ){
        SelectThread(ThreadPlayerInfo).post_sub_msg( new DB_RECORD::CRecordPlayer( _rhs ), (nl::_proc_msg)db_sub_insert_playerinfo );
    }
    else if( _rhs.is_update() ){
        SelectThread(ThreadPlayerInfo).post_sub_msg( new DB_RECORD::CRecordPlayer( _rhs ), (nl::_proc_msg)db_sub_update_playerinfo );
    }

    _rhs.rest_flag();
}

bool CDBMgr::is_exec_limit()
{
    if ( SelectThread(ThreadPlayerInfo).sub_event_size()    > VAR_SQL_EXEC_LIMIT
         )
    {
        return true;
    }

    return false;
}


