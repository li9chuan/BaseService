#include "auth_mgr.h"
#include <nel/net/service.h>
#include <nel/misc/md5.h>
//
// Namespaces
//

using namespace std;
using namespace NLMISC;
using namespace NLNET;
using namespace LS;
using namespace DEF;
//using namespace MSH;



bool LS::CAuthMgr::init()
{
	string DatabaseName = IService::getInstance()->ConfigFile.getVar("DatabaseName").asString ();
	string DatabaseHost = IService::getInstance()->ConfigFile.getVar("DatabaseHost").asString ();
	string DatabaseLogin = IService::getInstance()->ConfigFile.getVar("DatabaseLogin").asString ();
	string DatabasePassword = IService::getInstance()->ConfigFile.getVar("DatabasePassword").asString ();

	nlinfo("DatabaseName '%s'", DatabaseName.c_str());
	nlinfo("DatabaseHost '%s'", DatabaseHost.c_str());
	nlinfo("DatabaseLogin '%s'", DatabaseLogin.c_str());
	nlinfo("DatabasePassword '%s'", DatabasePassword.c_str());

	//_MyAccountDB.connect( DatabaseHost, DatabaseLogin, DatabasePassword, DatabaseName );
	return true;
}

TErrorType LS::CAuthMgr::verifyAccount( TAuthType auth_type, std::string name, std::string pwd )
{
 	TErrorType res = NO_AUTH_TYPE;
// 
// 	switch ( auth_type )
// 	{
// 	case AUTH_TYPE_MY:
// 		{
// 			CSString query;
// 			query << "SELECT f_password FROM t_account WHERE ";
// 			query << "f_account_name = '"+MSH::escapeString(name, _MyAccountDB)+"'";
// 			//WARN_IF(!_MyAccountDB.query(query), "invitedAccountByName : failed request in database", CallbackServer()->disconnect(pLoginRequest->sockId); break;);
// 
// 			if ( !_MyAccountDB.query(query) )
// 			{
// 				res = QUERY_FAILED;
// 				break;
// 			}
// 
// 			auto_ptr<CStoreResult> result = auto_ptr<CStoreResult>(_MyAccountDB.storeResult());
// 
// 			if ( result->getNumRows() == 0 )
// 			{
// 				res = NO_ACCOUNT;
// 				break;
// 			}
// 
// 			result->fetchRow();
// 			std::string password;
// 			result->getField(0, password);
// 
// 			if ( pwd == password )
// 			{
// 				res = SUCESS;
// 			}
// 			else
// 			{
// 				res = PWD_ERROR;
// 			}
// 			break;
// 		}
// 	case AUTH_TYPE_BBS:
// 		{
// 			res = NO_AUTH_TYPE;
// 			break;
// 		}
// 	default:
// 		{
// 			res = NO_AUTH_TYPE;
// 			break;
// 		}
// 	}

	return res;
}
