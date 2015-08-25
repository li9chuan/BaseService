#include "msg_client.h"
#include "session_mgr.h"
#include "client_mgr.h"
#include "frontend_service.h"
#include <game_share/define_sys.pb.h>
#include <game_share/proto_msg/msg_c2s.pb.h>
#include <server_share/client_msg_desc.h>
#include "msg_forwarding.h"

using namespace NLMISC;
using namespace NLNET;
using namespace FES;
using namespace DEF;
using namespace std;

#define MSG_GET_STRBUFF( PROTO_MSG ) \
	if ( SessionMgr.removeConnectTimeOut(from) )\
	{\
	MsgSession  _ProtoSession; \
	PROTO_MSG _##PROTO_MSG; \
	try \
	{\
		msgin.serial(&_ProtoSession); \
		msgin.serial(&_##PROTO_MSG);  \
	}\
	catch (Exception &) \
	{ \
		netbase.disconnect(from); \
		return; \
	}\
	CClient* pClient = ClientMgr.findClientBySession(_ProtoSession.session()); \
	if ( pClient!=NULL ) { \
		RPC_SESSION __session = SessionMgr.NewSession(CSessionMgr::TCP); \
        from->setAppId( __session );\
		SessionMgr.addSession( __session, pClient->uid, from ); \
		pClient->sendMsgBufferToSock(from);


#define MSG_GET_STRBUFF_END \
        return;\
    }}\
netbase.disconnect(from);


#define  MAKE_MSG_OUT( __msg_instance_name, __msg_name ) \
	CMessage __msg_instance_name(__msg_name);  \
	__msg_instance_name.session(from->appId());


/****************************************************************************
 * Connection callback for a client
 ****************************************************************************/

void onConnectionClient ( NLNET::TSockId from, void *arg )
{
	SessionMgr.addConnectTimeOut(from);
}

void onDisconnectClient ( NLNET::TSockId from, void *arg )
{
	SessionMgr.removeConnectTimeOut(from);
	SessionMgr.removeSession(from->appId());
	from->setAppId(0);
    
//	nlinfo( "A client with unique Id %u has disconnected", uid );
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// SERVICE IMPLEMENTATION /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void cbLogin( NLNET::CMessage& msgin, NLNET::TSockId from, NLNET::CCallbackNetBase &netbase )
{
	if ( SessionMgr.removeConnectTimeOut(from) )
	{
		MsgSession  proto_session;
		MsgLogin    msg_login;
		try 
		{
			msgin.serial(&proto_session);
			msgin.serial(&msg_login);

            from->setAppId( SessionMgr.NewSession(CSessionMgr::TCP) );

            CMessage msgout(from->appId(), "LOGIN");
			msgout.serial (&msg_login);
			Network->send("LS", msgout);

            nldebug("recv client LOGIN. rpc_session:%u token: %s", (uint32)from->appId(), msg_login.token().c_str());
		}
		catch (Exception &)
		{
			netbase.disconnect(from);
			return;
		}
	}
}

void cbCreatePlayer (NLNET::CMessage& msgin, NLNET::TSockId from, NLNET::CCallbackNetBase &netbase)
{
	MSG_GET_STRBUFF(MsgCreatePlayer)
	{
		MAKE_MSG_OUT( msgout, "CP" );
		msgout.serial ( pClient->uid );
		msgout.serial ( pClient->conPLS );
		msgout.serial ( &_MsgCreatePlayer );

        int name_length = _MsgCreatePlayer.name().length();
		if ( name_length<PLAYERNAME_LENGTH && name_length>=PLAYERNAME_LENGTH_MIN )
		{
			Network->send("PDS", msgout);
		}
	}
	MSG_GET_STRBUFF_END
}

void TCPForwarding( NLNET::CMessage& msgin, NLNET::TSockId from, NLNET::CCallbackNetBase &netbase )
{
    if ( SessionMgr.removeConnectTimeOut(from) )
    {
        MsgLeaf* pMsgLeaf = MsgDesc.GetTCPMsgLeaf( msgin.getName() );

        if ( pMsgLeaf!=NULL )
        {
            MsgSession  _ProtoSession;

            try
            {
                msgin.serial(&_ProtoSession);
            }
            catch (Exception &) {
                return;
            }

            CClient* pClient = ClientMgr.findClientBySession(_ProtoSession.session());
            if ( pClient!=NULL ) 
            {
                bool msg_valid = pClient->CheckMsgValid( msgin.buffer()+msgin.getPos(), msgin.length()-msgin.getPos(), 
                    _ProtoSession.index(), _ProtoSession.check() );

                if ( msg_valid )
                {
                    RPC_SESSION rpc_session = SessionMgr.NewSession(CSessionMgr::TCP);
                    from->setAppId(rpc_session);
                    SessionMgr.addSession( rpc_session, pClient->uid, from );
                    pClient->sendMsgBufferToSock(from);

                    if ( ForwardingMsg( pClient, rpc_session, msgin, pMsgLeaf ) )
                    {
                        return;
                    }
                }
            }
        }
    }

    netbase.disconnect(from);
}

