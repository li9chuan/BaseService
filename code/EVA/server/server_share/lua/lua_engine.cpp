#include "lua_engine.h"
#include "lua_param.h"
#include <nel/misc/displayer.h>
#include <nel/misc/path.h>
#include "pbc.h"

#include "lua_base_function.h"
#include "server_share/bin_luabind/ScriptHandle.h"
#include "server_share/bin_luabind/ScriptExporter.h"

extern int  luaopen_protobuf_c (lua_State* tolua_S);

//void Lua_Trace(lua_State *L, lua_Debug *debug_msg)
//{
//	char tmp[6]={0};
//	char * what = tmp;
//	strcpy(what,"nl\0\0");	
//	switch(debug_msg->event)
//	{
//	case LUA_HOOKCALL:
//		what = strcat(what,"uS");
//		nlinfo("LUA_HOOKCALL  ");
//		break;
//	case LUA_HOOKRET:
//		what = strcat(what,"u");
//		nlinfo("LUA_HOOKRETURN  ");
//		break;
//	case LUA_HOOKTAILRET:
//		what = strcat(what,"uS");
//		nlinfo("LUA_HOOKTAILRETURN  ");
//		break;
//	case LUA_HOOKLINE:
//		what = strcat(what,"uS");
//		nlinfo("LUA_HOOKLINE  ");
//		break;
//	case LUA_HOOKCOUNT:
//		break;
//	default:
//		break;
//	}
//	//printf("%s",what);
//	if(debug_msg->currentline >0 )	
//		nlinfo("curr line = ",debug_msg->currentline);
//
//	if(lua_getinfo(L, what, debug_msg))
//	{
//		//printf("开始于%d行，结束于%d行，使用外部变量%d个", debug_msg->linedefined,debug_msg->lastlinedefined,debug_msg->nups);
//		if(debug_msg->short_src != NULL) printf(debug_msg->short_src);
//		printf("   ");
//		if(debug_msg->what != NULL) printf(debug_msg->what);
//		printf("   ");
//		if(debug_msg->source != NULL) printf(debug_msg->source);
//		printf("   ");
//		if(debug_msg->name != NULL) printf(debug_msg->name);
//		if(debug_msg->namewhat != NULL)printf(debug_msg->namewhat);
//		printf("\n");
//	}
//	printf("\n");
//}
//
//static int Lua_Note(lua_State *L)
//{
//
//	int n = lua_gettop(L);  //传进来的参数个数
//	int i;
//	lua_getglobal(L, "tostring"); //将脚本变量tostring（脚本库函数）压栈
//	for (i=1; i<=n; i++)
//	{
//		const char *s;
//		lua_pushvalue(L, -1);  //将栈顶的变量，即为tostring 函数再次压栈
//		lua_pushvalue(L, i);   //将要打印的值，也就是脚本中调用时传入的参数压栈
//		lua_call(L, 1, 1);     //调用脚本函数tostring
//		s = lua_tostring(L, -1);  //将返回值从栈中读出
//
//		if (s == NULL)
//		{
//			return luaL_error(L, "`tostring' must return a string to `print'");			
//		}
//
//		if (i>1)
//			nldebug("\t");
//
//		nldebug("%s",s);
//		lua_pop(L, 1);  //弹出返回值
//	}
//
//	nldebug("\n");
//	return 0;
//}


CLuaEngine::CLuaEngine(void)
{
	m_pLuaState = NULL;

    m_ScriptHandle = new bin::CScriptHandle();
	//CreateLuaState();
}

CLuaEngine::~CLuaEngine(void)
{
}

