#include "id_generate.h"
#include <nel/misc/debug.h>

CIDGenerate::CIDGenerate():m_CurrentID(0)
{

}

void CIDGenerate::init( uint region, uint8 service_id, uint64 current_id )
{
    nlassert(region<=0x3ff);
    nlassert(service_id<=0x3f);

    if ( m_CurrentID < current_id )
    {
        m_CurrentID = current_id;
    }

    m_BaseCode  = service_id;
    m_BaseCode  = m_BaseCode << (PlaceHolderIncreaseID+PlaceHolderRegion);
    m_BaseCode |= region;
}

void CIDGenerate::init( uint region, uint64 current_id )
{
    init(region,0,current_id);
}
