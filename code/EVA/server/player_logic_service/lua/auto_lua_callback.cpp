/*
** Lua binding: pls
** Generated automatically by tolua++-1.0.92 on 08/25/15 15:39:49.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_pls_open (lua_State* tolua_S);

#include "lua_function.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* function: AddMoney */
#ifndef TOLUA_DISABLE_tolua_pls_AddMoney00
static int tolua_pls_AddMoney00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned long long pid = ((unsigned long long)  tolua_tonumber(tolua_S,1,0));
  int money = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   AddMoney(pid,money);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddMoney'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_pls_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"AddMoney",tolua_pls_AddMoney00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_pls (lua_State* tolua_S) {
 return tolua_pls_open(tolua_S);
};
#endif

