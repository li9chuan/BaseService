#include "lua_thread.h"
#include <nel/misc/thread.h>
#include "script_mgr.h"
#include <server_share/server_def.h>

using namespace NLMISC;

CLuaThread::CLuaThread( std::string name ) : m_ThreadHandle(NULL), m_ThreadName(name), m_RequireExit(false)
{
    LuaThreadMgr.RegisterLuaThread(name, this);
}


CLuaThread::~CLuaThread( void )
{
    LuaThreadMgr.RemoveLuaThread(m_ThreadName);
    Close();
}
 
bool CLuaThread::Start( std::string& lua_start, std::string& export_name )
{
    m_ThreadHandle = NLMISC::IThread::create( this );

    if ( m_ThreadHandle != NULL )
    {
        m_SubLuaEngine.Init();

        m_SubLuaEngine.ExportModule("Misc");
        m_SubLuaEngine.ExportModule("Debug");
        m_SubLuaEngine.ExportModule("LuaThread");

        m_SubLuaEngine.ExportClass("MysqlStmt");
        m_SubLuaEngine.ExportClass("MysqlConn");
        m_SubLuaEngine.ExportClass("MysqlResult");

        m_ToSubEvent.init(1024*1024);
        m_ToMainEvent.init(1024*1024);

        m_ThreadHandle->start();


        //GetScriptObject().SetDelByScr(false);
        //m_SubLuaEngine.GetScriptHandle()->Set( export_name.c_str(), this );

        m_SubLuaEngine.GetScriptHandle()->Exec(lua_start.c_str());
    }

    return ( NULL != m_ThreadHandle );
}

void CLuaThread::Close( void )
{
    if ( NULL != m_ThreadHandle )
    {
        m_RequireExit = true;
        m_ThreadHandle->wait();
        m_ThreadHandle = NULL;
    }
}

void CLuaThread::Update( void )
{
    CThreadEvent* pEvent = NULL;

    while ( (pEvent=m_ToMainEvent.pop_front()) != NULL )
    {
        LuaParams lua_params( pEvent->m_From, pEvent->m_EventType, pEvent->m_Data );  
        ScriptMgr.run( "NetWorkHandler", "OnMessage", lua_params );
    }
}

void CLuaThread::run( void )
{
    while ( !m_RequireExit )
    {
        CThreadEvent* pEvent = m_ToSubEvent.pop_front();

        if ( pEvent != NULL )
        {
            LuaParams lua_params( pEvent->m_From, pEvent->m_EventType, pEvent->m_Data );
            m_SubLuaEngine.RunLuaFunction( "OnMessage", "NetWorkHandler", NULL, &lua_params );
        }
        else
        {
            NLMISC::nlSleep( 3 );
        }
    }
}

void CLuaThreadMgr::Init()
{
    CConfigFile::CVar* pVar = Config.getVarPtr("LuaWorkThread");

    if ( pVar != NULL )
    {
        for (uint i = 0; i < pVar->size(); ++i)
        {
            NLMISC::CSString start_file = pVar->asString(i);
            CVectorSString  res;
            start_file.splitBySeparator( ' ', res );

            if( res.size()!=2 ) { nlwarning( "%s, config format error.", start_file.c_str() ); continue; }

            NLMISC::CSString script_full_path = CPath::lookup( res[1] );

            if( !script_full_path.empty() )
            {
                CLuaThread* pLuaThread = new CLuaThread( res[0] );
                pLuaThread->Start( script_full_path, res[0] );
            }
            else
            {
                nlwarning( "%s, not found.", script_full_path.c_str() );
            }
        }
    }
}

void CLuaThreadMgr::RegisterLuaThread( std::string& name, CLuaThread* pThread )
{
    CSynchronized<TThreadHandle>::CAccessor luathreadsync( &m_LuaThreadHandle );
    TThreadHandle::iterator iter = luathreadsync.value().find(name);

    if ( iter == luathreadsync.value().end() )
    {
        luathreadsync.value().insert( make_pair(name, pThread) ); 
    }
    else
    {
        nlstop;
    }
}

void CLuaThreadMgr::Update()
{
    TThreadHandle::iterator ipb;
    CSynchronized<TThreadHandle>::CAccessor luathreadsync( &m_LuaThreadHandle );

    for ( ipb=luathreadsync.value().begin(); ipb!=luathreadsync.value().end(); ++ipb )
    {
        ipb->second->Update();
    }
}

void CLuaThreadMgr::Release()
{
    CSynchronized<TThreadHandle>::CAccessor luathreadsync( &m_LuaThreadHandle );

    for ( TThreadHandle::iterator ipb=luathreadsync.value().begin(); ipb!=luathreadsync.value().end(); ++ipb )
    {
        delete ipb->second;
    }

    luathreadsync.value().clear();
}

