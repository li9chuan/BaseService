#include <map>
#include <vector>

#include <nel/misc/log.h>
#include <nel/misc/debug.h>
#include <nel/misc/displayer.h>
#include <nel/misc/config_file.h>

#include <nel/net/service.h>
#include <game_share/object_pool.h>
#include <game_share/proto_msg/msg_c2s.pb.h>
#include "auth_mgr.h"
#include "login_mgr.h"
#include "fes_mgr.h"
#include "sender.h"

using namespace std;
using namespace NLMISC;
using namespace NLNET;
using namespace DEF;
using namespace LS;

void cbFesInfo (CMessage &msgin, const std::string &serviceName, TServiceId sid)
{
    try
    {
        MSG_STR::CFrontEnd fes;
        msgin.serial(fes);
        fes.ConnectId = sid;
        FesMgr.addFes(fes);

        nldebug( "FES Info. host:%s fes:%s sid:%d", fes.HostName.c_str(), fes.UnifiedName.c_str(), sid.get() );
    }
    catch (const NLMISC::EInvalidDataStream&)
    {
        nldebug( "FES Info. EInvalidDataStream Errorrrr sid:%d", sid.get() );
    }
	catch (const NLMISC::EStreamOverflow&)
    {
        nldebug( "FES Info. EStreamOverflow Errorrrr sid:%d", sid.get() );
    }
	catch (const NLMISC::EStream&)
    {
        nldebug( "FES Info. EStream Errorrrr sid:%d", sid.get() );
    }

	

}

void cbFesCurrPlayers (CMessage &msgin, const std::string &serviceName, TServiceId sid)
{
	//MSG_STR::CFrontEnd* pFes = FesMgr.findFes(sid);
	//if ( pFes != NULL )
	//{
	//	msgin.serial(pFes->CurrClientCount);
	//	FesMgr.resetCurrPlayers();
	//}
}

void cbClientRequestVLP(CMessage &msgin, const std::string &serviceName, TServiceId sid)
{
	//std::string  strbuf;

	//try
	//{
	//	msgin.serial(strbuf);

	//	if ( LoginMgr.isFull() )
	//	{
	//		SendErrorToClient( SERVER_FULL, msgin.session() );
	//		return;
	//	}

	//	MsgVLP msg_vlp;
	//	if ( msg_vlp.ParseFromString(strbuf) )
	//	{
	//		string username = msg_vlp.username();

	//		SLoginRequest* pLoginRequest = LoginMgr.findUser(username);
	//		if ( pLoginRequest != NULL )
	//		{
	//			return;
	//		}

	//		pLoginRequest = new SLoginRequest;
	//		pLoginRequest->user     = username;
	//		pLoginRequest->password = msg_vlp.password();
	//		pLoginRequest->authType = msg_vlp.auth_type();
	//		pLoginRequest->session  = msgin.session();


	//		if ( !LoginMgr.push( pLoginRequest ) )
	//		{
	//			delete pLoginRequest;
	//			pLoginRequest = NULL;
	//		}
	//	}
	//}
	//catch (Exception &)
	//{
	//	nlwarning ("Error recv msg ClientVerifyLoginPassword.");
	//	return;
	//}
}

void cbLogin(CMessage &msgin, const std::string &serviceName, TServiceId sid)
{
	MsgLogin msg_login;
    NLNET::CInetAddress AddrFrom;

	try
	{
		msgin.serial(&msg_login);
        
		SLoginToken* pLoginToken = LoginMgr.FindLoginToken(msg_login.token());
		if ( pLoginToken != NULL )
		{
            msgin.serial(AddrFrom);

			Sender.SyncSession( msgin.session(), pLoginToken->uid, sid, AddrFrom );
			// LoginMgr.RemoveLoginToken(msg_login.token());
		}
	}
	catch (Exception &)
	{
		nlwarning ("Error recv msg Login.");
		return;
	}
}


void cbFESConnection (const std::string &serviceName, TServiceId  sid, void *arg)
{
	//CMessage        msgout("GET_FES_INFO");
	//NLNET::CUnifiedNetwork::getInstance()->send (sid, msgout);
    nldebug("FES Connection. sid:%d", sid.get());
}

void cbFESDisConnection (const std::string &serviceName, TServiceId  sid, void *arg)
{
    MSG_STR::CFrontEnd* pFES = FesMgr.findFes(sid);

    if ( pFES!=NULL )
    {
        nldebug( "FES DisConnection. host:%s fes:%s sid:%d", pFES->HostName.c_str(), pFES->UnifiedName.c_str(), sid.get() );
    }
    else
    {
        nldebug( "Errorrrrr FES DisConnection. sid:%d", sid.get() );
    }
    
	FesMgr.removeFes(sid);
}
