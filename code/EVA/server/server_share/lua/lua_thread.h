#ifndef SERVER_SHARD_LUA_THREAD_H
#define SERVER_SHARD_LUA_THREAD_H

#include <nel/misc/singleton.h>
#include <nel/misc/mutex.h>
#include <game_share/buf_fifo2.h>
#include <server_share/bin_luabind/Public.hpp>
#include <server_share/lua/lua_engine.h>

namespace bin{
    class CScriptHandle;
}

class CThreadEvent
{
public:
    uint64          m_From;
    std::string     m_Data;
    std::string     m_EventType;

};



class CLuaThread : public NLMISC::IRunnable
{
    DECLARE_SCRIPT_CLASS();

public:

    CLuaThread( std::string name );
    virtual ~CLuaThread(void);


    // start;
    bool Start( std::string& lua_start, std::string& export_name );

    // close;
    void Close( void );

    void Update( void );

    void PostSub( CThreadEvent* pThreadEvent )  { m_ToSubEvent.push_back(pThreadEvent); }
    void PostMain( CThreadEvent* pThreadEvent ) { m_ToMainEvent.push_back(pThreadEvent); }

    CLuaEngine&     GetLuaEngine() { return m_SubLuaEngine; }
protected:

    // sub threads run;
    void run( void );

private:

    NLMISC::IThread*            m_ThreadHandle;
    CLuaEngine		            m_SubLuaEngine;
    std::string                 m_ThreadName;

    volatile bool               m_RequireExit;

    NLMISC::CBufFIFO2< CThreadEvent > m_ToSubEvent;
    NLMISC::CBufFIFO2< CThreadEvent > m_ToMainEvent;

};

class CLuaThreadMgr : public NLMISC::CSingleton<CLuaThreadMgr>
{
public:
    CLuaThreadMgr() : m_LuaThreadHandle("CLuaThreadMgr:m_LuaThreadHandle") {}

    void RegisterLuaThread( std::string& name, CLuaThread* pThread );
    void RemoveLuaThread( std::string& name );
    CLuaThread* GetLuaThread( std::string& name );

    void Init();

    void Update();

    void Release();



private:

    typedef std::map<std::string, CLuaThread*>      TThreadHandle;

    NLMISC::CSynchronized<TThreadHandle>            m_LuaThreadHandle;
    NLMISC::CMutex      m_LuaThreadMutex;
};

#define  LuaThreadMgr  CLuaThreadMgr::instance()

#endif



