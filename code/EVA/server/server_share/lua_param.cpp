#include "lua_param.h"
#include <nel/misc/debug.h>
#include <nel/misc/displayer.h>

NLMISC::CLog &LuaLoger()
{
    static NLMISC::CLog	theVar;
    return theVar;
}

lua_Number LuaParams::GetParam( uint index, lua_Number defaultValue/*=0 */ )
{
    if ( index<m_Count )
    {
        return m_Params[index].DataNumber();
    }
    else
    {
        LuaLoger().displayNL("GetParam Error.");
        return defaultValue;
    }
}
