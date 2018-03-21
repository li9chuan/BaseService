#include <server_share/server_def.h>
#include <nel/misc/path.h>
#include <string>

#include "lua_base_function.h"
#include "lua_engine.h"
#include "script_mgr.h"
#include "static_table/static_table_mgr.h"

#include "server_share/bin_luabind/Public.hpp"

using namespace std;
using namespace NLMISC;
using namespace NLNET;
using namespace DEF;
using namespace ATTRIB_TYPE;
using namespace StaticData;

int  GetCurrentDay1970( uint32 time )
{
    int res = 0;
    int sec_day = 60 * 60 * 24;
    if ( time == 0 )
    {
        res = CTime::getSecondsSince1970() / sec_day;
    }
    else
    {
        res = time / sec_day;
    }
    return res;
}

void LoadLuaFile(const char* szFileName)
{
    string script = NLMISC::CPath::lookup(string(szFileName));

    if ( script.size() > 0 )
    {
        ScriptMgr.LoadScrpit(script.c_str());
    }
}

void RegScriptEvent( const char* script_scope, int event_id )
{
    ScriptMgr.register_event(script_scope,event_id);
}

int LuaAddSearchPath( lua_State *L )
{
    int n = lua_gettop(L);

    //if( n == 1)

    const char *pPath = lua_tostring(L, -1);    //将返回值从栈中读出
    lua_pop(L, 1);                              //弹出返回值


    lua_getglobal(L, "package");  
    lua_getfield(L, -1, "path"); 
    std::string v;
    v.append(lua_tostring(L, -1)); 
    v.append(";");
    v.append(NLMISC::CPath::getCurrentPath());
    v.append("/");
    v.append(pPath);  
    lua_pushstring(L, v.c_str());  
    lua_setfield(L, -3, "path");  
    lua_pop(L, 2);  

    return 1;
}

 namespace bin
 {
     BEGIN_SCRIPT_MODULE(Utility)
 
     DEFINE_MODULE_FUNCTION(TestPB, void, (const char* buff, int len))
     {
         MsgSession  msg_session;
         msg_session.ParseFromArray(buff,len);

         std::string  msg_name = "LuaTestCB";
         std::string  msg_buff = "ddd111dd";
         msg_session.SerializeToString(&msg_buff);





         LuaParams lua_params( msg_name, msg_buff );  
         ScriptMgr.run( "NetWorkHandler", "OnMessage", lua_params);

         //m_LuaEngine.RunLuaFunction( "OnMessage", "NetWorkHandler", NULL, &lua_params );


         return 1;
     }

     DEFINE_MODULE_FUNCTION(GetBasePath, std::string, (void))
     {
         //nldebug("Calling func5 with (%s, %lld)\n", path.c_str(), a1);
         r = NLMISC::CPath::getCurrentPath();
         return 1;
     }
 
     END_SCRIPT_MODULE()
 }

int LuaPrint(lua_State *L)
{
    int n = lua_gettop(L);          //传进来的参数个数
    int i;
    //	const char* str = lua_tostring(L,-1);
    lua_getglobal(L, "tostring");   //将脚本变量tostring（脚本库函数）压栈
    for (i=1; i<=n; i++)
    {
        const char *s;
        lua_pushvalue(L, -1);       //将栈顶的变量，即为tostring 函数再次压栈
        lua_pushvalue(L, i);        //将要打印的值，也就是脚本中调用时传入的参数压栈
        lua_call(L, 1, 1);          //调用脚本函数tostring
        s = lua_tostring(L, -1);    //将返回值从栈中读出
        if (s == NULL)
            return luaL_error(L, "`tostring' must return a string to `print'");
        lua_pop(L, 1);  //弹出返回值

        nldebug("%s",s);
    }
    return 0;
}


namespace bin
{
    BEGIN_SCRIPT_MODULE(ServerNet)

    DEFINE_MODULE_FUNCTION(Broadcast, void, (string service_name, CScriptTable& tb_msg))
    {
        if( tb_msg.IsReferd() )
        {
            std::string  msg_name;
            std::string  msg_body;

            tb_msg.Get(1, msg_name);
            tb_msg.Get(2, msg_body);

            CMessage msg_out("L");
            msg_out.serial(msg_name);
            msg_out.serial(msg_body);

            Network->send( service_name, msg_out );
        }

        return 1;
    }

