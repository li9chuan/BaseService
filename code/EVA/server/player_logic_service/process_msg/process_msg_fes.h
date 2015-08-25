#ifndef PLS_PROCESS_MSG_FES_H
#define PLS_PROCESS_MSG_FES_H

#include <nel/net/service.h>

///////////////////////////      NLNET::TCallbackItem ClientCallbackArray[] 

void cbSyncChecksum  (NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId  sid);
void cbSyncPlayerInfo  (NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId  sid);
void cbGlobleChat  (NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId  sid);


#endif

