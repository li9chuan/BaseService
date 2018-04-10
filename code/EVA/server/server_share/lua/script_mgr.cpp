#include "script_mgr.h"
#include <nel/misc/debug.h>
#include <server_share/server_def.h>
#include <string>
#include "lua_engine.h"
#include "server_share/bin_luabind/Public.hpp"
#include <server_share/lua/lua_thread.h>
#include <server_share/lua_mysql/mysql_conn.h>
#include <server_share/id_generate.h>

using namespace std;
using namespace DEF;
using namespace NLMISC;
using namespace NLNET;

extern void forLuaMysqlConnForceLink();

void luaexportforcelink()  {   forLuaMysqlConnForceLink();  CIDGenerate idgen; }

void CScriptMgr::init( LUA_OPEN pLuaOpen )
{
	//UpdateServiceBootCount();

    //string fn = IService::getInstance()->SaveFilesDirectory.toString();
    string log_file = /*fn +*/ Config.getVar("LogDirectory").asString();
    string lua_log = log_file + "lua_engine.log";

    m_LuaEngine.Release();
    nlassert( m_LuaEngine.Init(lua_log) );

    if ( pLuaOpen!=NULL )
    {
        pLuaOpen( m_LuaEngine.GetLuaState() );
    }

    ///  导出C++接口
    Export();


    CConfigFile::CVar* pVar = NULL;

    if ((pVar = Config.getVarPtr("StartLuaScript")) != NULL)
    {
        string script_full_path = CPath::lookup( pVar->asString() );
        nlinfo("Loading %s.", script_full_path.c_str());
        ScriptMgr.LoadScrpit(script_full_path.c_str());
    }
}

LuaParams CScriptMgr::run( std::string script_scope, std::string script_name, LuaParams lua_in, uint outnum )
{
    nlassert(outnum<=LuaParams::MAX_PARAMS);

    LuaParams lua_out;
    lua_out.resize(outnum);

    bool run_ret = m_LuaEngine.RunLuaFunction( script_name.c_str(), script_scope.c_str(), NULL, 
                                                &lua_in, lua_out.GetParams(), lua_out.Count() );

    //nlassert( run_ret );

    if ( !run_ret )
    {
        lua_out.resize(0);
    }

    return lua_out;
}

lua_State * CScriptMgr::GetLuaState()
{
    return m_LuaEngine.GetLuaState();
}

void CScriptMgr::release()
{
    m_LuaEngine.RunLuaFunction( "ServiceRelease" );
    m_LuaEngine.Release();
}

void CScriptMgr::LoadScrpit( const char* szName )
{
    if( m_LuaEngine.LoadLuaFile(szName) )
    {
        m_LuaEngine.RunLuaFunction( "ServiceInit" );
    }
}

void CScriptMgr::update()
{
    m_LuaEngine.RunLuaFunction( "ServiceUpdate" );
}



void CScriptMgr::Export()
{
    m_LuaEngine.ExportModule("Utility");
    m_LuaEngine.ExportModule("Debug");
    m_LuaEngine.ExportModule("LuaThread");
    m_LuaEngine.ExportModule("ServerNet");
    m_LuaEngine.ExportClass("WebSocketNetwork");
    m_LuaEngine.ExportClass("LuaTimer");
    m_LuaEngine.ExportClass("IDGenerate");


    m_LuaEngine.ExportClass("MysqlStmt");
    m_LuaEngine.ExportClass("MysqlConn");
    m_LuaEngine.ExportClass("MysqlResult");

    
}

void CScriptMgr::ExecString( std::string exec_str )
{
    m_LuaEngine.GetScriptHandle()->ExecString( exec_str.c_str() );
}

void CScriptMgr::UpdateServiceBootCount()
{
    std::string cache_file = ".";
    cache_file.append( NLNET::IService::getInstance()->getServiceShortName() );
    cache_file.append( "-" );
    cache_file.append( NLMISC::toString(NLNET::IService::getInstance()->getServiceId().get()) );
    cache_file.append( ".che" );

    if (!NLMISC::CFile::fileExists(cache_file))
    {
        NLMISC::CFile::createEmptyFile(cache_file);
    }

    CConfigFile cf;
    cf.load(cache_file);

    if ( !cf.exists("BootCnt") )
    {
        CConfigFile::CVar var;
        var.forceAsInt(1);
        cf.insertVar("BootCnt", var);
    }
    else
    {
        CConfigFile::CVar* pVar = cf.getVarPtr("BootCnt");
        uint32 boot_cnt = pVar->asInt();
        ++boot_cnt;
        pVar->setAsInt(boot_cnt);
    }

    cf.save();
}



NLMISC_COMMAND (lua, "run lua string.", "lua")
{
    if(args.size() != 1) return false;
    ScriptMgr.ExecString( args[0] );
    return true;
}


