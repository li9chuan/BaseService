#include "sender.h"
#include "login_mgr.h"
#include <game_share/proto_msg/msg_s2c.pb.h>

using namespace DEF;
using namespace NLNET;



void LS::CSenderLogin::SyncSession( DEF::RPC_SESSION rpc_session, DEF::UID user_id, TServiceId sid, NLNET::CInetAddress& AddrFrom )
{
	CLIENT_SESSION client_session = LoginMgr.rand();
	client_session <<= 16;
	client_session |= LoginMgr.rand();
	client_session <<= 16;
	client_session |= LoginMgr.rand();
	client_session <<= 16;
	client_session |= user_id & 0xFFFF;

	MsgSetSession  set_session;
	set_session.set_seed(111);
	set_session.set_session(client_session);

	{
		CMessage sync_client_session(rpc_session,"SYNC_CLIENT_SESSION");
		sync_client_session.serial(user_id);
        sync_client_session.serial(&set_session);
        sync_client_session.serial(AddrFrom);

		Network->send( sid, sync_client_session );
	}
}

