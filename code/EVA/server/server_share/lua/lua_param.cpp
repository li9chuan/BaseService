#include "lua_param.h"
#include <nel/misc/debug.h>
#include <nel/misc/displayer.h>

static LuaParam  err;

LuaParam& LuaParams::GetParam( uint index )
{
    if ( index<m_Count )
    {
        return m_Params[index];
    }
    else
    {
        nldebug("GetParam Error.");

        
        return err;
    }
}
