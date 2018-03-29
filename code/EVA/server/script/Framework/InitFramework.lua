--========================================================= 
-- 初始化
--=========================================================

local BasePath = Utility.GetBasePath() .. "/script/";
package.path = package.path .. BasePath .. "Framework/?.lua;";
package.path = package.path .. BasePath .. "Framework/Event/?.lua;";
package.path = package.path .. BasePath .. "Framework/Net/?.lua;";

protobuf = require "protobuf"

addr = io.open( BasePath.."DataTable/proto_msg.pb", "rb")
buffer = addr:read "*a"  
addr:close()  
protobuf.register(buffer) 
protobuf.decode("google.protobuf.FileDescriptorSet", buffer)


-- 工具库
require("Class")
--require("Framework/Stack")
require("functions")
--require ("bit")

require("NetWorkHandler")




--List 	= require("Common/List")
--Queue   = require("Common/Queue")
--Map     = require("Common/Map")

-- protobuf
--require ("Common/ProtoBuffer/define_attrib_pb")

--[[
-- LuaFramework --
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
EventController				= require("Event/EventController")
--ServerManager				= require("Net/ServerManager")
EventRegister				= require("Event/EventRegister")



-- 通用模块
function OnCommonInit()
	EventController.Instance():Init()
--	ServerManager.Instance():Init()
--	UIManager.Instance():Init()
--	SceneManager.Instance():Init()
end

-- 初始化单例
function OnInit()
	OnCommonInit();		-- 通用模块
--	OnLoginInit();		-- 登录模块
end

OnInit();
