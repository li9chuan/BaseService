#ifndef SERVICE_SHARD_MSG_SERVER_INFO_H
#define SERVICE_SHARD_MSG_SERVER_INFO_H

#include <game_share/game_def.h>
#include <nel/misc/stream.h>

struct ServerInfo
{
    uint32    sid;
    uint64    player_max;

    ServerInfo( void )
        : sid( 1 )
        , player_max( 1 )
    { }

    virtual ~ServerInfo()    {}

    void serial( NLMISC::IStream& s )
    {
        s.serial( sid );
        s.serial( player_max );
    }
};

#endif