void CLuaThreadMgr::RemoveLuaThread( std::string& name )
{
    CSynchronized<TThreadHandle>::CAccessor luathreadsync( &m_LuaThreadHandle );
    luathreadsync.value().erase(name);
}

CLuaThread* CLuaThreadMgr::GetLuaThread( std::string& name )
{
    CLuaThread* pLuaThread = NULL;
    CSynchronized<TThreadHandle>::CAccessor luathreadsync( &m_LuaThreadHandle );
    TThreadHandle::iterator iter = luathreadsync.value().find(name);

    if ( iter != luathreadsync.value().end() )
    {
        pLuaThread = iter->second;
    }

    return pLuaThread;
}


namespace bin
{
    //BEGIN_SCRIPT_CLASS( LuaThread, CLuaThread )

    //DEFINE_CLASS_FUNCTION( PostSub, void, (const char* buff, CScriptTable& tb_msg))
    //{
    //    if( tb_msg.IsReferd() )
    //    {
    //        lua_Integer     from;
    //        CThreadEvent*   pThreadEvent = new CThreadEvent();
    //        lua_Integer     buf_len;

    //        tb_msg.Get(1, from);
    //        tb_msg.Get(2, pThreadEvent->m_EventType);
    //        tb_msg.Get(3, buf_len);

    //        pThreadEvent->m_From = from;
    //        pThreadEvent->m_Data.assign( buff, buf_len );
    //        obj->PostSub(pThreadEvent);
    //    }

    //    return 1;
    //}

    //DEFINE_CLASS_FUNCTION( PostMain, void, (const char* buff, CScriptTable& tb_msg))
    //{
    //    if( tb_msg.IsReferd() )
    //    {
    //        lua_Integer     from;
    //        CThreadEvent*   pThreadEvent = new CThreadEvent();
    //        lua_Integer     buf_len;

    //        tb_msg.Get(1, from);
    //        tb_msg.Get(2, pThreadEvent->m_EventType);
    //        tb_msg.Get(3, buf_len);

    //        pThreadEvent->m_From = from;
    //        pThreadEvent->m_Data.assign( buff, buf_len );
    //        obj->PostMain(pThreadEvent);
    //    }

    //    return 1;
    //}

    //DEFINE_STATIC_FUNCTION(NewInstance, CLuaThread*, (std::string name))
    //{
    //    r = new CLuaThread(name);
    //    r->GetScriptObject().SetDelByScr(true);

    //    return 1;
    //}

    //END_SCRIPT_CLASS()



    BEGIN_SCRIPT_MODULE(LuaThread)

    //DEFINE_MODULE_FUNCTION(GetMain, void, (std::string lua_name, std::string thread_name))
    //{
    //    _LuaThreadMutex.enter();
    //    CLuaThread* pThread = LuaThreadMgr.GetLuaThread(thread_name);

    //    if( pThread != NULL )
    //    {
    //        ScriptMgr.GetLuaEngine().GetScriptHandle()->Set( lua_name.c_str(), pThread );
    //    }
    //    _LuaThreadMutex.leave();

    //    return 1;
    //}

    DEFINE_MODULE_FUNCTION( PostMain, void, (const char* buff, CScriptTable& tb_msg) )
    {
        if( tb_msg.IsReferd() )
        {
            std::string     thread_name;
            lua_Integer     from;
            CThreadEvent*   pThreadEvent = new CThreadEvent();
            lua_Integer     buf_len;

            tb_msg.Get(1, thread_name);
            tb_msg.Get(2, from);
            tb_msg.Get(3, pThreadEvent->m_EventType);
            tb_msg.Get(4, buf_len);

            pThreadEvent->m_From = from;
            pThreadEvent->m_Data.assign( buff, buf_len );

            CLuaThread* pThread = LuaThreadMgr.GetLuaThread(thread_name);
            
            if( pThread != NULL )
            {
                pThread->PostMain(pThreadEvent);
            }
        }

        return 1;
    }

    DEFINE_MODULE_FUNCTION( PostSub, void, (const char* buff, CScriptTable& tb_msg))
    {
        if( tb_msg.IsReferd() )
        {
            std::string     thread_name;
            lua_Integer     from;
            CThreadEvent*   pThreadEvent = new CThreadEvent();
            lua_Integer     buf_len;

            tb_msg.Get(1, thread_name);
            tb_msg.Get(2, from);
            tb_msg.Get(3, pThreadEvent->m_EventType);
            tb_msg.Get(4, buf_len);

            pThreadEvent->m_From = from;
            pThreadEvent->m_Data.assign( buff, buf_len );

            CLuaThread* pThread = LuaThreadMgr.GetLuaThread(thread_name);

            if( pThread != NULL )
            {
                pThread->PostSub(pThreadEvent);
            }
        }

        return 1;
    }


    END_SCRIPT_MODULE()


}

