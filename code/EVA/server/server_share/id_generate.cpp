#include "id_generate.h"
#include <nel/misc/debug.h>
#include <nel/net/service.h>

CIDGenerate::CIDGenerate():m_CurrentID(0)
{

}

void CIDGenerate::init( uint32 region_id, uint32 service_id, uint64 current_id )
{
    nlassert(region_id<=0x3ff);
    nlassert(service_id<=0x3f);

    if ( m_CurrentID < current_id )
    {
        m_CurrentID = current_id;
    }
    else
    {
        nlwarning("CIDGenerate::init   CurrID>InitID   (%lld)>(%lld)", m_CurrentID, current_id);
    }

    m_BaseCode  =   region_id;
    m_BaseCode  =   m_BaseCode << PlaceHolderService;
    m_BaseCode  |=  service_id;
    m_BaseCode  =   m_BaseCode << PlaceHolderIncreaseID;
}

namespace bin
{
    BEGIN_SCRIPT_CLASS( IDGenerate, CIDGenerate )

    DEFINE_CLASS_FUNCTION( Generate, lua_Integer, ())
    {
        r = obj->Generate();
        return 1;
    }

    DEFINE_STATIC_FUNCTION(NewInstance, CIDGenerate*, (lua_Integer curr_id))
    {
        r = new CIDGenerate();

        uint32 share_id = NLNET::IService::getInstance()->getShardId();
        uint32 svr_id   = NLNET::IService::getInstance()->getServiceId().get();

        r->init(share_id, svr_id, curr_id);
        r->GetScriptObject().SetDelByScr(true);

        return 1;
    }

    END_SCRIPT_CLASS()
}

