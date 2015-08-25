#ifndef LS_AUTH_MGR_H
#define LS_AUTH_MGR_H

#include <game_share/game_def.h>
#include <nel/misc/singleton.h>
/*#include <server_share/database/engine/mysql_helper.h>*/

namespace LS
{

class CAuthMgr : public NLMISC::CSingleton<CAuthMgr>
{
public:

	bool init();


	TErrorType verifyAccount( TAuthType auth_type, std::string name, std::string pwd );


private:

	//NLNET::CCallbackServer*     _pCallbackServer;
	//MSH::CConnection            _MyAccountDB;


};

#define  AuthMgr  LS::CAuthMgr::instance()

}
#endif // LS_LOGIN_MGR_H

/* End of login_mgr.h */
