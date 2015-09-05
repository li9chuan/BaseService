#ifndef PDS_DB_LOAD_TABLE_H
#define PDS_DB_LOAD_TABLE_H

#include <game_share/game_def.h>
#include <server_share/database/db_logic_chunk.h>
#include <server_share/db_threads/sql_result.h>

inline void  load_table_player( DB_RECORD::CRecordPlayer& RecordPlayer, nl::sql_result* pResult )
{
    uint i = 0;

    RecordPlayer.pid                 = pResult->get_uint64( i++ );
    RecordPlayer.uid                 = pResult->get_uint64( i++ );
    RecordPlayer.name                = pResult->get_string( i++ ).second;
    RecordPlayer.level               = pResult->get_uint16( i++ );
    RecordPlayer.money               = pResult->get_uint64( i++ );
}



#endif // PDS_DB_LOAD_TABLE_H





