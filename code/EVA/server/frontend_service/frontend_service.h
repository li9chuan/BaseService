#ifndef FRONTEND_SERVICE_H
#define FRONTEND_SERVICE_H

// We're using the NeL Service framework and layer 5
#include <nel/misc/stop_watch.h>
#include <nel/net/service.h>
#include "udp/fe_receive_sub.h"

#include <vector>
#include <string>

//typedef CHashMap< TDataSetIndex, std::string> TEntityNamesMap;

/**
 * CFrontEndService, based on IService5
 */
class CFrontEndService : public NLNET::IService
{
public:

    CFrontEndService():_preTime(0),ReceiveWatch(10),
        SendWatch(10) {}

    /// Return the instance of the service
    static CFrontEndService *instance() { return (CFrontEndService*)IService::getInstance(); }

    // Initialisation
    void init();
    bool update();
    void release();

    /// Accept Clients
    //bool		AcceptClients;
    //std::string getInetIP()  { return _inetIP; }
    //uint32      getInetPort(){return _inetPort;}

    void SendToClient (const NLNET::CMessage &buffer, NLNET::TSockId hostid, bool log = true) {  /*m_Clients->send(buffer, hostid, log);*/  }
    void DisconnectClient (NLNET::TSockId hostid) {  /*m_Clients->disconnect(hostid);*/  }

    void SendUDP( const uint8* buffer, uint length, NLNET::CInetAddress& addr )
    {
        try
        {
            _ReceiveSub.dataSock()->sendTo(buffer,length,addr);
        }
        catch (...)
        {
        }
    }

    //NLNET::CCallbackServer&     Clients()   {  return *m_Clients;  }

    NLMISC::CStopWatch	ReceiveWatch;				// All Receive Sub
    NLMISC::CStopWatch  SendWatch;					// All Send Sub

    CFeReceiveSub&	ReceiveSub() { return _ReceiveSub; }
    //CFeSendSub&		SendSub()    { return _SendSub; }

    bool				AcceptClients;

    void  IncReceiveMsgCount( std::string msg_name );

    typedef std::map<std::string, uint64>   TReceiveMsgCount;
    TReceiveMsgCount                        m_ReceiveMsgCount;

private:
    uint32         _preTime;
//	std::string	   _inetHost;
    uint32		   _inetPort;

    NLMISC::TTime   _CurrTicks;

    //NLNET::CCallbackServer*    m_Clients;

    CFeReceiveSub				_ReceiveSub;
    std::vector<std::string>    _WaitServiceAccess;

};

#define  FrontEndService  CFrontEndService::instance()

#endif
