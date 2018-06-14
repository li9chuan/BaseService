--========================================================= 
-- 初始化基础通用工具
--=========================================================

local BasePath = Misc.GetBasePath() .. "/script/";
package.path = package.path .. BasePath .. "Framework/?.lua;";
package.path = package.path .. BasePath .. "Framework/Event/?.lua;";
package.path = package.path .. BasePath .. "Framework/Net/?.lua;";


protobuf    = require "protobuf"
Json        = require "cjson"
JsonUtil    = require "CJsonUtil"


-- 工具库
require("Class")
require("functions")

require("Map")
require("MapMap")

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
HotfixHelper                = require("Hotfix/HotfixHelper");

enum = {}


-- 初始化单例
function OnInitFramework()

    addr = io.open( BasePath.."DataTable/ProtoMsg.pb", "rb")
    buffer = addr:read "*a"  
    addr:close()  
    protobuf.register(buffer) 
    local decode = protobuf.decode("google.protobuf.FileDescriptorSet", buffer)

    for _,v in ipairs(decode.file) do
        if v.enum_type ~= nil then
            for _,etp in ipairs(v.enum_type) do
                local enum_val = etp.value;
                for _,eval in ipairs(enum_val) do
                    --nlinfo( "    " .. eval.name .. "  " .. eval.number );
                    enum[eval.name] = eval.number;
                end
            end
        end
    end

    --math.randomseed(tostring(os.time()):reverse():sub(1, 7)) 
    
    local seed = os.time() + (Misc.GetLocalTime()*100000);
    math.randomseed(seed) 

    EventController.Instance():Init()
    TimerMgr:Init( Misc.GetLocalTime() );
    
    Hotfix = HotfixHelper:new();
    Hotfix:Init();
end


OnInitFramework();