    DEFINE_MODULE_FUNCTION(Send, void, (int sid, CScriptTable& tb_msg))
    {
        if( tb_msg.IsReferd() )
        {
            string  msg_name;
            string  msg_body;

            tb_msg.Get(1, msg_name);
            tb_msg.Get(2, msg_body);

            CMessage msg_out("_L");
            msg_out.serial(msg_name);
            msg_out.serial(msg_body);

            Network->send( (NLNET::TServiceId)sid, msg_out );
        }

        return 1;
    }

    END_SCRIPT_MODULE()
}


#include "nel/net/callback_server_websocket.h"



class CLuaWebSocketNetwork
{
    DECLARE_SCRIPT_CLASS();
public:

    void Hello()
    {
        nldebug("CLuaWebSocketNetwork::Hello");


        //ScriptMgr.on_event()    
        //NetWorkHandler.OnMessage( data )

    }

    CLuaWebSocketNetwork( std::string name, uint16 port )
    {
        m_NetName = name;
        CCallbackServerWebSocket* pCallbackServerHandle = new CCallbackServerWebSocket();
        pCallbackServerHandle->init (port);

        m_CallbackServerHandle = pCallbackServerHandle;
    }

    ~CLuaWebSocketNetwork()
    {
        //delete m_CallbackServerHandle; 
    }

    void Update()
    {
        
    }

    void Say(const std::string& msg)
    {
        printf(msg.c_str());
    }

    std::string Want()
    {
        return "World";
    }

private:

    std::string                 m_NetName;
    CCallbackNetBase*           m_CallbackServerHandle;
};


namespace bin
{
    BEGIN_SCRIPT_CLASS( WebSocketNetwork, CLuaWebSocketNetwork )
        DEFINE_CLASS_FUNCTION(hello, void, ())
        {
            obj->Hello();
            return 1;
        }

        DEFINE_CLASS_FUNCTION(say, void, (const std::string& message))
        {
            obj->Say(message);
            return 1;
        }

        DEFINE_CLASS_FUNCTION(want , std::string, ())
        {
            r = obj->Want();
            return 1;
        }

        DEFINE_STATIC_FUNCTION(newInstance, CLuaWebSocketNetwork*, (std::string name, int port))
        {
            r = new CLuaWebSocketNetwork(name, port);
            r->GetScriptObject().SetDelByScr(true);

            return 1;
        }
    END_SCRIPT_CLASS()
}

void cbLuaMsg ( NLNET::CMessage& msgin, const std::string &serviceName, NLNET::TServiceId serviceId )
{
    std::string  msg_type;
    std::string  msg_body;

    msgin.serial(msg_type);
    msgin.serial(msg_body);

    LuaParams lua_params( msg_type, msg_body );  
    ScriptMgr.run( "NetWorkHandler", "OnMessage", lua_params);
}

NLNET::TUnifiedCallbackItem LuaCallbackArray[] =
{
    { "_L",                        cbLuaMsg              },
};


class CLuaWebSocketNetworkMgr : public NLMISC::CSingleton<CScriptMgr>
{
public:

    void Init()
    {
        NLNET::CUnifiedNetwork::getInstance()->addCallbackArray(LuaCallbackArray, sizeof(LuaCallbackArray)/sizeof(LuaCallbackArray[0]));
    }

    void RegisterNetModule( std::string name, CLuaWebSocketNetwork* pNet )
    {
        TNetHandle::iterator iter = m_LuaWebSocketNetworkHandle.find(name);

        if ( iter != m_LuaWebSocketNetworkHandle.end() )
        {
            m_LuaWebSocketNetworkHandle.insert( make_pair(name,pNet) ); 
        }
        else
        {
            nlstop;
        }
    }

    void Update()
    {
        TNetHandle::iterator iter = m_LuaWebSocketNetworkHandle.begin();

        while (iter!=m_LuaWebSocketNetworkHandle.end())
        {

            iter->second->Update();
            ++iter;
        }
    }

    void Release()
    {
        TNetHandle::iterator iter = m_LuaWebSocketNetworkHandle.begin();

        while (iter!=m_LuaWebSocketNetworkHandle.end())
        {

            delete iter->second;
            ++iter;
        }

        m_LuaWebSocketNetworkHandle.clear();
    }


private:

    typedef std::map<std::string, CLuaWebSocketNetwork*>    TNetHandle;
    TNetHandle          m_LuaWebSocketNetworkHandle;
};

 #define  LuaWebSocketNetworkMgr  CLuaWebSocketNetworkMgr::instance()