bool CLuaEngine::Init( std::string logpath )
{
    if(m_pLuaState!=NULL)
        return true;

    m_pLuaState = luaL_newstate();	                /// 创建一个Lua状态机的实例	

    if(m_pLuaState==NULL)
    {
        nlerror( "Lua State Create Fail." );
        return false;
    }


    luaL_openlibs(m_pLuaState);                     /// 加载Lua的基本库


    //luaopen_protobuf_c(m_pLuaState);
    luaL_requiref(m_pLuaState, "protobuf.c", luaopen_protobuf_c, 0);


    lua_register( m_pLuaState, "print",             LuaPrint );                /// 注册常用函数
    lua_register( m_pLuaState, "AddSearchPath",     LuaAddSearchPath );


    m_ScriptHandle->Init(m_pLuaState);

    //tolua_auto_lua_callback_open(m_pLuaState);

    //if( !lua_checkstack(m_pLuaState, stacksize) )     //增加Lua的堆栈大小，防止因为堆栈过小而死机
    //{
    //	nldebug( "set LuaState size fail(stacksize=%d, top=%d, base=%d).", stacksize, m_pLuaState->top, m_pLuaState->base );
    //	return false;
    //}

    //#ifdef _DEBUG
    //	lua_sethook( m_pLuaState, Lua_Trace, LUA_MASKLINE , 0);
    //#endif


    //lua_register( m_pLuaState,"note",Lua_Note);

    return true;
}

void CLuaEngine::Release()
{
	if(m_pLuaState != NULL)
	{
		lua_close(m_pLuaState);
	}
}

lua_State *	CLuaEngine::GetLuaState()
{
	return m_pLuaState;
}

const char* CLuaEngine::GetLastError()
{
	return lua_tostring(m_pLuaState, -1);
}

bool CLuaEngine::LoadLuaFile(const char* fileName)
{
	int top = lua_gettop(m_pLuaState);			
	if( !luaL_loadfile(m_pLuaState, fileName) )
	{
		if( !lua_pcall(m_pLuaState, 0, 0, 0) )
		{
			lua_settop(m_pLuaState, top);			
			return true;
		}
	}		

	nldebug("load %s file error, cause %s.", fileName, GetLastError());
	lua_settop(m_pLuaState, top);		
	return false;
}


bool CLuaEngine::RunMemoryLua(const char* pLuaData, int nDataLen)
{
	if(pLuaData == NULL || nDataLen <= 0)
	{
		return false;
	}	
	int top = lua_gettop(m_pLuaState);
	if( !luaL_loadbuffer(m_pLuaState, pLuaData, nDataLen, pLuaData) )
	{
		if( !lua_pcall(m_pLuaState, 0, 0, 0) )
		{
			lua_settop(m_pLuaState, top);
			return true;
		}
	}

	nldebug("exec memory lua error, cause %s", GetLastError());
	lua_settop(m_pLuaState, top);	
	return false;
}

bool CLuaEngine::RunLuaFunction(const char* szFunName, const char* szTableName, const char* szSubTableName, 
					LuaParams* pInParams, LuaParam * pOutParam, int nOutNum)
{
	int top = lua_gettop(m_pLuaState);
	if(szTableName==NULL)
	{
		lua_getglobal(m_pLuaState, szFunName);
	}
	else if(szSubTableName==NULL)
	{
		lua_getglobal(m_pLuaState, szTableName);
		if(lua_istable(m_pLuaState, -1))
		{
			lua_getfield(m_pLuaState,-1,szFunName);
		}
	}
	else
	{
		lua_getglobal(m_pLuaState, szTableName);
		if(lua_istable(m_pLuaState, -1))
		{
			lua_getfield(m_pLuaState, -1,szSubTableName);
			if(lua_istable(m_pLuaState, -1))
			{
				lua_getfield(m_pLuaState,-1,szFunName);
			}
		}
	}
	
	if(!lua_isfunction(m_pLuaState, -1))
	{
        nldebug("call function(%s) fail, cause %s", szFunName, GetLastError());
		lua_settop(m_pLuaState, top);
		return false;
	}

    int inParamCnt = pInParams==NULL?0:pInParams->Count();

	for(int i = 0; i < inParamCnt; i++)
	{
		switch(pInParams->at(i).Type())
		{
			case LUA_TNUMBER:				
				lua_pushnumber(m_pLuaState, pInParams->at(i).GetInt());						
				break;			
			case LUA_TSTRING:
				lua_pushlstring(m_pLuaState, (char*)pInParams->at(i).Data(), pInParams->at(i).GetStringLen());
				break;
			case LUA_TLIGHTUSERDATA:
				lua_pushlightuserdata(m_pLuaState, pInParams->at(i).Data());
				break;
			default:
				nldebug("call function(%s) fail, in param type error", szFunName);
				lua_settop(m_pLuaState, top);
				return false;
		}
	}

	if( !lua_pcall(m_pLuaState, inParamCnt, nOutNum, 0) )
	{
		for(int n = 0; n < nOutNum; n++)
		{
			int nType = lua_type(m_pLuaState, -1);
			switch(nType)
			{
				case LUA_TNUMBER:
					pOutParam[n].SetInt( lua_tointeger(m_pLuaState, -1) );
					lua_pop(m_pLuaState, 1);
					break;
				case LUA_TBOOLEAN:
					pOutParam[n].SetInt( lua_toboolean(m_pLuaState, -1) );
					lua_pop(m_pLuaState, 1);
					break;
				case LUA_TSTRING:

					//pOutParam[n].SetString((char*)lua_tostring(m_pLuaState, -1));
					lua_pop(m_pLuaState, 1);
					break;
				case LUA_TLIGHTUSERDATA:
					pOutParam[n].SetDataPointer((void*)lua_topointer(m_pLuaState, -1));
					lua_pop(m_pLuaState, 1);
					break;
				default:
					nldebug("call function(%s) fail, out param type error = %s ", szFunName , lua_typename( m_pLuaState , -1 ) );
					lua_settop(m_pLuaState, top);
					return false;					
			}
		}

		lua_settop(m_pLuaState, top);   /// 恢复栈成为未调用时的状态。
		return true;
	}	
	
	nldebug("call function(%s) fail, cause %s", szFunName, GetLastError());
	lua_settop(m_pLuaState, top);
	return false;
}

