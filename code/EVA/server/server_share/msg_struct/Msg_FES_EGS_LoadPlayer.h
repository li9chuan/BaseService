#ifndef MSG_FES_EGS_LOAD_PLAYER_H
#define MSG_FES_EGS_LOAD_PLAYER_H

#include <game_share/game_def.h>

struct CMsg_FES_EGS_LoadPlayer
{
	DEF::UID          uid;
	uint32            player_idx;

	void serial( NLMISC::IStream& s )
	{
		s.serial( uid );
		s.serial( player_idx );
	}

	CMsg_FES_EGS_LoadPlayer():uid(0),player_idx(0)
	{}


};

#endif
