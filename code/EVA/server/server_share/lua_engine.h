#pragma once
#include "lua_param.h"

class CLuaEngine
{
public:
	CLuaEngine(void);
	virtual ~CLuaEngine(void);

public:
	bool			Init(std::string logpath);
	void			Release();
	lua_State *		GetLuaState();
	const char*		GetLastError();
	
	//bool			RegUserFunc(const char *classname, const luaL_Reg *l);
	//bool			RegUserFunc(const luaL_Reg* l);
	//bool			RegGlobalFunc(const char* name, lua_CFunction func);
	
	bool			LoadLuaFile(const char* szName);
	bool			RunMemoryLua(const char* pLuaData, int nDataLen);
	bool			RunLuaFunction(const char* szFunName, const char* szTableName = NULL,  const char* szSubTableName = NULL,LuaParam * pInParam = NULL, int nInNum = 0, LuaParam * pOutParam = NULL, int nOutNum = 0);

	lua_Number		GetLuaVariableNumber( const char* szVariableName, const char* szTableName = NULL);
	const char*		GetLuaVariableString( const char* szVariableName, const char* szTableName = NULL);

private:
	bool			CreateLuaState();
	lua_State*		m_pLuaState;
};





