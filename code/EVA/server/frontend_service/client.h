#ifndef FES_PLAYER_H
#define FES_PLAYER_H


#include <nel/net/service.h>
#include <nel/misc/mem_stream.h>
#include <game_share/timer.h>
#include <queue>
#include "check.h"

namespace FES
{
/// 缓存的将要推送给客户端的消息
typedef std::queue<NLNET::CMessage*> TMsgBuffer;

class CClient
{
public:

	CClient( DEF::UID _uid );

	~CClient();

	void addMsgBuffer ( NLNET::CMessage* pMsg );
    void sendMsgBufferToSock( NLNET::TSockId hostid );
	void clearMsgBuffer();
    bool clearTimeoutMsgBuffer();

    uint32 FillUDPStream( NLMISC::CMemStream& mem_stream );

    bool CheckMsgValid( const uint8* buffer, uint32 length, uint32 index, uint32 check );
    bool CheckMsgValid( uint32 index, uint32 check );

    void    SetSeed( sint32 seed )  {   m_MsgCheck.SetSeed(seed);   }

    void    UpdateSyncTime( void );

	DEF::UID                uid;
	DEF::PID                pid;
	NLNET::TServiceId       conPLS;
	DEF::CLIENT_SESSION     client_session;
    NLNET::CInetAddress     addr_from;          /// udp

	NLMISC::TTime           last_recv_msg;      ///	最后一次收到消息的时间
    //NLMISC::TTime           last_send_msg;      ///	最后一次发消息的时间

private:

    CTimer                  m_LogoutTimer;

    CMsgCheck               m_MsgCheck;
	TMsgBuffer              m_MsgBuffer;
};


}

#endif
