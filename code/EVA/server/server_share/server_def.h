#ifndef SERVICE_SHARED_SERVER_DEF_H
#define SERVICE_SHARED_SERVER_DEF_H

#include <game_share/define_sys.pb.h>
#include <game_share/game_def.h>
#include <game_share/tools.h>
#include <server_share/database/def_db.h>
#include <nel/net/message.h>
#include <nel/net/unified_network.h>
#include <nel/net/callback_server.h>
//#include <nel/misc/log.h>
#include <nel/misc/debug.h>

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
//NLMISC::CLog& Loger();

#define     MaxUDPPacketSize   512

//#ifdef NL_RELEASE
//#	if defined(NL_COMP_VC) && NL_COMP_VC_VERSION >= 71
//#		define DebugLoger __noop
//#	else
//#		define DebugLoger 0&&
//#	endif
//#else // NL_RELEASE
//#	define DebugLoger Loger().displayNL
//#endif // NL_RELEASE

#define  Network NLNET::CUnifiedNetwork::getInstance()
#define  Config  NLNET::IService::getInstance()->ConfigFile

const std::string LogicService   = "PLS";
const std::string EntitiesServer = "EGS";
const std::string Persistent     = "PDS";

/// msg name
const std::string T2C       = "T2C";
const std::string U2C       = "U2C";
const std::string ERR       = "ERR";
const std::string DB_PUT    = "DB_PUT";
const std::string DB_GET    = "DB_GET";
const std::string PLS2DB    = "PLS2DB";
const std::string DB2PLS    = "DB2PLS";

/** Callback function type for message processing
 *
 * msgin contains parameters of the message
 * from is the SockId of the connection, for a client, from is always the same value
 */
typedef void (*TUDPCallback) (NLNET::CMessage &msgin, NLNET::CInetAddress& AddrFrom, MsgSession& ProtoSession);


/// Callback items. See CMsgSocket::update() for an explanation on how the callbacks are called.
typedef struct
{
	/// Key C string. It is a message type name, or "C" for connection or "D" for disconnection
	const char		*Key;
	/// The callback function
	TUDPCallback	Callback;

} TUDPCallbackItem;

inline void SendToClient( DEF::RPC_SESSION session, NLNET::CMessage& msgout, bool disconnect=true )
{
	NLNET::CMessage _msgout(T2C);
	_msgout.serial(session);
	_msgout.serial(disconnect);
	_msgout.serialMessage(msgout);

	Network->send( NLNET::TServiceId(GetSID(session)), _msgout );
}

inline void SendToClient( DEF::RPC_SESSION session, std::string name, google::protobuf::Message* message, bool disconnect=true )
{
	NLNET::CMessage msgout(name);
	msgout.serial(message);
	SendToClient(session,msgout,disconnect);
}

/**
*   @brief 用于向客户端即时推送的消息，仅当客户端UDP端口可用时有效。
*   @param conFES 前端TServiceId。
*   @param uid 账号ID。
*   @param name 消息名。
*   @param auto_resend 发送失败时是否需要网络层尝试自动重发。
*/
inline void SendUDP( NLNET::TServiceId conFES, DEF::UID uid, std::string name, google::protobuf::Message* message, bool auto_resend )
{
    NLNET::CMessage msgsub(name);
    msgsub.serial(message);
    
    NLNET::CMessage msgout(U2C);
    msgout.serial(uid);
    msgout.serial(auto_resend);
    msgout.serialMessage(msgsub);

    if ( NLNET::TServiceId::InvalidId == conFES || conFES.get()==0 )
    {
        nlwarning("------ %s  %d",name.c_str(), conFES.get());
        return;
    }

    Network->send( conFES, msgout );
}

inline void SendUDP( NLNET::TServiceId conFES, DEF::UID uid, NLNET::CMessage& msgout, bool auto_resend )
{
    NLNET::CMessage _msgout(U2C);
    _msgout.serial(uid);
    _msgout.serial(auto_resend);
    _msgout.serialMessage(msgout);

    if ( NLNET::TServiceId::InvalidId == conFES || conFES.get()==0 )
    {
        nlwarning("------ %s  %d", msgout.getName().c_str(), conFES.get());
        return;
    }
    Network->send( conFES, _msgout );
}

inline void AddToClientBuffer( NLNET::TServiceId conFES, DEF::UID uid, NLNET::CMessage& msgout )
{
	NLNET::CMessage _msgout("ADD_MSG_BUF");
	_msgout.serial(uid);
	_msgout.serialMessage(msgout);

	Network->send( conFES, _msgout );
}

inline void AddToClientBuffer( NLNET::TServiceId conFES, DEF::UID uid, std::string name, google::protobuf::Message* message )
{
	NLNET::CMessage msgout(name);
	msgout.serial(message);
	AddToClientBuffer(conFES,uid,msgout);
}

