#ifndef NL_CONNECTION_CLIENT_H
#define NL_CONNECTION_CLIENT_H

#include <nel/net/service.h>

void cbFesInfo (CMessage &msgin, const std::string &serviceName, TServiceId sid);

void cbFesCurrPlayers (CMessage &msgin, const std::string &serviceName, TServiceId sid);

void cbClientRequestVLP(CMessage &msgin, const std::string &serviceName, TServiceId sid);

void cbLogin(CMessage &msgin, const std::string &serviceName, TServiceId sid);

void cbFESConnection (const std::string &serviceName, TServiceId  sid, void *arg);

void cbFESDisConnection (const std::string &serviceName, TServiceId  sid, void *arg);

#endif // NL_CONNECTION_CLIENT_H

/* End of connection_client.h */
