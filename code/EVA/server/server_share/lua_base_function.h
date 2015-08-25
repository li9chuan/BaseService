#ifndef SERVICE_SHARD_LUA_BASE_FUNCTION_H
#define SERVICE_SHARD_LUA_BASE_FUNCTION_H

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

/**
*   @brief 注册脚本事件到脚本管理器。
*          服务器监听该事件，如果触发会调用脚本的OnEvent。
*   @param script_scope 脚本名字。
*   @param event_id 事件ID。
*/
void RegScriptEvent( const char* script_scope, int event_id );


#endif
