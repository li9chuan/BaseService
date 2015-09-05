#include "recv_login.h"
#include "sender_login.h"
#include <server_share/server_def.h>
#include <server_share/script_mgr.h>
#include <player_logic_service/player/player_mgr.h>
#include "../event_trigger.h"


using namespace NLNET;
using namespace DEF;
using namespace PLS;

void CRecvLogin::SendPlayerInfo( NLNET::CMessage& msg , TServiceId from  )
{
    DEF::PID pid = 0;
    DEF::UID uid = 0;

    msg.serial( pid );
    msg.serial( uid );


    CPlayer* pPlayer = PlayerMgr.findPlayer(pid);
    if ( pPlayer != NULL )
    {
        /// 第一行就要设置这个，否则下面的触发可能推不出消息。
        pPlayer->setConFES(GetSID(msg.session()));

        /// 触发登陆事件;
        EventTrigger.Login(pPlayer);

        /// 通知客户端玩家基础信息;
		SenderLogin.SendPlayerInfo( msg.session(), pPlayer );
	}
	else
	{
		SendERR( PLAYER_RELOAD, uid , from, msg.session() );
	}
}


