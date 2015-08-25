#include "player.h"

#include <server_share/server_def.h>
#include <game_share/static_table/static_table_mgr.h>

#include "player_timer_event.h"

namespace PLS
{
	using namespace DB_RECORD;
	using namespace NLMISC;
	using namespace NLNET;
	using namespace DEF;
	using namespace std;

	CPlayer::CPlayer() 
        : conFES(NLNET::TServiceId::InvalidId), 
          m_DataHelper(DBLogicPlayer)
    {
        m_DataHelper.m_Player = this;
    }

	CPlayer::~CPlayer()
	{
		conFES = NLNET::TServiceId::InvalidId;
	}

	void CPlayer::fillData( NLNET::CMessage &msgin )
	{
		m_LastUpdateTime = LocalTime.GetCurrTime();

		msgin.serial( DBLogicPlayer );

	}

    void CPlayer::FirstLogin( void )
    {

    }



    void CPlayer::save()
    {
        SaveToDB( UPDATE_PLAYER, DBLogicPlayer.RecordPlayer.pid, DBLogicPlayer );
    }


}


