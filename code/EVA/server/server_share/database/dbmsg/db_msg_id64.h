#ifndef DB_ENENT_ID64_H
#define DB_ENENT_ID64_H

#include <game_share/game_def.h>
#include <server_share/database/def_db.h>

struct DBMsgID64 : public DBMsgData
{
    uint64            id64;

    DBMsgID64( uint64 id ):id64(id)
    {

    }
};

#endif

