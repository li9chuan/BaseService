#ifndef PLS_PLAYER_MGR_H
#define PLS_PLAYER_MGR_H

#include <nel/net/unified_network.h>
#include <nel/misc/time_nl.h>
#include <nel/misc/singleton.h>

#include "player.h"
#include <queue>

namespace PLS
{
class CPlayerMgr : public NLMISC::CSingleton<CPlayerMgr>
{
public:

	CPlayerMgr();

    CPlayer* alloc();
    void free( CPlayer* pPlayer );

    bool push( CPlayer* pPlayer );


    uint32 getPlayerNum()	{ return _PlayerMap.size(); }

    CPlayer* findPlayer(DEF::PID pid, bool update_heartbeat=true);


    void removePlayer(DEF::PID pid);

    void update(NLMISC::TTicks curr_ticks);

    void RemoveFES(NLNET::TServiceId sid);
    void RemoveAllPlayer();

    typedef CHashMap<DEF::PID, CPlayer*>		TPLAYER_MAP;

    const TPLAYER_MAP::iterator  getPlayerMapBegin()	{ return _PlayerMap.begin(); }
    const TPLAYER_MAP::iterator  getPlayerMapEnd()		{ return _PlayerMap.end(); }

private:

    TPLAYER_MAP             _PlayerMap;

};

}

#define  PlayerMgr  PLS::CPlayerMgr::instance()

#endif		//	PLS_PLAYER_MGR_H









