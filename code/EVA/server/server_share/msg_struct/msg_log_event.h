#ifndef SERVICE_SHARED_MSG_LOG_EVENT
#define SERVICE_SHARED_MSG_LOG_EVENT

#include <game_share/game_def.h>
#include <nel/misc/stream.h>

namespace MSG_STR {

    struct MsgLogEvent
    {
        DEF::PID    pid;
        std::string event;
        std::string param;

        MsgLogEvent(): pid(0)
        {
        }

        void serial( NLMISC::IStream& s )
        {
            s.serial( pid );
            s.serial( event );
            s.serial( param );
        }
    };
}

#endif
