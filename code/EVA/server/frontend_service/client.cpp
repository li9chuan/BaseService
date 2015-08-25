#include "client.h"
#include "session_mgr.h"
#include "frontend_service.h"
#include "msg_timeout_event.h"
#include "client_mgr.h"
#include <server_share/server_def.h>

// for htonl
#ifdef NL_OS_WINDOWS
#	include <winsock2.h>
#elif defined NL_OS_UNIX
#	include <arpa/inet.h>
#endif


FES::CClient::CClient( DEF::UID _uid ) : uid(_uid),pid(0),
    conPLS(NLNET::TServiceId::InvalidId), client_session(0), last_recv_msg(0),
    state(ONLINE)
{
    last_recv_msg = LocalTime.GetCurrTime();
    m_OfflineTimer.setRemaining( ClientMgr.GetClientOfflineTimeCheck(), new ClientOfflineTimerEvent(this) );
    //m_LogoutTimer.setRemaining( ClientMgr.GetClientLogoutTimeCheck(), new ClientLogoutTimerEvent(this) );
}    

FES::CClient::~CClient()
{
    m_OfflineTimer.reset();
    //m_LogoutTimer.reset();
    clearMsgBuffer();
}

void FES::CClient::addMsgBuffer( NLNET::CMessage* pMsg )
{
    if ( clearTimeoutMsgBuffer() )
    {
        return;
    }

    if ( pMsg!=NULL )
    {
        if ( m_MsgBuffer.size() > SessionMgr.GetMsgBufferMaxNum() )
        {
            delete m_MsgBuffer.front();
            m_MsgBuffer.pop();
        }
        m_MsgBuffer.push(pMsg);
    }
}

void FES::CClient::sendMsgBufferToSock( NLNET::TSockId hostid )
{
    clearTimeoutMsgBuffer();

    while ( m_MsgBuffer.size()>0 )
    {
        FrontEndService->SendToClient( *m_MsgBuffer.front(), hostid );
        delete m_MsgBuffer.front();
        m_MsgBuffer.pop();
    }
}

uint32 FES::CClient::FillUDPStream( NLMISC::CMemStream& mem_stream )
{
    clearTimeoutMsgBuffer();
    uint32 ret_length = 0;
    
    while ( m_MsgBuffer.size()>0 )
    {
        //mem_stream.serialBufferWithSize( (uint8*)m_MsgBuffer.front()->buffer() , m_MsgBuffer.front()->length() );

        uint32 msg_length = m_MsgBuffer.front()->length();
        uint32 net_length = htonl( msg_length );
        mem_stream.serial(net_length);
        mem_stream.serialBuffer( (uint8*)m_MsgBuffer.front()->buffer(), msg_length);

        ret_length += msg_length;
        delete m_MsgBuffer.front();
        m_MsgBuffer.pop();
    }

    return ret_length;
}

void FES::CClient::clearMsgBuffer()
{
    while ( m_MsgBuffer.size()>0 )
    {
        delete m_MsgBuffer.front();
        m_MsgBuffer.pop();
    }
}

void FES::CClient::Online()
{
    if ( state!=ONLINE )
    {
        state = ONLINE;
        m_OfflineTimer.setRemaining( ClientMgr.GetClientOfflineTimeCheck(), new ClientOfflineTimerEvent(this) );
    }

    if ( conPLS != NLNET::TServiceId::InvalidId )
    {
        NLNET::CMessage msg("ONLINE");
        msg.serial(uid);
        msg.serial(pid);
        msg.serial(conPLS);
        Network->send( conPLS , msg );
        Network->send("EGS",msg);
    }

    UpdateSyncTime();
}

void FES::CClient::Offline()
{
    state = OFFLINE;

    NLNET::CMessage  msgout("OFFLINE");
    msgout.serial(uid);
    msgout.serial(pid);
    Network->send("EGS",msgout);
}

void FES::CClient::UpdateSyncTime( void )
{
    uint32 unCurTime = NLMISC::CTime::getSecondsSince1970();

    NLNET::CMessage SendPack("SyncTime");
    SendPack.serial( unCurTime );

    SessionMgr.sendudp( this , SendPack , true );
}

bool FES::CClient::clearTimeoutMsgBuffer()
{
    NLMISC::TTime  timeout = LocalTime.GetCurrTime() - last_recv_msg;

    if ( timeout > SessionMgr.GetMsgBufferTimeOut() )
    {
        clearMsgBuffer();
        return true;
    }
    return false;
}

NLMISC::CVariable<bool>	VAR_CHECK_MSG("fes", "CheckMsg", "", false, 0, true);

bool FES::CClient::CheckMsgValid( const uint8* buffer, uint32 length, uint32 index, uint32 check )
{
    if ( !VAR_CHECK_MSG.get() )    {    return true;    }

    uint32 crc = 0;

    if ( length>0 )
    {
        crc = crc32( crc, buffer, length );
        check -= crc;
    }

    return CheckMsgValid( index, check );
}

bool FES::CClient::CheckMsgValid( uint32 index, uint32 check )
{
    if ( m_MsgCheck.Check( index, check ) )
    {
        return true;
    }

    return false;
}

