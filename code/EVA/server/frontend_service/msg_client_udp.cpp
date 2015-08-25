#include "msg_client.h"
#include "session_mgr.h"
#include "client_mgr.h"
#include "udp_sender.h"
#include "frontend_service.h"
#include <game_share/proto_msg/msg_c2s.pb.h>
#include <server_share/server_def.h>

using namespace NLMISC;
using namespace NLNET;
using namespace FES;
using namespace DEF;

void cbUDPLogin( NLNET::CMessage& msgin, NLNET::CInetAddress& AddrFrom, MsgSession& ProtoSession )
{
    if ( UDPSender.IsAddrRecv(AddrFrom) )
    {
        UDPSender.SetAddrRecvCDStart(AddrFrom);
        MsgLogin    msg_login;

        try 
        {
            msgin.serial(&msg_login);

            RPC_SESSION rpc_session = SessionMgr.NewSession(CSessionMgr::UDP);
            //SessionMgr.addSession( rpc_session, pClient->uid );   ·µ»ØÊ±Ìí¼Ósession

            CMessage msgout(rpc_session, "LOGIN");
            msgout.serial (&msg_login);
            msgout.serial(AddrFrom);
        
            Network->send("LS", msgout);
            nldebug("recv client LOGIN. rpc_session:%u  token: %s  from:%s", (uint32)rpc_session, msg_login.token().c_str(), AddrFrom.asString().c_str());
        }
        catch (Exception &)
        {
            return;
        }
    }
    else
    {
        nldebug( "cbUDPLogin" );
    }
}


void cbUDPCreatePlayer( NLNET::CMessage& msgin, NLNET::CInetAddress& AddrFrom, MsgSession& ProtoSession )
{
    CClient* pClient = ClientMgr.findClientBySession(ProtoSession.session());

    if ( pClient!=NULL ) 
    {
        bool msg_valid = pClient->CheckMsgValid( msgin.buffer()+msgin.getPos(), msgin.length()-msgin.getPos(), 
            ProtoSession.index(), ProtoSession.check() );

        if ( msg_valid )
        {
            MsgCreatePlayer _MsgCreatePlayer;

            try
            {
                msgin.serial(&_MsgCreatePlayer);
            }
            catch (Exception &) {
                return;
            }
            
            pClient->addr_from = AddrFrom;
            RPC_SESSION rpc_session = SessionMgr.NewSession(CSessionMgr::UDP);
            SessionMgr.addSession( rpc_session, pClient->uid );

            CMessage msgout(rpc_session, "CP");
            msgout.serial ( pClient->uid );
            msgout.serial ( pClient->conPLS );
            msgout.serial ( &_MsgCreatePlayer );

            int name_length = _MsgCreatePlayer.name().length();
            if ( name_length<=PLAYERNAME_LENGTH && name_length>=PLAYERNAME_LENGTH_MIN )
            {
                Network->send("PDS", msgout);
            }
        }
    }
}



