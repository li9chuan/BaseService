#ifndef EVA_PLS_RECV_LOGIN_H
#define EVA_PLS_RECV_LOGIN_H

#include <game_share/game_def.h>
#include <nel/net/unified_network.h>
#include <nel/misc/singleton.h>

namespace PLS
{
	class CRecvLogin : public NLMISC::CSingleton<CRecvLogin>
	{
	public:
		void SendPlayerInfo( NLNET::CMessage& msg , NLNET::TServiceId from );
	};




}





#define  RecvLogin  PLS::CRecvLogin::instance()

#endif // EVA_PLS_RECV_LOGIN_H


