/////////////////////////

#include <nel/net/service.h>
#include <server_share/server_def.h>

#include "db_mgr.h"
#include "db_struct.h"
#include "db_connect.h"
#include "db_sub_func.h"
#include "db_worker.h"
#include "db_main_func.h"
#include "db_load_table.h"

using namespace std;
using namespace DEF;
using namespace DB_RECORD;

#define WHILE_NEXT_RESULT()\
    DEF::PID load_pid = 0;\
    DB_RECORD::CDBLogicPlayer* pPlayer = NULL;\
    uint32  rec_load_count = 0;\
    uint32  page_load_count = 0;\
    uint32  page_time = NLMISC::CTime::getLocalTime();\
    uint32  start_time = NLMISC::CTime::getSecondsSince1970();\
    while ( true )\
    {\
        uint32  query_time = NLMISC::CTime::getLocalTime();\
        pStmt->set_uint64( 0, load_pid );\
        pWorker->query( pStmt , &pResult );\
        query_time = NLMISC::CTime::getLocalTime()-query_time;\
        if ( pResult==NULL || pResult->count()==0 ) { break; }\
        while ( pResult->next() )

#define END_RESULT( __func_name, __load_pid )\
        if ( page_load_count>=100000 ){\
            page_time = NLMISC::CTime::getLocalTime() - page_time;\
            nlinfo(#__func_name" load count: %d   page_time:%d  query time:%d", page_load_count, page_time, query_time);\
            page_load_count = 0;\
            page_time = NLMISC::CTime::getLocalTime();\
        }\
        (load_pid==__load_pid) ? (++load_pid) : (load_pid=__load_pid);\
    }\
    pResult->release();\
    nlinfo(#__func_name" use seconds: %d  count:%d", NLMISC::CTime::getSecondsSince1970()-start_time, rec_load_count);

#define FOUND_PLAYER(__find_pid)\
    if ( pPlayer!=NULL ) { \
        if ( pPlayer->RecordPlayer.pid != __find_pid ) { \
            pPlayer = DBMgr.find_player_pid( __find_pid );\
        } \
    }\
    else { \
        pPlayer = DBMgr.find_player_pid( __find_pid );\
    }\
    if ( pPlayer!=NULL ){\
        ++page_load_count;\
        ++rec_load_count;\
    {


#define END_FOUND()  }}


void pageload_playerinfo()
{
    GET_WORKER_AND_STMT_MAIN( ThreadLoadPlayer, DB_Player, stmt_page_player_info );

    WHILE_NEXT_RESULT()
    {
        pPlayer = new(std::nothrow) DB_RECORD::CDBLogicPlayer();

        if ( pPlayer!=NULL )
        {
            load_table_player( pPlayer->RecordPlayer, pResult );

            DBMgr.save_playerlist(pPlayer->RecordPlayer.uid , pPlayer->RecordPlayer.pid);
            DBMgr.save_player_detail(pPlayer, false);

            ++rec_load_count;
            ++page_load_count;
        }
    }
    END_RESULT(pageload_playerinfo, pPlayer->RecordPlayer.pid)
}



void db_main_load_all_data()
{
    uint32  start_time = NLMISC::CTime::getSecondsSince1970();

    pageload_playerinfo();


    nlinfo("db_main_load_all_data use seconds: %d", NLMISC::CTime::getSecondsSince1970()-start_time);
}
