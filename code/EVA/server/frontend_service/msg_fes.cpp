
#include "msg_fes.h"
#include "client_mgr.h"
#include "session_mgr.h"
#include "frontend_service.h"

#include <server_share/msg_struct/msg_struct_login.h>
#include <server_share/server_def.h>

using namespace NLNET;
using namespace FES;
using namespace DEF;

void cbLSConnection (const std::string &serviceName, TServiceId  sid, void *arg)
{
	MSG_STR::CFrontEnd frontEnd;
	frontEnd.ShardId = FrontEndService->ConfigFile.getVar ("ShardId").asInt();
	frontEnd.SId     = TServiceId(FrontEndService->ConfigFile.getVar ("SId").asInt());
	frontEnd.Port    = FrontEndService->ConfigFile.getVar ("INetPort").asInt();
	frontEnd.ClientMax  = FrontEndService->ConfigFile.getVar ("ClientMax").asInt();
	frontEnd.CurrClientCount = ClientMgr.ClientNumber();
    frontEnd.HostName = IService::getInstance ()->getHostName();
    frontEnd.UnifiedName = IService::getInstance ()->getServiceUnifiedName();

	CMessage        msgout("SET_FES_INFO");
	msgout.serial(frontEnd);
	NLNET::CUnifiedNetwork::getInstance()->send (sid, msgout);
}

void cbPLSDisconnection (const std::string &serviceName, TServiceId  sid, void *arg)
{
    ClientMgr.ClearPLSClient(sid);
}

void cbEGSDisconnection (const std::string &serviceName, TServiceId  sid, void *arg)
{
    ClientMgr.ClearAllClient();
}

//void cbErrToClient ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
//{
//	ERROR_TYPE      errNO;
//	RPC_SESSION     session;
//	msgin.serial(errNO);
//	msgin.serial(session);
//
//	CMessage        msgout("ERR");
//	msgout.serial(errNO);
//	SessionMgr.send( session, msgout );
//}

void cbSetUserPID( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
    UID uid = 0;
    PID pid = 0;
    msgin.serial(uid);
    msgin.serial(pid);

    ClientMgr.setClientPID(uid,pid);
}

void cbCleanUpClient( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
    DEF::UID uid=0;
    msgin.serial(uid);

    ClientMgr.removeClient(uid);
}










