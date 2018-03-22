#ifndef FRONTEND_SERVICE_H
#define FRONTEND_SERVICE_H

// We're using the NeL Service framework and layer 5
#include <nel/misc/stop_watch.h>
#include <nel/net/service.h>

#include <vector>
#include <string>

//typedef CHashMap< TDataSetIndex, std::string> TEntityNamesMap;

/**
 * CFrontEndService, based on IService5
 */
class CFrontEndService : public NLNET::IService
{
public:

    CFrontEndService():ReceiveWatch(10),
        SendWatch(10) {}

    /// Return the instance of the service
    static CFrontEndService *instance() { return (CFrontEndService*)IService::getInstance(); }

    // Initialisation
    void init();
    bool update();
    void release();

    void SendToClient (const NLNET::CMessage &buffer, NLNET::TSockId hostid, bool log = true) {  /*m_Clients->send(buffer, hostid, log);*/  }
    void DisconnectClient (NLNET::TSockId hostid) {  /*m_Clients->disconnect(hostid);*/  }

    NLMISC::CStopWatch	ReceiveWatch;				// All Receive Sub
    NLMISC::CStopWatch  SendWatch;					// All Send Sub

    bool				AcceptClients;

    void  IncReceiveMsgCount( std::string msg_name );

    typedef std::map<std::string, uint64>   TReceiveMsgCount;
    TReceiveMsgCount                        m_ReceiveMsgCount;

private:

    NLMISC::TTime   _CurrTicks;

    std::vector<std::string>    _WaitServiceAccess;

};

#define  FrontEndService  CFrontEndService::instance()

#endif
