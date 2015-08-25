#include "chat_logic.h"
#include <server_share/server_def.h>
#include "../player/player_mgr.h"

namespace PLS
{
    using namespace NLMISC;
    using namespace NLNET;

    void CChatLogic::GlobleChat( DEF::PID pid, MsgChat& msg_chat )
    {
        CPlayer* pPlayer = PlayerMgr.findPlayer(pid);

        if ( pPlayer!=NULL )
        {
            msg_chat.set_pid( pid );
            msg_chat.set_name( pPlayer->getPlayerHelper().getName() );

            CMessage  msgout("GC");
            msgout.serial(&msg_chat);

            Network->send( "FES", msgout );
        }
    }

}

