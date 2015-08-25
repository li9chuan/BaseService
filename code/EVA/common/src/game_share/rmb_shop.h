#ifndef GAME_SHARED_RMB_SHOP
#define GAME_SHARED_RMB_SHOP

#include <nel/misc/stream.h>
#include <game_share/game_def.h>

struct RMBShopItem
{
    uint32              index;
    DEF::TEMPLATE_ID    template_id;
    uint32              num;
    uint32              price;

    void serial( NLMISC::IStream& s )
    {
        s.serial( index );
        s.serial( template_id );
        s.serial( num );
        s.serial( price );
    }
};

struct RMBShop
{
    uint32                     m_version;
    std::vector<RMBShopItem>   m_RMBShopItems;

    RMBShop():m_version(0){}

    void serial( NLMISC::IStream& s )
    {
        s.serial( m_version );
        s.serialCont( m_RMBShopItems );
    }
};


#endif

