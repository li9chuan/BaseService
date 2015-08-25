#ifndef MSG_CLIENT_BASE_H
#define MSG_CLIENT_BASE_H

#include <server_share/server_def.h>
#include <server_share/msg_leaf.h>
#include "client.h"

bool ForwardingMsg( FES::CClient* pClient, DEF::RPC_SESSION rpc_session, NLNET::CMessage& msgin, MsgLeaf* pMsgLeaf );

#endif
