#include "login_mgr.h"
#include "fes_mgr.h"
#include "auth_mgr.h"
#include "sender.h"

#include <nel/net/service.h>
#include <nel/misc/time_nl.h>

//
// Namespaces
//

using namespace std;
using namespace NLMISC;
using namespace NLNET;
using namespace LS;
using namespace DEF;
//using namespace MSH;

bool CLoginMgr::init()
{
	_LoginConnectMax     = Config.getVar ("LoginConnectMax").asInt();
	_LoginQueueMax       = Config.getVar ("LoginQueueMax").asInt();
	_LoginMsgTimeOutTick = Config.getVar ("LoginMsgTimeOutTick").asInt();
	_LoginMsgCheckTick   = Config.getVar ("LoginMsgCheckTick").asInt();

	_MaxPlayers  = 0;
	_CurrPlayers = 0;

	_Random.srand(CTime::getSecondsSince1970());

    _CurrTicks = CTime::getLocalTime();

	return true;
}

bool CLoginMgr::isFull()
{
	bool res = false;

	if ( _LoginQueue.size() < _LoginQueueMax )
	{
		if ( _LoginQueue.size()+_CurrPlayers < _MaxPlayers  )
		{
			res = true;
		}
	}

	return res;
}

//SLoginRequest* CLoginMgr::findUser( string& user )
//{
//	SLoginRequest* pLoginRequest = NULL;
//	TVerifyMap::iterator iter = _VerifyMap.find(user);
//
//	if ( iter != _VerifyMap.end() )
//	{
//		pLoginRequest = iter->second;
//	}
//
//	return pLoginRequest;
//}
//
//void CLoginMgr::removeUser( std::string& user )
//{
//	TVerifyMap::iterator iter_verify = _VerifyMap.find(user);
//	if ( iter_verify != _VerifyMap.end() )
//	{
//		delete iter_verify->second;
//		_VerifyMap.erase(iter_verify);
//	}
//}
//
//bool CLoginMgr::push( SLoginRequest* pLoginRequest )
//{
//	bool res = false;
//	
//	if ( pLoginRequest != NULL )
//	{
//		std::pair<TVerifyMap::iterator,bool> ins_verify;
//		ins_verify = _VerifyMap.insert( std::make_pair(pLoginRequest->user,pLoginRequest) );
//
//		if ( ins_verify.second )
//		{
//			_LoginQueue.push(pLoginRequest->user);
//			res = true;
//		}
//	}
//	return res;
//}

bool CLoginMgr::update()
{
	NLMISC::TTime curr_ticks = CTime::getLocalTime();

	if ( curr_ticks-_CurrTicks > _LoginMsgCheckTick )
	{
		TLoginTokenMap::iterator iter=_LoginTokenMap.begin();

		while ( iter!=_LoginTokenMap.end() )
		{
			if ( curr_ticks - iter->second->time_out > _LoginMsgTimeOutTick )
			{
				SLoginToken* pLoginToken = iter->second;
                nldebug("remove token %s , %llu" , pLoginToken->token.c_str() ,pLoginToken->uid );

				_LoginTokenMap.erase(iter++);
				_LoginUIDMap.erase(pLoginToken->uid);
				delete pLoginToken;
			}
			else
			{
				++iter;
			}
		}

		_CurrTicks = curr_ticks;
	}

	return false;
}


//SLoginRequest* CLoginMgr::pop() 
//{
//	SLoginRequest* pLoginRequest = NULL;
//
//	if ( !_LoginQueue.empty() )
//	{
//		string& user = _LoginQueue.front(); 
//		TVerifyMap::iterator iter = _VerifyMap.find( user );
//		if ( iter != _VerifyMap.end() )
//		{
//			pLoginRequest = iter->second;
//		}
//		_LoginQueue.pop();
//	}
//
//	return pLoginRequest;
//}

void CLoginMgr::AddLoginToken( const sint32 uid, const std::string &token )
{
	TLoginUIDMap::iterator iter = _LoginUIDMap.find( uid );

	if( iter != _LoginUIDMap.end() )
	{
        nldebug(" remove token %s , %llu" , iter->second->token.c_str(), iter->second->uid );
		_LoginTokenMap.erase( iter->second->token );
		iter->second->token = token;
        iter->second->time_out = CTime::getLocalTime();
		

        std::pair< TLoginTokenMap::iterator , bool  > _result = _LoginTokenMap.insert( make_pair(token, iter->second) );

        if( !_result.second )
        {
            nldebug( " AddLoginToken [ insert token faile ]" );
        }
	}
	else
	{
		SLoginToken* pLoginToken = new SLoginToken();
		pLoginToken->time_out = CTime::getLocalTime();
		pLoginToken->uid      = uid;
		pLoginToken->token    = token;
		_LoginUIDMap.insert( make_pair( uid, pLoginToken ) );
		//_LoginTokenMap.insert( make_pair(token, pLoginToken) );

        std::pair< TLoginTokenMap::iterator , bool  > _result = _LoginTokenMap.insert(make_pair(token, pLoginToken));

        if( !_result.second )
        {
            nldebug( " AddLoginToken [ insert token faile ]" );
        }
	}

    nldebug("uid:%d  token:%s",uid,token.c_str());
}

void LS::CLoginMgr::RemoveLoginToken( const std::string &token )
{
	TLoginTokenMap::iterator iter = _LoginTokenMap.find(token);

	if ( iter!=_LoginTokenMap.end() )
	{
		SLoginToken* pLoginToken = iter->second;
        nldebug("token:%s, %llu ",pLoginToken->token.c_str() , pLoginToken->uid );

		_LoginUIDMap.erase(pLoginToken->uid);
		_LoginTokenMap.erase(iter);
		delete pLoginToken;
	}
}

SLoginToken* LS::CLoginMgr::FindLoginToken( const std::string &token )
{
	SLoginToken* pLoginToken = NULL;
	TLoginTokenMap::iterator iter = _LoginTokenMap.find(token);

	if ( iter!=_LoginTokenMap.end() )
	{
		pLoginToken = iter->second;
	}

    if( NULL == pLoginToken )
    {
        nldebug( " not find token;%s" , token.c_str() );
    }

    nldebug("token:%s",token.c_str());
	return pLoginToken;
}
