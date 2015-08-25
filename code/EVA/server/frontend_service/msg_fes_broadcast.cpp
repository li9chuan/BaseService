#include "msg_fes_broadcast.h"
#include <server_share/server_def.h>

#include "client_mgr.h"
#include "frontend_service.h"
#include "session_mgr.h"
#include "client_mgr.h"
#include "proto_msg/msg_s2c.pb.h"

extern NLNET::CCallbackServer *Clients;

using namespace DEF;
using namespace FES;
using namespace NLNET;

void cbUDP2Client( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
    DEF::UID uid=0;
    bool     need_ack = false;
    msgin.serial(uid);
    msgin.serial(need_ack);

    uint32 subSize;
    msgin.serial(subSize);
    msgin.lockSubMessage(subSize);

    CMessage msgout;
    msgout.assignFromSubMessage(msgin);

    msgin.unlockSubMessage();

    SessionMgr.sendudp( ClientMgr.findClient(uid), msgout, need_ack );
}

void cbTransport2Client ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
	DEF::RPC_SESSION session;
	bool disconnect;
	msgin.serial(session);
	msgin.serial(disconnect);

	uint32 subSize;
	msgin.serial(subSize);
	msgin.lockSubMessage(subSize);

	CMessage msgout;
	msgout.assignFromSubMessage(msgin);

	msgin.unlockSubMessage();

	SessionMgr.send( session, msgout, disconnect );
}

void cbAddMsgBuffer( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
	DEF::UID uid;
	msgin.serial(uid);

	uint32 subSize;
	msgin.serial(subSize);
	msgin.lockSubMessage(subSize);

	CMessage* pMsg = new(std::nothrow) CMessage();
    if ( pMsg!=NULL ){
        pMsg->assignFromSubMessage(msgin);
    }

	msgin.unlockSubMessage();

	ClientMgr.addMsgBuffer(uid,pMsg);
}

void cbSyncUserLogicServer( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
	UID  uid;
	uint16 logicSId;
	msgin.serial(uid);
	msgin.serial(logicSId);
	ClientMgr.updateConPLS(uid,logicSId);
}

void cbSyncClientSession( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
 	UID  uid;
	MsgSetSession   set_session;
    NLNET::CInetAddress AddrFrom;

	msgin.serial(uid);
	msgin.serial(&set_session);
    msgin.serial(AddrFrom);

	CClient* pClient = ClientMgr.updateClientSession(uid, set_session.session(), set_session.seed());

    if ( pClient!=NULL )
    {
        pClient->addr_from = AddrFrom;
        SessionMgr.addSession( msgin.session(), uid );

        nldebug("ls sync session uid:%"NL_I64"u client_session:%"NL_I64"u", (uint64)uid, (uint64)set_session.session());

        //////  send to client
        uint32 logout_time = ClientMgr.GetClientLogoutTime();

        CMessage msgout("LOGIN");
        msgout.serial(&set_session);
        msgout.serial(logout_time);

        SessionMgr.send( msgin.session(), msgout );
    }
    else
    {
        nldebug("updateClientSession error");
    }
}

void cbGlobleChat( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
    ClientMgr.SendMsgToAll(msgin, false);
}


