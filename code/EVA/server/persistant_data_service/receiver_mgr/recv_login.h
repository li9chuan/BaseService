#ifndef EVA_PDS_RECV_LOGIN_H
#define EVA_PDS_RECV_LOGIN_H

#include <game_share/game_def.h>
#include <server_share/server_def.h>

#include <nel/misc/singleton.h>

namespace PDS
{
    class CRecvLogin : public NLMISC::CSingleton<CRecvLogin>
    {
    public:
        void RequestCreatePlayer( NLNET::CMessage& msgin );
        void LoadPlayer( NLNET::CMessage& msgin );

        void SendPlayerBaseToClient( DEF::RPC_SESSION session, DEF::UID uid );
        void LoadPlayer( DEF::PID pid, NLNET::TServiceId logic_id, DEF::RPC_SESSION session, std::string room_type );
    };
}

#define  RecvLogin  PDS::CRecvLogin::instance()



#endif // EVA_CLIENT_RECV_LOGIN_H


