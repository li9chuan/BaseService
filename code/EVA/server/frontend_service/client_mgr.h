#ifndef FES_PLAYER_MGR_H
#define FES_PLAYER_MGR_H

#include <nel/misc/singleton.h>
#include "client.h"
/*
 * Keep a list of the players connected to that Frontend. Only map Id
 * to a Connection
 */
namespace FES
{

class CClientMgr : public NLMISC::CSingleton<CClientMgr>
{
public:

	CClientMgr();

	void updateConPLS( DEF::UID uid, uint16 conPLS );
	CClient* updateClientSession( DEF::UID uid, DEF::CLIENT_SESSION client_session, uint32 seed );

	CClient* findClientBySession(DEF::CLIENT_SESSION client_session);
	//CClient* findClientByPID(DEF::PID pid);
    CClient* findClient(DEF::UID uid);
	void setClientPID( DEF::UID uid, DEF::PID pid );
	void removeClient(DEF::UID uid);

    void ClearPLSClient( NLNET::TServiceId conPLS );
    void ClearAllClient();

	void addMsgBuffer( DEF::UID uid, NLNET::CMessage* pMsg );

    uint32 ClientNumber()   { return _ClientMap.size(); }

    const uint32   GetClientOfflineTime()           {  return m_ClientOfflineTime;  }
    const uint32   GetClientOfflineTimeCheck()      {  return m_ClientOfflineTimeCheck;  }
    const uint32   GetClientLogoutTime()            {  return m_ClientLogoutTime;  }
    const uint32   GetClientLogoutTimeCheck()       {  return m_ClientLogoutTimeCheck;  }

    void SendMsgToAll( NLNET::CMessage& msg, bool auto_resend );

private:

	typedef std::map<DEF::UID, CClient*>               TClientMap;
	//typedef std::map<DEF::PID, CClient*>               TPlayerMap;
	typedef std::map<DEF::CLIENT_SESSION, CClient*>    TClientSessionMap;

	TClientMap                             _ClientMap;
	//TPlayerMap                             _PlayerMap;
	TClientSessionMap                      _ClientSessionMap;

    uint32              m_ClientOfflineTime;
    uint32              m_ClientOfflineTimeCheck;

    uint32              m_ClientLogoutTime;
    uint32              m_ClientLogoutTimeCheck;

};

#define ClientMgr FES::CClientMgr::instance()

}
#endif
