#include "player_mgr.h"
#include <server_share/server_def.h>
#include <nel/net/service.h>
#include <game_share/static_table/static_table_mgr.h>
#include "player_data_helper.h"

extern NLMISC::CVariable<uint32>	VAR_PLAYER_NUM;

using namespace PLS;
using namespace NLMISC;
using namespace NLNET;
using namespace ATTRIB_TYPE;

PLS::CPlayerMgr::CPlayerMgr()
{
}

CPlayer* PLS::CPlayerMgr::alloc()
{
    return new(std::nothrow) CPlayer;
}

void PLS::CPlayerMgr::free( CPlayer* pPlayer )
{
    if (pPlayer != NULL)
    {
        delete pPlayer;
        pPlayer = NULL;
    }
}

void PLS::CPlayerMgr::update(NLMISC::TTicks curr_ticks)
{

}

void PLS::CPlayerMgr::RemoveAllPlayer()
{
    TPLAYER_MAP::iterator iter = _PlayerMap.begin();
    while ( iter != _PlayerMap.end() )
    {
        free(iter->second);
        ++iter;
    }
    _PlayerMap.clear();
    VAR_PLAYER_NUM = getPlayerNum();
}

CPlayer* PLS::CPlayerMgr::findPlayer( DEF::PID pid, bool update_heartbeat )
{
	CPlayer* pPlayer = NULL;

	TPLAYER_MAP::iterator iter = _PlayerMap.find(pid);
	if( iter != _PlayerMap.end() )
	{
		pPlayer = iter->second;

		if ( update_heartbeat )
		{
			pPlayer->SetLastUpdateTime( LocalTime.GetCurrTime() );
		}
	}

	return pPlayer;
}

void PLS::CPlayerMgr::removePlayer( DEF::PID id )
{
	TPLAYER_MAP::iterator iter = _PlayerMap.find(id);
	if( iter != _PlayerMap.end() )
	{
        iter->second->getPlayerHelper().UpdateLastOnlineTime();
		free(iter->second);
		_PlayerMap.erase(iter);
        VAR_PLAYER_NUM = getPlayerNum();
	}
}

void PLS::CPlayerMgr::RemoveFES(TServiceId sid)
{
    TPLAYER_MAP::iterator iter = _PlayerMap.begin();
    while ( iter != _PlayerMap.end() )
    {
        if ( iter->second->getConFES() == sid )
        {
            free(iter->second);
            _PlayerMap.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
    VAR_PLAYER_NUM = getPlayerNum();
}

bool PLS::CPlayerMgr::push( CPlayer* pPlayer )
{
	if( pPlayer == NULL )
	{
		return false;
	}

	TPLAYER_MAP::iterator iter = _PlayerMap.find( pPlayer->getPID() );

	if(iter == _PlayerMap.end())
	{
		_PlayerMap.insert( std::make_pair(pPlayer->getPID(), pPlayer) );
        VAR_PLAYER_NUM = getPlayerNum();
		return true;
	}

	nlwarning("Warning**  PLS::CPlayerMgr Memory leak. playerID:%"NL_I64"d", pPlayer->getPID());
	free(pPlayer);
	return false;
}






