#include <server_share/server_def.h>
#include <nel/misc/path.h>
#include <nel/misc/md5.h>
#include <string>
#include "lua_engine.h"
#include "script_mgr.h"
#include <server_share/bin_luabind/Public.hpp>

using namespace std;
using namespace NLMISC;
using namespace NLNET;
using namespace DEF;

void forLuaBaseFunctionForceLink()
{
    ScriptMgr.ExecString( "" );
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


    BEGIN_SCRIPT_MODULE(Debug)

        DEFINE_MODULE_FUNCTION(Warning, void, (const char* waring_str))
        {
            nlwarning(waring_str);
            return 1;
        }

        DEFINE_MODULE_FUNCTION(Info, void, (const char* str))
        {
            nlinfo(str);
            return 1;
        }

        DEFINE_MODULE_FUNCTION(Debug, void, (const char* str))
        {
            nldebug(str);
            return 1;
        }

    END_SCRIPT_MODULE()
 }












