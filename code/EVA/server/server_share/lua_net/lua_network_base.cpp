#include "lua_network_base.h"
#include "lua_message.h"
#include <server_share/lua/script_mgr.h>

using namespace bin;
using namespace NLNET;

static CLuaMessage* pLuaMsg = new CLuaMessage();

void cbLuaServiceMsg ( CMessage &msgin, TSockId from, CCallbackNetBase &netbase )
{
    CScriptTable    functbl;
    ScriptMgr.GetScriptHandle()->Get("NetWorkHandler", functbl);

    int nRet = 0;
    pLuaMsg->m_Msg.swap(msgin);

    functbl.CallFunc<lua_Integer, CLuaMessage*, int>("OnMessage", (lua_Integer)from, pLuaMsg, nRet);
}

