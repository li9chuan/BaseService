#ifndef PDS_DB_MANAGER_MESSAGES_H
#define PDS_DB_MANAGER_MESSAGES_H

#include <nel/misc/types_nl.h>
#include <nel/net/unified_network.h>

void cbPut  (NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId  sid);
void cbLoadPlayer  (NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId  sid);
void cbCreatePlayer  (NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId  sid);


void cbEnterGame( NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId  sid );
void cbPlayerBaseToClient( NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId  sid );
void cbRequestNextPlayer( NLNET::CMessage &msgin, const std::string &serviceName, NLNET::TServiceId sid );


#endif // PDS_DB_MANAGER_MESSAGES_H

/* End of db_manager_messages.h */
