#ifndef EVA_PDS_SENDER_LOGIN_H
#define EVA_PDS_SENDER_LOGIN_H

#include <game_share/game_def.h>
#include <server_share/server_def.h>
#include <nel/misc/singleton.h>
#include <server_share/database/db_logic_chunk.h>
#include <game_share/game_def.h>

namespace PDS
{
    class CSenderLogin : public NLMISC::CSingleton<CSenderLogin>
    {
    public:
        void SendPlayerNull( DEF::RPC_SESSION session );
        void SendLoadPlayerSucess( DEF::RPC_SESSION session, NLNET::TServiceId pls, DB_RECORD::CDBLogicPlayer& DBPlayer, std::string room_type );

        void SendPreLoadPlayerFPSEnd( NLNET::TServiceId sid, DEF::PID pid );
        void SendPreLoadPlayerDone( NLNET::TServiceId sid );
    };
}





#define  SenderLogin  PDS::CSenderLogin::instance()

#endif
