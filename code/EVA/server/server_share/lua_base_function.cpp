#include <server_share/lua_engine.h>
#include <server_share/script_mgr.h>
#include <server_share/server_def.h>
#include <nel/misc/path.h>
#include <string>

#include "lua_base_function.h"
#include "static_table/static_table_mgr.h"


using namespace std;
using namespace NLMISC;
using namespace NLNET;
using namespace DEF;
using namespace ATTRIB_TYPE;
using namespace StaticData;

int  GetCurrentDay1970( uint32 time )
{
    int res = 0;
    int sec_day = 60 * 60 * 24;
    if ( time == 0 )
    {
        res = CTime::getSecondsSince1970() / sec_day;
    }
    else
    {
        res = time / sec_day;
    }
    return res;
}

void LoadLuaFile(const char* szFileName)
{
    string script = NLMISC::CPath::lookup(string(szFileName));

    if ( script.size() > 0 )
    {
        ScriptMgr.LoadScrpit(script.c_str());
    }
}

void RegScriptEvent( const char* script_scope, int event_id )
{
    ScriptMgr.register_event(script_scope,event_id);
}


