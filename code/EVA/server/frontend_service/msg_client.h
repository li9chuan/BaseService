#ifndef MSG_CLIENT_H
#define MSG_CLIENT_H

#include "nel/net/service.h"

class MsgSession;

///////////////////////////      NLNET::TCallbackItem ClientCallbackArray[] 

void onConnectionClient ( NLNET::TSockId from, void *arg );
void onDisconnectClient ( NLNET::TSockId from, void *arg );

void cbLogin (NLNET::CMessage& msgin, NLNET::TSockId from, NLNET::CCallbackNetBase &netbase);

void cbCreatePlayer (NLNET::CMessage& msgin, NLNET::TSockId from, NLNET::CCallbackNetBase &netbase);

//////////////////////////////////////////////////////////////////////////

void cbUDPLogin (NLNET::CMessage& msgin, NLNET::CInetAddress& AddrFrom, MsgSession& ProtoSession);
void cbUDPCreatePlayer (NLNET::CMessage& msgin, NLNET::CInetAddress& AddrFrom, MsgSession& ProtoSession);


//////////////////////////////////////////////////////////////////////////

void TCPForwarding (NLNET::CMessage& msgin, NLNET::TSockId from, NLNET::CCallbackNetBase &netbase);

#endif

