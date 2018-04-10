#ifndef SERVER_SHARD_SCRIPT_MGR_H
#define SERVER_SHARD_SCRIPT_MGR_H

#include <nel/misc/singleton.h>
#include <server_share/game_def.h>
#include <set>
#include "lua_engine.h"

typedef int (*LUA_OPEN)(lua_State* tolua_S);

class CScriptMgr : public NLMISC::CSingleton<CScriptMgr>
{
public:

	void            init( LUA_OPEN pToluaOpen=NULL );
    void            update();
    void		    release();

    LuaParams       run( std::string script_scope, std::string script_name, LuaParams lua_in, uint outnum=0 );

    void            ExecString( std::string );


    void            LoadScrpit(const char* szName);
    lua_State*      GetLuaState();
    CLuaEngine&     GetLuaEngine()  { return m_LuaEngine; }

private:

    void            UpdateServiceBootCount();

    void            Export();
    CLuaEngine      m_LuaEngine;
};

#define  ScriptMgr  CScriptMgr::instance()
#define  ScriptRun  ScriptMgr.run

#endif






