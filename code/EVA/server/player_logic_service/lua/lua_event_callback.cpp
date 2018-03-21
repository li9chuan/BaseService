#include "lua_event_callback.h"
#include "../event_trigger.h"
#include "../player/player_mgr.h"
#include <define_attrib.pb.h>
#include <game_share/static_table/static_table_mgr.h>
#include <server_share/lua/script_mgr.h>

namespace PLS
{

using namespace std;
using namespace DEF;
using namespace NLMISC;
using namespace NLNET;
using namespace ATTRIB_TYPE;
using namespace StaticData;
using namespace DB_RECORD;

CLuaEventCallback* CLuaEventCallback::Instance = NULL;

void CLuaEventCallback::RegisterMoniterEvent()
{
    EventTrigger.Login.connect( Instance, &CLuaEventCallback::CallbackLogin );
}

void CLuaEventCallback::CallbackLogin( CPlayer* pPlayer )
{
    ScriptMgr.run( "event_cb_base", "Login", LuaParams( pPlayer->getPID() ) );
}



}


