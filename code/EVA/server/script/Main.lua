--========================================================= 
-- 加载常用模块
--=========================================================

BasePath = Utility.GetBasePath() .. "/script/";
package.path = BasePath .. "?.lua;" .. BasePath .. "Framework/?.lua;";
package.path = package.path .. BasePath .. "Framework/Event/?.lua;";
package.path = package.path .. BasePath .. "Framework/Net/?.lua;";

require("Init")

OnInit();

LuaTestMsg = require("PLS/LuaTestMsg")

lua_test = LuaTestMsg:new();


-- 主入口函数。从这里开始lua逻辑
function Init()
	
	
	msg_session = {  
		check = 13,  
		session = 4611686020574871551,  
		index = 21
	}

	code = protobuf.encode("MsgSession", msg_session)
	Utility.TestPB(code, string.len(code))
	
--	AddSearchPath( "?.lua" );
--	AddSearchPath( "Framework/?.lua" );
	

	
--	msg = { "LTst", "neirong" }
--	ServerNet.Send( "PLS", msg );
	
	
	local web_sock = bin_types.WebSocketNetwork.newInstance();
	web_sock:hello();

	
end

-- 游戏循环
function Update()

end

function Release()

end

--[[




--bash_path = "E:\\BaseService\\code\\EVA\\server\\script\\";
--package.path = bash_path .. "Framework\\?.lua;" .. bash_path .. "Framework\\Net\\?.lua;";

print(package.path);

local protobuf = require "protobuf"

addr = io.open( bash_path.."DataTable\\proto_msg.pb", "rb")
buffer = addr:read "*a"  
addr:close()  
  
protobuf.register(buffer)  

t = protobuf.decode("google.protobuf.FileDescriptorSet", buffer) 

player_info = {  
    name = "Alice",  
    pid = 12345,  
    view_player_list = {  
        { pid = 17712345678, head_portrait = 1 },  
        { pid = 17712345679, head_portrait = 2 },  
    },  
    level = 2
}


code = protobuf.encode("MsgPlayerInfo", player_info)
decode = protobuf.decode("MsgPlayerInfo" , code)

print(decode.name)
print(decode.pid)

for _,v in ipairs(decode.view_player_list) do
	print("\t"..v.pid, v.head_portrait)
end

]]