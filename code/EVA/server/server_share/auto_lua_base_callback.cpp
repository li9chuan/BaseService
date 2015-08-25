/*
** Lua binding: base
** Generated automatically by tolua++-1.0.92 on 08/25/15 15:39:49.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_base_open (lua_State* tolua_S);

#include "lua_base_function.h"
#include <game_share/define_pro.pb.h>

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* function: GetCurrentDay1970 */
#ifndef TOLUA_DISABLE_tolua_base_GetCurrentDay197000
static int tolua_base_GetCurrentDay197000(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int time = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   int tolua_ret = (int)  GetCurrentDay1970(time);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCurrentDay1970'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: LoadLuaFile */
#ifndef TOLUA_DISABLE_tolua_base_LoadLuaFile00
static int tolua_base_LoadLuaFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* szFileName = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   LoadLuaFile(szFileName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadLuaFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: RegScriptEvent */
#ifndef TOLUA_DISABLE_tolua_base_RegScriptEvent00
static int tolua_base_RegScriptEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* script_scope = ((const char*)  tolua_tostring(tolua_S,1,0));
  int event_id = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   RegScriptEvent(script_scope,event_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RegScriptEvent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_base_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"GetCurrentDay1970",tolua_base_GetCurrentDay197000);
  tolua_function(tolua_S,"LoadLuaFile",tolua_base_LoadLuaFile00);
  tolua_function(tolua_S,"RegScriptEvent",tolua_base_RegScriptEvent00);
  tolua_constant(tolua_S,"EventInvalid",EventInvalid);
  tolua_constant(tolua_S,"EventPlayerUp",EventPlayerUp);
  tolua_constant(tolua_S,"EventCostMoney",EventCostMoney);
  tolua_constant(tolua_S,"EventLogin",EventLogin);
  tolua_constant(tolua_S,"INVALID_TYPE",INVALID_TYPE);
  tolua_constant(tolua_S,"ACCOUNT_LOGGED",ACCOUNT_LOGGED);
  tolua_constant(tolua_S,"SERVER_FULL",SERVER_FULL);
  tolua_constant(tolua_S,"SERVER_NOT_OPEN",SERVER_NOT_OPEN);
  tolua_constant(tolua_S,"TEXT_SUCESS",TEXT_SUCESS);
  tolua_constant(tolua_S,"TEXT_FAIL",TEXT_FAIL);
  tolua_constant(tolua_S,"PWD_ERROR",PWD_ERROR);
  tolua_constant(tolua_S,"PLAYER_ONLINE_TO_FES",PLAYER_ONLINE_TO_FES);
  tolua_constant(tolua_S,"PLAYER_EXISTS",PLAYER_EXISTS);
  tolua_constant(tolua_S,"PLAYER_RELOAD",PLAYER_RELOAD);
  tolua_constant(tolua_S,"SUCESS",SUCESS);
  tolua_constant(tolua_S,"NO_AUTH_TYPE",NO_AUTH_TYPE);
  tolua_constant(tolua_S,"CONFIG_NOT_FOUND",CONFIG_NOT_FOUND);
  tolua_constant(tolua_S,"NOT_ENOUGH_MONEY",NOT_ENOUGH_MONEY);
  tolua_constant(tolua_S,"PLAYER_BASE_ERROR",PLAYER_BASE_ERROR);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_base (lua_State* tolua_S) {
 return tolua_base_open(tolua_S);
};
#endif

