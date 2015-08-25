#ifndef FES_UDP_SENDER_H
#define FES_UDP_SENDER_H

#include <nel/net/service.h>
#include <game_share/udp_msg.h>
#include <server_share/server_def.h>

#include <map>

class CUDPSender
{
public:

    static CUDPSender& instance ()
    {
        static CUDPSender inst;
        return inst;
    }

    void Forwarding (NLNET::CMessage& msgin, NLNET::CInetAddress& AddrFrom, bool need_ack);
    void addCallbackArray (const TUDPCallbackItem *callbackarray, sint arraysize);

    void update();

    bool IsAddrRecv( NLNET::CInetAddress& AddrFrom )  { return _ClientResendCD.find(AddrFrom)==_ClientResendCD.end(); }
    void SetAddrRecvCDStart( NLNET::CInetAddress& AddrFrom )  { _ClientResendCD[AddrFrom]=LocalTime.GetCurrTime(); }


private:

    void UDPSendACK ( uint32 msg_id, NLNET::CInetAddress& AddrFrom );
    bool TestCallback (NLNET::CMessage& msgin, NLNET::CInetAddress& AddrFrom, MsgSession& ProtoSession);

    CUDPSender();
    CUDPSender(const CUDPSender &);  
    CUDPSender & operator = (const CUDPSender &);  

    UDPCommand udp_command_ack;

    std::vector<TUDPCallbackItem>	_CallbackArray;
    std::map<std::string, TUDPCallback>   m_Callback;

    typedef std::map<NLNET::CInetAddress,NLMISC::TTicks>   TClientResendCD;
    TClientResendCD                    _ClientResendCD;
};

#define UDPSender CUDPSender::instance()



#endif
