
#include "db_connect.h"
#include "db_worker.h"
#include <game_share/game_def.h>
#include <nel/misc/debug.h>
#include <nel/misc/hierarchical_timer.h>

CMysqlConnect::CMysqlConnect( void )
{
    m_DBThreads.resize(ThreadsMax);
}

void CMysqlConnect::init_db_connect( char const* _host,
                                  char const* _user,
                                  char const* _pwd,
                                  char const* _db,
                                  short _port )
{
    m_DBThreads[ThreadPlayerInfo].connect(  DB_Player, _host, _user, _pwd, _db, _port, (nl::_init_sql_worker)init_player_worker );
    m_DBThreads[ThreadLoadPlayer].connect(  DB_Player, _host, _user, _pwd, _db, _port, (nl::_init_sql_worker)init_load_role );

}

bool CMysqlConnect::start_db_threads( void )
{
    for ( uint32 i=0; i<ThreadsMax; ++i )
    {
        if ( !m_DBThreads[i].start() )
        {
            nlerror("DBThreads[%d] start faile.", i);
        }
    }

    return true;
}

void CMysqlConnect::close_db_threads( void )
{
    for ( uint32 i=0; i<ThreadsMax; ++i )
    {
        m_DBThreads[i].close();
    }
}

void CMysqlConnect::frame_move( void )
{
    H_AUTO(db_connect_frame_move);

    m_DBThreads[ThreadLoadPlayer].frame_move();
}

bool CMysqlConnect::db_pre_treatment( void )
{
    nl::sql_worker* _worker = m_DBThreads[ThreadLoadPlayer].get_worker( DB_Player, true );
    if( NULL == _worker )
        return false;

    nl::sql_stmt* _stmt = _worker->get_stmt( stmt_pre_treatment );
    if ( NULL == _stmt )
        return false;

    return ( -1 != _worker->execute( _stmt ) );
}

bool CMysqlConnect::db_after_treatment( void )
{
    nl::sql_worker* _worker = m_DBThreads[ThreadLoadPlayer].get_worker( DB_Player, true );
    if( NULL == _worker )
        return false;

    nl::sql_stmt* _stmt = _worker->get_stmt( stmt_after_treatment );
    if( NULL == _stmt )
        return false;

    return ( -1 != _worker->execute( _stmt ) );
}