//bool CLuaEngine::RegUserFunc( const char *classname, const luaL_Reg *l)
//{
//	if(!classname)
//		return RegUserFunc(l);
//	luaL_register(m_pLuaState,classname,l);
//	return true;
//}
//
//bool CLuaEngine::RegUserFunc(const luaL_Reg* l)
//{
//	lua_pushvalue(m_pLuaState,LUA_GLOBALSINDEX);
//	luaL_register(m_pLuaState,NULL,l);
//	return true;
//}
//
//bool CLuaEngine::RegGlobalFunc(const char* name, lua_CFunction func)
//{
//	lua_register(m_pLuaState,name,func);
//	return true;
//}

lua_Number CLuaEngine::GetLuaVariableNumber( const char* szVariableName, const char* szTableName /* = NULL */)
{
	int top = lua_gettop(m_pLuaState);
	if(szTableName==NULL)
	{
		lua_getglobal(m_pLuaState, szVariableName);
	}
	else 
	{
		lua_getglobal(m_pLuaState, szTableName);
		if(lua_istable(m_pLuaState, -1))
		{
			lua_getfield(m_pLuaState,-1,szVariableName);
		}
	}
	lua_Number result = 0;
	if (lua_isnumber(m_pLuaState, -1))
	{
		result = lua_tonumber(m_pLuaState, -1);
	}
	lua_settop(m_pLuaState, top);

	return result;
}

const char* CLuaEngine::GetLuaVariableString( const char* szVariableName, const char* szTableName /* = NULL */)
{
	int top = lua_gettop(m_pLuaState);
	if(szTableName==NULL)
	{
		lua_getglobal(m_pLuaState, szVariableName);
	}
	else 
	{
		lua_getglobal(m_pLuaState, szTableName);
		if(lua_istable(m_pLuaState, -1))
		{
			lua_getfield(m_pLuaState,-1,szVariableName);
		}
	}
	const char *result = 0;
	if (lua_isstring(m_pLuaState, -1))
	{
		result = lua_tostring(m_pLuaState, -1);
	}
	lua_settop(m_pLuaState, top);

	return result;
}

void CLuaEngine::ExportModule( const char* pszName )
{
    bin::ScriptExporterManager().ExportModule( pszName, *m_ScriptHandle );
}

void CLuaEngine::ExportClass( const char* pszName )
{
    bin::ScriptExporterManager().ExportClass( pszName, *m_ScriptHandle );
}