inline google::protobuf::Message* CreateMessage(const std::string& typeName)
{
    google::protobuf::Message* message = NULL;
    const google::protobuf::Descriptor* descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
    if (descriptor)
    {
        const google::protobuf::Message* prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (prototype)
        {
            message = prototype->New();
        }
    }
    return message;
}

#define  SaveToDB( opt, id, stru ) do {\
	DB_OPT _opt = opt;\
	DEF::PID _pid = id;\
	NLNET::CMessage msg_pds( DB_PUT );\
	msg_pds.serial(_opt);\
	msg_pds.serial(_pid);\
	msg_pds.serial(stru);\
	Network->send( Persistent, msg_pds ); \
	} while(0)

#define GetFromDB( opt , id , stru ) do{\
    DB_OPT _opt = opt;\
    DEF::PID _pid = id;\
    NLNET::CMessage msg_pds( DB_GET );\
    msg_pds.serial(_opt);\
    msg_pds.serial( _pid );\
    msg_pds.serial( stru );\
    Network->send( Persistent , msg_pds );\
    }while( 0 )

inline void SendERR( DEF::ERROR_TYPE errNO, DEF::PID pid, NLNET::TServiceId sid, DEF::RPC_SESSION rpc_session=0 )
{
	NLNET::CMessage msgout(rpc_session,ERR);
	msgout.serial(errNO);
	msgout.serial(pid);
	Network->send(sid, msgout);
}

inline void SendERR( DEF::ERROR_TYPE errNO, DEF::PID pid, std::string serviceName, DEF::RPC_SESSION rpc_session=0 )
{
	NLNET::CMessage msgout(rpc_session,ERR);
	msgout.serial(errNO);
	msgout.serial(pid);
	Network->send( serviceName, msgout );
}

inline void SendSyncData( std::string serviceName , uint32 type , DEF::PID pid , sint64 param1=0 , uint64 param2 =0 )
{
    NLNET::CMessage msgout( "SyncPlayerData" );
    msgout.serial( pid );
    msgout.serial( type );
    msgout.serial( param1 );
    msgout.serial( param2 );
    Network->send( serviceName, msgout );
}

inline void SendSyncData( NLNET::TServiceId sid , uint32 type , DEF::PID pid , sint64 param1=0 , uint64 param2 =0 )
{
    NLNET::CMessage msgout( "SyncPlayerData" );
    msgout.serial( pid );
    msgout.serial( type );
    msgout.serial( param1 );
    msgout.serial( param2 );
    Network->send( sid, msgout );
}

//inline void SendERR( DEF::ERROR_TYPE errNO, NLNET::TSockId from, NLNET::CCallbackServer* pCallbackServer )
//{
//	NLNET::CMessage msgout("ERR");
//	msgout.serial(errNO);
//	pCallbackServer->send(msgout, from);
//	pCallbackServer->disconnect (from);
//}

inline void SendErrorToClient( DEF::ERROR_TYPE errNO, DEF::RPC_SESSION session )
{
    NLNET::CMessage msgsub(ERR);
    msgsub.serial(errNO);

    SendToClient( session, msgsub );
}

inline void SendErrorToClient( DEF::ERROR_TYPE errNO, DEF::RPC_SESSION session, sint64 param )
{
    NLNET::CMessage msgsub(ERR);
    msgsub.serial(errNO);
    msgsub.serial(param);

    SendToClient( session, msgsub );
}

inline void SendErrorToClient( NLNET::TServiceId conFES, DEF::UID uid, DEF::ERROR_TYPE errNO )
{
    NLNET::CMessage msgsub(ERR);
    msgsub.serial(errNO);
    SendUDP( conFES, uid, msgsub, true);
}

inline void SendErrorToClient( NLNET::TServiceId conFES, DEF::UID uid, DEF::ERROR_TYPE errNO, sint64 param )
{
    NLNET::CMessage msgsub(ERR);
    msgsub.serial(errNO);
    msgsub.serial(param);
    SendUDP( conFES, uid, msgsub, true);
}

inline void SendErrorToClient( NLNET::TServiceId conFES, DEF::UID uid, DEF::ERROR_TYPE errNO, sint64 param1 , sint64 param2)
{
    NLNET::CMessage msgsub(ERR);
    msgsub.serial(errNO);
    msgsub.serial(param1);
    msgsub.serial(param2);
    SendUDP( conFES, uid, msgsub, true);
}

inline void SendEvent( NLNET::TServiceId sid, DEF::PID pid, TEvent type, sint32 param1 = 0 , sint32 param2 = 0 )
{
    NLNET::CMessage msgout( "TEVENT" );
    uint32 triggerType = (uint32)type;

    msgout.serial( pid );
    msgout.serial( triggerType );
    msgout.serial( param1 );
    msgout.serial( param2 );

    Network->send( sid, msgout );
}


#endif

