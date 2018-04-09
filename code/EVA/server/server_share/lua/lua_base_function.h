#ifndef SERVICE_SHARD_LUA_BASE_FUNCTION_H
#define SERVICE_SHARD_LUA_BASE_FUNCTION_H

struct lua_State;

/**
*   @brief 获取从1970-1-1到输入参数的天数。
*   @param time 输入时间，0表示从1970-1-1到现在。
*   @return  间隔天数
*/
int  GetCurrentDay1970( unsigned int time );

/**
*   @brief 加载lua脚本文件。
*   @param szFileName lua脚本文件名。
*/
void LoadLuaFile(const char* szFileName);

int LuaAddSearchPath( lua_State *L );
int LuaPrint( lua_State *L );


#endif
