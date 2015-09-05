#include "db_worker.h"

bool init_alone_value_worker( nl::sql_worker* _worker )
{
    if ( NULL != _worker )
    {
        _worker->add_stmt( stmt_query_player_maxid   , "CALL _t_mt_query_pid_max()" );
        return true;
    }

    return false;
}

bool init_player_worker( nl::sql_worker* _worker )
{
    if ( NULL != _worker )
    {
        _worker->add_stmt( stmt_pre_treatment           , "CALL _t_mt_pre_treatment()" );
        _worker->add_stmt( stmt_after_treatment         , "CALL _t_mt_after_treatment()" );

        _worker->add_stmt( stmt_insert_player_data      , "CALL _t_mt_insert_playerinfo(?,?,?,?,?,?,?)" );
        _worker->add_stmt( stmt_update_player_data      , "CALL _t_mt_update_playerinfo(?,?,?,?,?)" );

        _worker->add_stmt( stmt_page_player_info        , "CALL _t_mt_pagination_playerinfo(?)" );

        return true;
    }

    return false;
}

bool init_load_role( nl::sql_worker* _worker )
{
    init_alone_value_worker( _worker );
    init_player_worker( _worker );
    return true;
}


