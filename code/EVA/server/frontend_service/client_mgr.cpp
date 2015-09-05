#include "client_mgr.h"
#include <game_share/utils.h>
#include <game_share/tools.h>
#include <server_share/server_def.h>
#include <server_share/msg_struct/msg_struct_login.h>

#include "session_mgr.h"

using namespace NLNET;
using namespace NLMISC;
using namespace std;

extern NLMISC::CVariable<uint32>	VAR_PLAYER_NUM;

namespace FES
{

CClientMgr::CClientMgr()
{
    m_ClientOfflineTime         = Config.getVar ("ClientOfflineTime").asInt();
    m_ClientOfflineTimeCheck    = Config.getVar ("ClientOfflineTimeCheck").asInt();

    m_ClientLogoutTime          = Config.getVar ("ClientLogoutTime").asInt();
    m_ClientLogoutTimeCheck     = Config.getVar ("ClientLogoutTimeCheck").asInt();
}

void CClientMgr::updateConPLS( DEF::UID uid, uint16 conPLS)
{
	CClient* pClient = findClient(uid);

	if ( pClient != NULL )
	{
        pClient->conPLS.set(conPLS);
	}
}

CClient* CClientMgr::updateClientSession( DEF::UID uid, DEF::CLIENT_SESSION client_session, uint32 seed )
{
	CClient* pClient = findClient(uid);

	if ( pClient == NULL )
	{
		pClient = new(std::nothrow) CClient(uid);
        if ( pClient==NULL ) {
            return NULL;
        }
        _ClientMap[uid] = pClient;
        VAR_PLAYER_NUM = ClientNumber();
	}
    else
    {
        _ClientSessionMap.erase( pClient->client_session );
    }

    pair<TClientSessionMap::iterator,bool> res;
	res = _ClientSessionMap.insert( make_pair(client_session,pClient) );

    if ( res.second )
    {
	    pClient->client_session = client_session;
	    pClient->last_recv_msg  = LocalTime.GetCurrTime();
        //pClient->last_send_msg  = LocalTime.GetCurrTime();
	    pClient->clearMsgBuffer();
        pClient->SetSeed(seed);
        pClient->Online();

        CMessage  msg_out("CleanUpClient");
        msg_out.serial(uid);
        CUnifiedNetwork::getInstance()->send( "FES", msg_out, false );
    }
    else
    {
        _ClientMap.erase( uid );
        SAFE_DELETE(pClient);
    }

    nldebug("SessionMapSize: %d   ClientMapSize: %d", (sint32)_ClientSessionMap.size(), (sint32)_ClientMap.size() );
    return pClient;
}

CClient* CClientMgr::findClient(DEF::UID uid)
{
	TClientMap::iterator iter = _ClientMap.find( uid );

	if(iter == _ClientMap.end())
	{
		return NULL;
	}
	else
	{
		//iter->second->last_send_msg = LocalTime.GetCurrTime();
		return iter->second;
	}
}

CClient* CClientMgr::findClientBySession( DEF::CLIENT_SESSION client_session )
{
	TClientSessionMap::iterator iter = _ClientSessionMap.find( client_session );

	if(iter == _ClientSessionMap.end())
	{
		return NULL;
	}
	else
	{
        if ( iter->second->state!=CClient::ONLINE )
        {
            iter->second->Online();
        }

		iter->second->last_recv_msg = LocalTime.GetCurrTime();
		return iter->second;
	}
}

//CClient* CClientMgr::findClientByPID( DEF::PID pid )
//{
//	CClient* pClient = NULL;
//	TPlayerMap::iterator iter = _PlayerMap.find(pid);
//
//	if ( iter != _PlayerMap.end() )
//	{
//		pClient = iter->second;
//	}
//	return pClient;
//}

void CClientMgr::setClientPID( DEF::UID uid, DEF::PID pid )
{
    CClient* pClient = findClient(uid);
    if ( pClient != NULL )
    {
        pClient->pid = pid;
        //_PlayerMap[pid] = pClient;
    }
}

void CClientMgr::removeClient(DEF::UID uid)
{
    nldebug("uid: %"NL_I64"u", (uint64)uid);
	CClient* pClient = findClient(uid);
	if ( pClient != NULL )
	{
		_ClientMap.erase(uid);
		//_PlayerMap.erase(pClient->pid);
		_ClientSessionMap.erase(pClient->client_session);

		delete pClient;
        VAR_PLAYER_NUM = ClientNumber();
	}

    nldebug("SessionMapSize: %d   ClientMapSize: %d", (sint32)_ClientSessionMap.size(), (sint32)_ClientMap.size() );
}

void CClientMgr::addMsgBuffer( DEF::UID uid, NLNET::CMessage* pMsg )
{
	CClient* pClient = findClient(uid);

	if ( pClient!=NULL )
	{
		pClient->addMsgBuffer(pMsg);
	}
}

void CClientMgr::ClearPLSClient( NLNET::TServiceId conPLS )
{
    TClientMap::iterator iter = _ClientMap.begin();

    while( iter!=_ClientMap.end() )
    {
        if ( iter->second->conPLS==conPLS )
        {
            //_PlayerMap.erase(iter->second->pid);
            _ClientSessionMap.erase(iter->second->client_session);
            delete iter->second;
            _ClientMap.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
}

void CClientMgr::ClearAllClient()
{
    TClientMap::iterator iter = _ClientMap.begin();

    while( iter!=_ClientMap.end() )
    {
        delete iter->second;
        ++iter;
    }
    _ClientMap.clear();
    //_PlayerMap.clear();
    _ClientSessionMap.clear();
}

void CClientMgr::SendMsgToAll( NLNET::CMessage& msg, bool auto_resend )
{
    TClientMap::iterator iter = _ClientMap.begin();

    while( iter!=_ClientMap.end() )
    {
        SessionMgr.sendudp( iter->second, msg, auto_resend );
        ++iter;
    }
}






}

