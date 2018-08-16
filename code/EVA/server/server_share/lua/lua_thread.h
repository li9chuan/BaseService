#ifndef SERVER_SHARD_LUA_THREAD_H
#define SERVER_SHARD_LUA_THREAD_H

#include <nel/misc/singleton.h>
#include <nel/misc/mutex.h>
#include <server_share/buf_fifo2.h>
#include <server_share/bin_luabind/Public.hpp>
#include <server_share/lua/lua_engine.h>
#include <server_share/lua_net/lua_message.h>
#include <nel/net/message.h>

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

    CLuaThread( std::string name, sint32 update_tick);
    virtual ~CLuaThread(void);


    /**
    *  启动线程
    *  @param lua_start     子线程lua脚本加载路径。
    *  @param params        传递参数。
    *  @return   lua_thread_handle
    */
    ///@{
    sint32 Start(std::string lua_start, std::string params);
    ///@}


    // close;
    void Close( void );

	// 主线程调用，读取 m_ToMainEvent
    void Update( void );

    void PostSub(NLNET::CMessage* pMsg)     { m_ToSubEvent.push_back(pMsg);  }
    void PostMain(NLNET::CMessage* pMsg )   { m_ToMainEvent.push_back(pMsg); }

    CLuaEngine&     GetLuaEngine() { return m_SubLuaEngine; }
protected:

    // sub threads run;
    void run( void );

private:

    NLMISC::IThread*            m_ThreadHandle;
    CLuaEngine		            m_SubLuaEngine;
    std::string                 m_ThreadName;
    sint32                      m_LuaThreadHandle;
    bool                        m_AlreadyStarted;
    sint32                      m_UpdateTick;

    volatile bool               m_RequireExit;


    NLMISC::CBufFIFO2< NLNET::CMessage >    m_ToSubEvent;
    NLMISC::CBufFIFO2< NLNET::CMessage >    m_ToMainEvent;

    CLuaMessage                 m_LuaMainMsg;
    CLuaMessage                 m_LuaSubMsg;

};

class CLuaThreadMgr : public NLMISC::CSingleton<CLuaThreadMgr>
{
public:
    CLuaThreadMgr() {}

    sint32 RegisterLuaThread( CLuaThread* pThread );
    void RemoveLuaThread( sint32 lua_handle );
    CLuaThread* GetLuaThread( sint32 lua_handle);

    void Init();

    void Update();

    void Release();



private:

    typedef std::vector<CLuaThread*>        TThreadHandles;

    TThreadHandles      m_LuaThreadHandles;
    NLMISC::CMutex      m_LuaThreadMutex;
};

#define  LuaThreadMgr  CLuaThreadMgr::instance()

#endif



