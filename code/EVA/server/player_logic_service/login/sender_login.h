#ifndef EVA_PLS_SENDER_LOGIN_H
#define EVA_PLS_SENDER_LOGIN_H

#include <game_share/game_def.h>
#include <server_share/server_def.h>
#include <nel/misc/singleton.h>
#include <server_share/database/db_logic_chunk.h>
#include <player_logic_service/player/player.h>

namespace PLS
{
	class CSenderLogin : public NLMISC::CSingleton<CSenderLogin>
	{
	public:
		void SendPlayerInfo( DEF::RPC_SESSION session, CPlayer* pPlayer );
	};

}





#define  SenderLogin  PLS::CSenderLogin::instance()

#endif
