--========================================================= 
-- 初始化基础通用工具
--=========================================================

local BasePath = Utility.GetBasePath() .. "/script/";
package.path = package.path .. BasePath .. "Framework/?.lua;";
package.path = package.path .. BasePath .. "Framework/Event/?.lua;";
package.path = package.path .. BasePath .. "Framework/Net/?.lua;";


protobuf    = require "protobuf"

addr = io.open( BasePath.."DataTable/ProtoMsg.pb", "rb")
buffer = addr:read "*a"  
addr:close()  
protobuf.register(buffer) 
protobuf.decode("google.protobuf.FileDescriptorSet", buffer)


Json        = require "cjson"
JsonUtil    = require "CJsonUtil"


-- 工具库
require("Class")
--require("Framework/Stack")
require("functions")

--require ("bit")

require("Net/NetWorkHandler")
require("Net/BaseService")
require("TimerMgr");
require("Event/EventTrigger");


--List 	= require("Common/List")
--Queue   = require("Common/Queue")
--Map     = require("Common/Map")

-- protobuf
--require ("Common/ProtoBuffer/define_attrib_pb")


-- LuaFramework --
IDGenerate                  = bin_types.IDGenerate;
MysqlStmt                   = bin_types.MysqlStmt;
MysqlConn                   = bin_types.MysqlConn;
MysqlResult                 = bin_types.MysqlResult;
CMessage                    = bin_types.LuaMessage.NewInstance;

print                       = Debug.Info;
nldebug                     = Debug.Debug;
nlinfo                      = Debug.Info;
nlwarning                   = Debug.Warning;
nlstop                      = Debug.Stop;

--[[
Util						= LuaFramework.Util;
NativeUtil					= LuaFramework.NativeUtil;
AppConst					= LuaFramework.AppConst;
LuaHelper					= LuaFramework.LuaHelper;
NetMessage					= LuaFramework.NetMessage;
RecordNodeTable				= LuaFramework.RecordNodeTable;

-- LuaHelper --
NetWorkHelper				= LuaHelper.GetNetManager();
GameManager					= LuaHelper.GetGameManager();
PoolManager					= LuaHelper.GetPoolManager();
LuaManager					= LuaHelper.GetLuaManger();

--]]

-- 表
--require("Event/EventType")



-- 工具


--LSModule					= require("SDK/LService/LSModule")
--WebModule					= require("SDK/Web/WebModule")
--HttpClient					= require("Common/Net/HttpClient")

EventController             = require("Event/EventController")
--ServerManager             = require("Net/ServerManager")
EventRegister               = require("Event/EventRegister")
CallbackServer              = require("Net/CallbackServer");
StateMachine                = require("SimpleStateMachine");





-- 初始化单例
function OnInitFramework()
    EventController.Instance():Init()
    TimerMgr:Init(os.clock()*1000);
end


OnInitFramework();
