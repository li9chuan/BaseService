#ifndef SERVICE_SHARED_MSG_LOGIN
#define SERVICE_SHARED_MSG_LOGIN

#include <game_share/game_def.h>
#include <nel/net/unified_network.h>

namespace MSG_STR {

	struct CFrontEnd
	{
		CFrontEnd() {}

		uint32              ShardId;
        uint32              ClientMax;
        uint32              CurrClientCount;
        uint32              Port;
		NLNET::TServiceId	SId;		// Service Id of the frontend on the remote shard
		NLNET::TServiceId   ConnectId;
        std::string         HostName;
        std::string         UnifiedName;
		

		void serial( NLMISC::IStream& s )
		{
			s.serial( ShardId );
            s.serial( ClientMax );
            s.serial( CurrClientCount );
			s.serial( Port );
            s.serial( SId );
            s.serial( HostName );
            s.serial( UnifiedName );
		}
	};

}

#endif
