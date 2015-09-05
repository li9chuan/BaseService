#include "stdpch.h"
#include "msg_timeout_event.h"
#include "session_mgr.h"
#include "client_mgr.h"
#include "frontend_service.h"
#include <server_share/server_def.h>

namespace FES
{
	using namespace NLMISC;

	NL_INSTANCE_COUNTER_IMPL(CMsgTimeoutTimerEvent);
    NL_INSTANCE_COUNTER_IMPL(ClientLogoutTimerEvent);

	CMsgTimeoutTimerEvent::CMsgTimeoutTimerEvent():call_count(0)
	{
	}

	void CMsgTimeoutTimerEvent::timerCallback( CTimer *owner )
	{
		// H_AUTO(CPlayerOfflineTimerEvent);

        ++call_count;

        if ( SessionMgr.ReSendMsgIdx.get(resend_idx) ) 
        {
            delete owner;
            return;
        }

        try
        {
            FrontEndService->SendUDP(mem_stream.buffer(), mem_stream.length(), address);
            //nldebug("resend:%d    count:%d", resend_idx, call_count);
        }
        catch (const NLMISC::Exception &e)
        {
            nldebug("resend fail:%d    count:%d", resend_idx, call_count);
        }

        switch (call_count)
        {
        case 1:
            owner->setRemaining( 60, this );
            break;
        case 2:
            owner->setRemaining( 60, this );
            break;
        case 3:
            owner->setRemaining( 80, this );
            break;
        case 4:
            owner->setRemaining( 80, this );
            break;
        default:
            delete owner;
            break;
        }
	}

    void ClientLogoutTimerEvent::timerCallback( CTimer *owner )
    {
        TTime _time = LocalTime.GetCurrTime() - m_Client->last_recv_msg;

        if ( _time >= ClientMgr.GetClientLogoutTime() )
        {
            if ( m_Client->conPLS!=NLNET::TServiceId::InvalidId )
            {
                NLNET::CMessage  msgout("LOGOUT");
                msgout.serial(m_Client->uid);
                Network->send(m_Client->conPLS, msgout);
            }

            return;
        }

        owner->setRemaining( ClientMgr.GetClientLogoutTimeCheck(), this, 0xf );
    }


}

