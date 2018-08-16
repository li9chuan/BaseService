#include "lua_thread.h"
#include <nel/misc/thread.h>
#include "script_mgr.h"
#include <server_share/server_def.h>
#include <server_share/lua_net/lua_message.h>

using namespace NLMISC;
using namespace NLNET;

void forLuaThreadForceLink()
{
    nlwarning("forLuaThreadForceLink");
}

CLuaThread::CLuaThread( std::string name, sint32 update_tick ) : 
    m_ThreadHandle(NULL), m_ThreadName(name), m_AlreadyStarted(false),
    m_RequireExit(false), m_LuaThreadHandle(-1), m_UpdateTick(0)
{
    if ( update_tick>0 && update_tick<10*1000 )
    {
        m_UpdateTick = update_tick;
    }
}

CLuaThread::~CLuaThread( void )
{
    Close();
    LuaThreadMgr.RemoveLuaThread(m_LuaThreadHandle);
}
 
sint32 CLuaThread::Start( std::string lua_start, std::string params )
{
    m_ThreadHandle = NLMISC::IThread::create( this );

    if ( m_ThreadHandle != NULL )
    {
        m_SubLuaEngine.Init();

        m_SubLuaEngine.ExportModule("Misc");
        m_SubLuaEngine.ExportModule("Debug");
        m_SubLuaEngine.ExportModule("Net");

        m_SubLuaEngine.ExportClass("LuaMessage");
        m_SubLuaEngine.ExportClass("LuaThread");
        m_SubLuaEngine.ExportClass("LuaCallbackClient");

        m_SubLuaEngine.ExportClass("MysqlStmt");
        m_SubLuaEngine.ExportClass("MysqlConn");
        m_SubLuaEngine.ExportClass("MysqlResult");

        m_ToSubEvent.init(1024*1024);
        m_ToMainEvent.init(1024*1024);

        


        //GetScriptObject().SetDelByScr(false);
        //m_SubLuaEngine.GetScriptHandle()->Set( export_name.c_str(), this );

        m_SubLuaEngine.GetScriptHandle()->Exec(lua_start.c_str());

        m_LuaThreadHandle = LuaThreadMgr.RegisterLuaThread(this);

        LuaParams params(m_LuaThreadHandle, params);
        m_SubLuaEngine.RunLuaFunction("ThreadInit", NULL, NULL, &params);

        m_AlreadyStarted = true;

        m_ThreadHandle->start();
    }

    return m_LuaThreadHandle;
}

void CLuaThread::Close( void )
{
    if ( NULL != m_ThreadHandle )
    {
        m_AlreadyStarted = false;
        m_RequireExit = true;
        m_ThreadHandle->wait();
        m_ThreadHandle = NULL;
    }
}

void CLuaThread::Update( void )
{
    if (m_AlreadyStarted)
    {
        CMessage* pMsg = NULL;
        bin::CScriptTable    functbl;
        ScriptMgr.GetScriptHandle()->Get("NetWorkHandler", functbl);

        while ((pMsg = m_ToMainEvent.pop_front()) != NULL)
        {
            int nRet = 0;
            m_LuaMainMsg.m_Msg.swap(*pMsg);
            m_LuaMainMsg.m_Msg.invert();
            functbl.CallFunc<lua_Integer, CLuaMessage*, int>("OnMessage", (lua_Integer)m_LuaThreadHandle, &m_LuaMainMsg, nRet);

            SAFE_DELETE(pMsg);
        }
    }
}

