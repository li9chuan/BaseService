#ifndef FES_BROADCAST_MSG_H
#define FES_BROADCAST_MSG_H

#include "nel/net/service.h"

///////////////////////////  NLNET::TUnifiedCallbackItem CallbackArray[]

/**
 *	"U2C"	发送UDP包到客户端，用于需要主动推送的消息，有失败几率。
 */
void cbUDP2Client ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId );

/**
 *	"T2C"	转发到客户端
 */
void cbTransport2Client ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId );

/**
 *	"ADD_MSG_BUF"	存入待发送到客户端的消息缓存，在下次客户端连接时同步给客户端
 */
void cbAddMsgBuffer ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId );

/**
 *	"SyncUserPLS"	同步账号使用的逻辑服务器
 */
void cbSyncUserLogicServer ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId );

/**
 *	"SYNC_CLIENT_SESSION"	同步客户端身份Session
 */
void cbSyncClientSession ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId );

void cbGlobleChat ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId );

#endif

