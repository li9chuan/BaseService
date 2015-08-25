#ifndef SERVICE_SHARED_MSG_PLS
#define SERVICE_SHARED_MSG_PLS

#include <game_share/game_def.h>
#include <nel/net/unified_network.h>

namespace MSG_STR {

	struct CPlayerLogicService
	{
		NLNET::TServiceId	SId;
		uint32              PlayerMax;
		uint32              PlayerUsed;
		std::string         Host;
		uint32              Port;

        bool                IsFull()        {  return PlayerUsed>=PlayerMax;  }

		void serial( NLMISC::IStream& s )
		{
			s.serial( SId );
			s.serial( PlayerMax );
			s.serial( PlayerUsed );
			s.serial( Host );
			s.serial( Port );
		}
	};

    struct CRefreshMoneyScore
    {
        DEF::PID    unRoleID;
        DEF::MONEY  nMoney; 
        uint32      nPvPScore;

        CRefreshMoneyScore( void )
            : unRoleID( 0 )
            , nMoney( 0 )
            , nPvPScore( 0 )
        {}

        void serial( NLMISC::IStream& s )
        {
            s.serial( unRoleID );
            s.serial( nMoney );
            s.serial( nPvPScore );
        }
    };

}

#endif