void CLuaThread::run( void )
{
    bin::CScriptTable    functbl;
    m_SubLuaEngine.GetScriptHandle()->Get("NetWorkHandler", functbl);
    NLMISC::TTime last_time = CTime::getLocalTime();

    while ( !m_RequireExit )
    {
        CMessage* pMsg = m_ToSubEvent.pop_front();

        if ( pMsg != NULL )
        {
            int nRet = 0;
            m_LuaSubMsg.m_Msg.swap(*pMsg);
            m_LuaSubMsg.m_Msg.invert();
            functbl.CallFunc<lua_Integer, CLuaMessage*, int>("OnMessage", (lua_Integer)m_LuaThreadHandle, &m_LuaSubMsg, nRet);

            SAFE_DELETE(pMsg);
        }
        else
        {
            NLMISC::nlSleep( 3 );
        }

        if (m_UpdateTick > 0)
        {
            NLMISC::TTime curr_time = CTime::getLocalTime();
            if (curr_time - last_time > m_UpdateTick)
            {
                m_SubLuaEngine.RunLuaFunction("ThreadUpdate");
                last_time = curr_time;
            }
        }
    }
}

void CLuaThreadMgr::Init()
{
    //CConfigFile::CVar* pVar = Config.getVarPtr("LuaWorkThread");

    //if ( pVar != NULL )
    //{
    //    for (uint i = 0; i < pVar->size(); ++i)
    //    {
    //        NLMISC::CSString start_file = pVar->asString(i);
    //        CVectorSString  res;
    //        start_file.splitBySeparator( ' ', res );

    //        if( res.size()!=2 ) { nlwarning( "%s, config format error.", start_file.c_str() ); continue; }

    //        NLMISC::CSString script_full_path = CPath::lookup( res[1] );

    //        if( !script_full_path.empty() )
    //        {
    //            CLuaThread* pLuaThread = new CLuaThread( res[0] );
    //            pLuaThread->Start( script_full_path, res[0] );
    //        }
    //        else
    //        {
    //            nlwarning( "%s, not found.", script_full_path.c_str() );
    //        }
    //    }
    //}
}

sint32 CLuaThreadMgr::RegisterLuaThread( CLuaThread* pThread )
{
    sint32 lua_handle = -1;

    m_LuaThreadMutex.enter();
    m_LuaThreadHandles.push_back(pThread);
    lua_handle = m_LuaThreadHandles.size()-1;
    m_LuaThreadMutex.leave();

    return lua_handle;
}

void CLuaThreadMgr::Update()
{
    m_LuaThreadMutex.enter();

    for (uint i=0; i< m_LuaThreadHandles.size(); ++i)
    {
        m_LuaThreadHandles[i]->Update();
    }

    m_LuaThreadMutex.leave();
}

void CLuaThreadMgr::Release()
{
    m_LuaThreadMutex.enter();

    for (uint i = 0; i< m_LuaThreadHandles.size(); ++i)
    {
        m_LuaThreadHandles[i]->Close();
        delete m_LuaThreadHandles[i];
    }

    m_LuaThreadHandles.clear();
    m_LuaThreadMutex.leave();
}

void CLuaThreadMgr::RemoveLuaThread( sint32 thread_handle )
{
    m_LuaThreadMutex.enter();
    m_LuaThreadHandles[thread_handle] = NULL;
    m_LuaThreadMutex.leave();
}

CLuaThread* CLuaThreadMgr::GetLuaThread( sint32 thread_handle )
{
    CLuaThread* pLuaThread = NULL;

    m_LuaThreadMutex.enter();
    pLuaThread = m_LuaThreadHandles[thread_handle];
    m_LuaThreadMutex.leave();

    return pLuaThread;
}


namespace bin
{
    BEGIN_SCRIPT_CLASS( LuaThread, CLuaThread )

    DEFINE_CLASS_FUNCTION( Post, void, (CLuaMessage* pMsgIn))
    {
		CMessage* pMsg = new CMessage();
		pMsg->swap(pMsgIn->m_Msg);
		obj->PostSub(pMsg);

        return 1;
    }

    DEFINE_CLASS_FUNCTION(Start, sint32, (std::string& lua_start, std::string& params))
    {
        r = obj->Start(lua_start, params);
        return 1;
    }

    DEFINE_STATIC_FUNCTION(NewInstance, CLuaThread*, (std::string name, sint32 update_tick))
    {
        r = new CLuaThread(name, update_tick);
        r->GetScriptObject().SetDelByScr(true);

        return 1;
    }

    END_SCRIPT_CLASS()
}

