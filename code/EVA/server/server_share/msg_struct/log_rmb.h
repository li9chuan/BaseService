#ifndef SERVICE_SHARED_MSG_LOG_RMB
#define SERVICE_SHARED_MSG_LOG_RMB

#include <game_share/game_def.h>
#include <nel/misc/stream.h>

namespace MSG_STR {

    struct LogRmb
    {
        sint64      Charge;
        sint64      Donate;
        sint64      Bind;
        sint64      CurrCharge;
        sint64      CurrDonate;
        sint64      CurrBind;

        LogRmb():
            Charge(0), Donate(0), Bind(0),
            CurrCharge(0), CurrDonate(0), CurrBind(0)
        {

        }

        void serial( NLMISC::IStream& s )
        {
            s.serial( Charge );
            s.serial( Donate );
            s.serial( Bind );
            s.serial( CurrCharge );
            s.serial( CurrDonate );
            s.serial( CurrBind );
        }
    };
}

#endif
