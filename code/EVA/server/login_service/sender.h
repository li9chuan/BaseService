#ifndef EVA_LS_SENDER_LOGIN_H
#define EVA_LS_SENDER_LOGIN_H

#include <game_share/game_def.h>
#include <server_share/server_def.h>
#include <nel/misc/singleton.h>


namespace LS
{
	class CSenderLogin : public NLMISC::CSingleton<CSenderLogin>
	{
	public:
		void SyncSession( DEF::RPC_SESSION rpc_session, DEF::UID user_id, NLNET::TServiceId sid, NLNET::CInetAddress& AddrFrom );


	};

}





#define  Sender  LS::CSenderLogin::instance()

#endif


