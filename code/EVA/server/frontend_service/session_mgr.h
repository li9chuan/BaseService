#ifndef FES_SESSION_MGR_H
#define FES_SESSION_MGR_H

#include <list>

#include <nel/misc/random.h>
#include <nel/misc/singleton.h>
#include <nel/misc/bit_set.h>
#include <nel/net/service.h>
#include <game_share/game_def.h>

class MsgSession;

namespace FES
{

#define MAX_RECV_ACK_IDX  (0x1FFFFFF)
//#define INVALID_ACK_IDX   (MAX_RECV_ACK_IDX+1)

class CSessionData
{
public:
	NLNET::TSockId sock_id;
	NLMISC::TTicks ticks;
    DEF::UID       uid;
};

class CClient;

class CSessionMgr : public NLMISC::CSingleton<CSessionMgr>
{
public:
    enum Protocol {  UDP,  TCP  };

public:

	CSessionMgr():_SessionIdx(0),_ACKIdx(0)
	{}

	void init( );

    

	bool send( DEF::RPC_SESSION session, NLNET::CMessage& msgout, bool disconnect=true );
    bool sendudp( CClient* pClient, NLNET::CMessage& msgout, bool need_ack );
    
    
    ///  UDP控制指令
    void RecvACK( DEF::ACK_IDX ack );
    void RecvHeartBeat( MsgSession& proto_session, NLNET::CInetAddress& AddrFrom );

	bool addConnectTimeOut( NLNET::TSockId sock );
	bool removeConnectTimeOut( NLNET::TSockId sock );

	CSessionData* findSession( DEF::RPC_SESSION session );

    //DEF::UID      GetUID( DEF::RPC_SESSION session );

    inline DEF::RPC_SESSION NewSession( Protocol protocol );

	bool addSession( DEF::RPC_SESSION session, DEF::UID uid, NLNET::TSockId sock_id=NLNET::InvalidSockId );

	void removeSession( DEF::RPC_SESSION session ) { _sessionMap.erase( session ); }

	void update( NLMISC::TTicks ticks );

    NLMISC::CBitSet   ReSendMsgIdx;

    uint32 GetMsgBufferTimeOut()    { return _MsgBufferTimeOut; }
    uint32 GetMsgBufferMaxNum()     { return _MsgBufferMaxNum; }

private:

    //DEF::ACK_IDX _sendudp( NLMISC::CMemStream& mem_stream, NLNET::CInetAddress& address );

	typedef std::map< DEF::RPC_SESSION, CSessionData >      TSessionMap;
	typedef std::map< NLNET::TSockId, NLMISC::TTicks >      TConnectMap;

	NLMISC::CRandom   _Random;

	uint32            _AcceptConnectMax;
	uint32		      _ConnectTimeOut;
	uint32		      _ConnectUpdateInterval;	///	update一次的间隔时间
	uint32		      _SessionTimeOut;
	uint32		      _SessionUpdateInterval;
	NLMISC::TTicks    _curTicks;
	NLMISC::TTicks    _preTicks;
	NLMISC::TTicks    _preConnectTimeOutTicks;
	TSessionMap       _sessionMap;
	TConnectMap       _connectMap;

    DEF::ACK_IDX      _ACKIdx;
	uint32            _SessionIdx;
	uint8             _Sid;
	NLNET::CCallbackServer* _Clients;
    uint32            _MsgBufferTimeOut;
    uint32            _MsgBufferMaxNum;
};

DEF::RPC_SESSION CSessionMgr::NewSession( Protocol protocol )
{
    ///   flag(1) session(24)  sid(7)
    ///   flag   是tcp还是udp， 1:TCP  0:UDP

    ++_SessionIdx;
    DEF::RPC_SESSION  session = _SessionIdx << 7;
    session |= _Sid;

    switch (protocol)
    {
    case FES::CSessionMgr::UDP:
        session &= ~RPC_SESSION_TCP_FLAG;
        break;
    case FES::CSessionMgr::TCP:
        session |= RPC_SESSION_TCP_FLAG;
        break;
    default:
        break;
    }
    
    return session;
}

}

#define  SessionMgr  FES::CSessionMgr::instance()

#endif	//	FES_SESSION_MGR_H

