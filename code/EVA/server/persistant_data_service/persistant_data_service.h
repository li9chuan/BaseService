#ifndef EVA_PERSISTANT_DATA_SERVICE_H
#define EVA_PERSISTANT_DATA_SERVICE_H

#include "nel/misc/types_nl.h"
#include "nel/net/service.h"

/**
* Persistant Data Service Class
* \author li9chuan@qq.com
* \date 2013
*/
class CPersistantDataService : public NLNET::IService
{
public:

    /// Constructor
    CPersistantDataService(){}
    /// Initialization
    virtual void    init();
    /// Release
    virtual void    release();
    /// Update
    virtual bool    update();

};


#endif // EVA_PERSISTANT_DATA_SERVICE_H

/* End of persistant_data_service.h */
