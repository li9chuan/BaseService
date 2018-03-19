#ifndef SERVER_SHARD_SCRIPT_MGR_H
#define SERVER_SHARD_SCRIPT_MGR_H

#include <nel/misc/singleton.h>
#include <game_share/game_def.h>
#include <set>
#include "lua_engine.h"

typedef int (*LUA_OPEN)(lua_State* tolua_S);

class CScriptMgr : public NLMISC::CSingleton<CScriptMgr>
{
public:

	void        init( LUA_OPEN pToluaOpen=NULL );
    void		release();

	bool        register_event( std::string script_scope, DEF::EVENT_ID script_event );
	bool        on_event( DEF::EVENT_ID script_event, LuaParams lua_in );
    LuaParams   run( std::string script_scope, std::string script_name, LuaParams lua_in, uint outnum=0 );

    bool        LoadScrpit(const char* szName);
    lua_State*  GetLuaState();

private:

	enum	{ SCRIPT_EVENT_MAX = 0xffff };

	typedef std::set<DEF::SCRIPT_ID>   TScriptReg;
	TScriptReg	m_ScriptReg;

	typedef std::vector<std::string>			   TEventScript;
	typedef std::vector<TEventScript>              TEventReg;	//	TEventReg[MISSION_EVENT][MISSION_ID]
	TEventReg   m_EventReg;

    CLuaEngine  m_LuaEngine;
};

#define  ScriptMgr  CScriptMgr::instance()
#define  ScriptRun  ScriptMgr.run

#endif






