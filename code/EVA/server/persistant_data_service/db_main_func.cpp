#include "db_main_func.h"
#include <nel/net/service.h>
#include <game_share/game_def.h>
#include <server_share/server_def.h>

#include "db_mgr.h"
#include "db_struct.h"
#include "db_connect.h"
#include "db_sub_func.h"
#include "db_worker.h"

using namespace std;
using namespace DEF;
using namespace DB_RECORD;

void db_main_range_value_data()
{
    GET_WORKER_AND_STMT_MAIN( ThreadLoadPlayer, DB_Player, stmt_query_player_maxid );
    ServerInfo _alone_value;

    pWorker->query( pStmt , &pResult );

    if( NULL!=pResult && pResult->next() )
    {
        _alone_value.player_max = pResult->get_uint64( 0 );
    }

    pResult->release();

    // 保存范围段;
    DBMgr.m_ServerInfo = _alone_value;

}
