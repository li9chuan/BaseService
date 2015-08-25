#ifndef MSG_FES_H
#define MSG_FES_H

#include "nel/net/service.h"

///////////////////////////  NLNET::TUnifiedCallbackItem CallbackArray[]

void cbLSConnection (const std::string &serviceName, NLNET::TServiceId  sid, void *arg);
void cbPLSDisconnection (const std::string &serviceName, NLNET::TServiceId  sid, void *arg);
void cbEGSDisconnection (const std::string &serviceName, NLNET::TServiceId  sid, void *arg);

//void cbErr ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId );
void cbSetUserPID ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId );
void cbCleanUpClient ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId );
void cbPlayerOnline ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId );




#endif

