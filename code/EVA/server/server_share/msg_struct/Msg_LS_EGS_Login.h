#ifndef MSG_LS_EGS_LOGIN_H
#define MSG_LS_EGS_LOGIN_H

#include <game_share/game_def.h>

struct CMsg_LS_EGS_Login
{
	DEF::RPC_SESSION session;
	DEF::UID          uid;

	void serial( NLMISC::IStream& s )
	{
		s.serial( session );
		s.serial( uid );
	}

	CMsg_LS_EGS_Login():session(0),uid(0)
	{}


};

#endif
