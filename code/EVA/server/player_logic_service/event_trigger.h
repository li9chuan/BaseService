#ifndef PLS_EVENT_TEIGGER_H
#define PLS_EVENT_TEIGGER_H

#include <nel/misc/singleton.h>
#include <game_share/game_def.h>
#include <game_share/sigslot.h>
#include <map>

namespace DB_RECORD
{
class CRecordItem;
class CRecordHero;
class CRecordGuard;
}

namespace PLS
{
class CPlayer;

class CEventTrigger : public NLMISC::CSingleton<CEventTrigger>
{
public:

    sigslot::signal2<CPlayer*, uint64/*消费金钱*/>          CostMoney;      // 消费金钱
    sigslot::signal2<CPlayer*, uint64/*获得金钱*/>          AddMoney;       // 获得金钱
    sigslot::signal1<CPlayer*>                             Login;          // 登录


};


}





#define  EventTrigger  PLS::CEventTrigger::instance()

#endif







