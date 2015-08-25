#include "udp_sender.h"
#include <game_share/udp_msg.h>
#include <server_share/client_msg_desc.h>
#include "session_mgr.h"
#include "client_mgr.h"
#include "frontend_service.h"
#include "msg_forwarding.h"

using namespace NLMISC;
using namespace NLNET;
using namespace FES;
using namespace DEF;
using namespace std;


void CUDPSender::update()
{
    TClientResendCD::iterator iter = _ClientResendCD.begin();

    while ( iter!=_ClientResendCD.end() )
    {
        TTicks tick = LocalTime.GetCurrTime();

        if ( tick - iter->second > 5000 )
        {
            iter = _ClientResendCD.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}


void CUDPSender::UDPSendACK( uint32 msg_id, NLNET::CInetAddress& AddrFrom )
{
    try
    {
        udp_command_ack.poke(msg_id,2);
        FrontEndService->SendUDP(udp_command_ack.buffer(), udp_command_ack.length(), AddrFrom);
    }
    catch (const Exception &e)
    {
    }
}



void CUDPSender::Forwarding( NLNET::CMessage& msgin, NLNET::CInetAddress& AddrFrom, bool need_ack )
{
    H_AUTO(UDPForwarding);

    MsgLeaf* pMsgLeaf = NULL;
    MsgSession  _ProtoSession;
    
    try
    {
        pMsgLeaf = MsgDesc.GetUDPMsgLeaf( msgin.getName() );
        msgin.serial(&_ProtoSession);
    }
    catch (Exception &) {
        return;
    }

    if ( pMsgLeaf==NULL )
    {
        if ( TestCallback(msgin, AddrFrom, _ProtoSession) )
        {
            if ( need_ack )
            {
                UDPSendACK(_ProtoSession.index(), AddrFrom);
            }
        }
    }
    else
    {
        CClient* pClient = ClientMgr.findClientBySession(_ProtoSession.session());

        if ( pClient!=NULL ) 
        {
            bool msg_valid = pClient->CheckMsgValid( msgin.buffer()+msgin.getPos(), msgin.length()-msgin.getPos(), 
                _ProtoSession.index(), _ProtoSession.check() );

            if ( msg_valid )
            {
                pClient->addr_from = AddrFrom;
                RPC_SESSION rpc_session = SessionMgr.NewSession(CSessionMgr::UDP);
                SessionMgr.addSession( rpc_session, pClient->uid );

                if ( ForwardingMsg( pClient, rpc_session, msgin, pMsgLeaf ) )
                {
                    if ( need_ack )
                    {
                        UDPSendACK(_ProtoSession.index(), AddrFrom);
                    }
                }
            }
        }
    }
}

CUDPSender::CUDPSender():udp_command_ack(PROTO_CMD_ACK)
{
    uint32  ackidx=0;
    udp_command_ack.serial(ackidx);
}

void CUDPSender::addCallbackArray( const TUDPCallbackItem *callbackarray, sint arraysize )
{
    for (sint i = 0; i < arraysize; i++)
    {
        _CallbackArray.push_back(callbackarray[i]);
    }
}

bool CUDPSender::TestCallback( NLNET::CMessage& msgin, NLNET::CInetAddress& AddrFrom, MsgSession& ProtoSession )
{
    std::string name = msgin.getName ();
    for ( size_t i=0; i<_CallbackArray.size(); ++i )
    {
        if ( name == _CallbackArray[i].Key )
        {
            _CallbackArray[i].Callback(msgin, AddrFrom, ProtoSession);
            return true;
        }
    }
    return false;
}
