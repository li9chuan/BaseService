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

    NLMISC::CStopWatch	ReceiveWatch;				// All Receive Sub
    NLMISC::CStopWatch  SendWatch;					// All Send Sub
};

#define  FrontEndService  CFrontEndService::instance()

#endif
