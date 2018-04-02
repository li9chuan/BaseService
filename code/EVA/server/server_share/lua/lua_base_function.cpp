#include <server_share/server_def.h>
#include <nel/misc/path.h>
#include <string>

#include "lua_base_function.h"
#include "lua_engine.h"
#include "script_mgr.h"
#include "static_table/static_table_mgr.h"
#include <nel/misc/md5.h>

#include "server_share/bin_luabind/Public.hpp"

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

int LuaAddSearchPath( lua_State *L )
{
    int n = lua_gettop(L);

    //if( n == 1)

    const char *pPath = lua_tostring(L, -1);    //将返回值从栈中读出
    lua_pop(L, 1);                              //弹出返回值


    lua_getglobal(L, "package");  
    lua_getfield(L, -1, "path"); 
    std::string v;
    v.append(lua_tostring(L, -1)); 
    v.append(";");
    v.append(NLMISC::CPath::getCurrentPath());
    v.append("/");
    v.append(pPath);  
    lua_pushstring(L, v.c_str());  
    lua_setfield(L, -3, "path");  
    lua_pop(L, 2);  

    return 1;
}

 namespace bin
 {
     BEGIN_SCRIPT_MODULE(Utility)
 
     DEFINE_MODULE_FUNCTION(GetBasePath, std::string, (void))
     {
         //nldebug("Calling func5 with (%s, %lld)\n", path.c_str(), a1);
         r = NLMISC::CPath::getCurrentPath();
         return 1;
     }

     DEFINE_MODULE_FUNCTION(GetConfigInt, int, (std::string config_field))
     {
         CConfigFile::CVar *pVar = Config.getVarPtr (config_field);

         if ( pVar!=NULL )
         {
             r = pVar->asInt();
         }

         return 1;
     }

     DEFINE_MODULE_FUNCTION(MD5, std::string, (const char* buff, int len))
     {
         CHashKeyMD5 hash_key = getMD5( (const uint8*)buff, len );
         r = hash_key.toString();
         return 1;
     }

     END_SCRIPT_MODULE()
 }

int LuaPrint(lua_State *L)
{
    int n = lua_gettop(L);          //传进来的参数个数
    int i;
    //	const char* str = lua_tostring(L,-1);
    lua_getglobal(L, "tostring");   //将脚本变量tostring（脚本库函数）压栈
    for (i=1; i<=n; i++)
    {
        const char *s;
        lua_pushvalue(L, -1);       //将栈顶的变量，即为tostring 函数再次压栈
        lua_pushvalue(L, i);        //将要打印的值，也就是脚本中调用时传入的参数压栈
        lua_call(L, 1, 1);          //调用脚本函数tostring
        s = lua_tostring(L, -1);    //将返回值从栈中读出
        if (s == NULL)
            return luaL_error(L, "`tostring' must return a string to `print'");
        lua_pop(L, 1);  //弹出返回值

        nldebug("%s",s);
    }
    return 0;
}










