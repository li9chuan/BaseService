#ifndef LS_LOGIN_MGR_H
#define LS_LOGIN_MGR_H

//#include <server_share/database/engine/mysql_helper.h>

#include <queue>
#include <map>

#include <game_share/game_def.h>

#include <nel/misc/singleton.h>
#include <nel/misc/random.h>
#include <nel/net/callback_server.h>

namespace LS
{

//struct SLoginRequest
//{
//	DEF::RPC_SESSION   session;
////	DEF::UID       uid;
//	uint32         authType;      
//	std::string user;
//	std::string password;
//
//
//	SLoginRequest():session(0)
//	{}
//};

struct SLoginToken
{
	sint32            uid;
	std::string       token;
	NLMISC::TTicks    time_out;


	SLoginToken():uid(0),time_out(0)
	{}
};

class CLoginMgr : public NLMISC::CSingleton<CLoginMgr>
{
public:

	bool init();

	void CallbackServer( NLNET::CCallbackServer* pCallbackServer ){ _pCallbackServer = pCallbackServer; }
	NLNET::CCallbackServer* CallbackServer() { return _pCallbackServer; }

	bool isFull();
	//bool push( SLoginRequest* pLoginRequest );

	//SLoginRequest* findUser( std::string& user );

	void removeUser( std::string& user );
	bool update();

	uint32 rand()	{ return _Random.rand(); }

	uint32  LoginTokenSize()	{	return _LoginTokenMap.size();	}
	uint32  LoginUIDSize()	{	return _LoginUIDMap.size();	}

	////////////////////////////////

	void AddLoginToken( const sint32 uid, const std::string &token );
	void RemoveLoginToken( const std::string &token );
	SLoginToken* FindLoginToken( const std::string &token );

private:

	//SLoginRequest* pop();

	//typedef std::map<std::string,SLoginRequest*>      TVerifyMap;

	uint32                 _MaxPlayers;
	uint32                 _CurrPlayers;
	NLMISC::TTicks         _CurrTicks;
	uint32                 _LoginConnectMax;
	uint32                 _LoginQueueMax;

	uint32                 _LoginMsgTimeOutTick;
	uint32                 _LoginMsgCheckTick;


	NLNET::CCallbackServer*     _pCallbackServer;
	//MSH::CConnection            _GlobleAccountDB;
	std::queue<std::string>     _LoginQueue;

	//TVerifyMap             _VerifyMap;

	NLMISC::CRandom        _Random;

	////////////////////////

	typedef std::map<std::string, SLoginToken*>      TLoginTokenMap;
	typedef std::map<sint32, SLoginToken*>           TLoginUIDMap;

	TLoginTokenMap          _LoginTokenMap;
	TLoginUIDMap            _LoginUIDMap;
};

#define  LoginMgr  LS::CLoginMgr::instance()

}
#endif // LS_LOGIN_MGR_H

/* End of login_mgr.h */
