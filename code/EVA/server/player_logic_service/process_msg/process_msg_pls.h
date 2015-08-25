#ifndef PLS_PROCESS_MSG_PLS_H
#define PLS_PROCESS_MSG_PLS_H

#include "nel/net/service.h"

void cbFESConnection  (const std::string &serviceName, NLNET::TServiceId  sid, void *arg);
void cbFESDisconnection  (const std::string &serviceName, NLNET::TServiceId  sid, void *arg);

///////////////////////////      NLNET::TCallbackItem ClientCallbackArray[] 

void cbPlayerLogout  (NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId  sid);

void cbPlayerOnline(NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId sid);
void cbPlayerOffline(NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId sid);

#endif

