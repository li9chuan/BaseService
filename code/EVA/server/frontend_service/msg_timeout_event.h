#ifndef FES_MSG_TIMEOUT_EVENT_H
#define FES_MSG_TIMEOUT_EVENT_H

#include <game_share/timer.h>
#include <game_share/udp_msg.h>

namespace FES
{
    class CClient;

	// ***************************************************************************
	/// 消息超时，重发或放弃
	class CMsgTimeoutTimerEvent : public CTimerEvent
	{
		NL_INSTANCE_COUNTER_DECL(CMsgTimeoutTimerEvent);
	public:

		CMsgTimeoutTimerEvent();
		void timerCallback(CTimer *owner);

        UDPToClientMsg          mem_stream;
        NLNET::CInetAddress     address;
        DEF::ACK_IDX            resend_idx;
        uint32                  call_count;
	};

    // ***************************************************************************
    /// 客户端离线检查
    class ClientOfflineTimerEvent : public CTimerEvent
    {
        NL_INSTANCE_COUNTER_DECL(ClientOfflineTimerEvent);
    public:

        ClientOfflineTimerEvent(CClient *pClient)   { m_Client = pClient; }
        void timerCallback(CTimer *owner);

    private:
        CClient*    m_Client;
    };

    class ClientLogoutTimerEvent : public CTimerEvent
    {
        NL_INSTANCE_COUNTER_DECL(ClientLogoutTimerEvent);
    public:

        ClientLogoutTimerEvent(CClient *pClient)    { m_Client = pClient; }
        void timerCallback(CTimer *owner);

    private:
        CClient*    m_Client;
    };
}


#endif		//	FES_MSG_TIMEOUT_EVENT_H
