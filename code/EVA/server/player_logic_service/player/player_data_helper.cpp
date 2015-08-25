#include "player_data_helper.h"
#include "player.h"
#include "../event_trigger.h"

#include <game_share/static_table/static_table_mgr.h>
#include <game_share/eval_expression.h>
#include <server_share/server_def.h>
#include <server_share/script_mgr.h>

using namespace DB_RECORD;
using namespace NLMISC;
using namespace NLNET;
using namespace DEF;
using namespace std;


PLS::CPlayerDataHelper::CPlayerDataHelper( DB_RECORD::CDBLogicPlayer& DBLogicPlayer ):
_RecordPlayer(DBLogicPlayer.RecordPlayer)
{

}

DEF::PID PLS::CPlayerDataHelper::getPID()
{
	return _RecordPlayer.pid;
}

DEF::UID PLS::CPlayerDataHelper::getUID()
{
	return _RecordPlayer.uid;
}

std::string PLS::CPlayerDataHelper::getName()
{
	return _RecordPlayer.name;
}

DEF::MONEY PLS::CPlayerDataHelper::addMoney( DEF::MONEY change_value, bool save/*=true*/ )
{
    if ( change_value!=0 && _RecordPlayer.money < 0x4000000000000000 )
    {
        DEF::MONEY addmoney = abs(change_value);
        _RecordPlayer.money += addmoney;

        SavePlayerToDB(save);

        EventTrigger.AddMoney(m_Player,addmoney);
    }

    return _RecordPlayer.money;
}

bool PLS::CPlayerDataHelper::subMoney( DEF::MONEY sub_money, bool save/*=true */ )
{
    if ( sub_money==0 )
    {
        return true;
    }

    DEF::MONEY submoney = abs(sub_money);

    if ( getMoney()>=submoney )
    {
        _RecordPlayer.money -= submoney;

        SavePlayerToDB(save);

        EventTrigger.CostMoney(m_Player, submoney);
        return true;
    }

    return false;
}

void PLS::CPlayerDataHelper::SavePlayerToDB(bool save/*=true */)
{
    if ( save )
    {
        //SaveToDB(UPDATE_PLAYER_INFO,m_Player->getPID(),_RecordPlayer);
    }
}

const DB_RECORD::CRecordPlayer& PLS::CPlayerDataHelper::getRecordPlayer()
{
	return _RecordPlayer;
}

void PLS::CPlayerDataHelper::UpdateLastOnlineTime()
{
    _RecordPlayer.last_online_time = CTime::getSecondsSince1970();
    SavePlayerToDB(true);
}


