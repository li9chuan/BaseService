#include <server_share/lua_engine.h>
#include <server_share/script_mgr.h>
#include <server_share/server_def.h>
#include <server_share/database/db_logic_chunk.h>
#include <nel/misc/path.h>
#include <string>

#include "lua_function.h"
#include "../player/player_mgr.h"
#include "static_table/static_table_mgr.h"

using namespace std;
using namespace PLS;
using namespace NLMISC;
using namespace NLNET;
using namespace DEF;
using namespace DB_RECORD;
using namespace ATTRIB_TYPE;
using namespace StaticData;

void AddMoney( unsigned long long pid, int money )
{
    CPlayer* pPlayer = PlayerMgr.findPlayer(pid);
    if ( pPlayer )
    {
        pPlayer->getPlayerHelper().addMoney(money);
    }
}

