--========================================================= 
-- 加载常用模块
--=========================================================

local BasePath = Misc.GetBasePath() .. "/script/";
package.path = BasePath .. "_SCH/?.lua;" .. BasePath .. "SharedLib/?.lua;";


require("InitSharedLib")
require("ScheduleService")
require("PLSInfo/PLSInfoMgr")
require("Player/PlayerInfoMgr")

MsgLogin            = require("Msg/MsgLogin")
MsgRoom             = require("Msg/MsgRoom")

PLSGameInfo         = require("PLSInfo/PLSGameInfo")
PLSInfo             = require("PLSInfo/PLSInfo")
PlayerInfo          = require("Player/PlayerInfo")

RoomInfo            = require("Room/RoomInfo")
RoomMgr             = require("Room/RoomMgr")
RoomIDAlloter       = require("Room/RoomIDAlloter")


PHPService          = CallbackServer:new();



-- 主入口函数。从这里开始lua逻辑
function ServiceInit()
	
    print("Lua Start.");
    
    MsgLogin:Init();
    MsgRoom:Init();
    
    RoomMgr:Init();
    PLSInfoMgr:Init();
    PlayerInfoMgr:Init();
    ScheduleService:Init();
    
    PHPService:Init( "PHP", "tcp" );
    PHPService:Listen( 20458 );

end

-- 游戏循环
function ServiceUpdate()
    TimerMgr:Update(math.floor(os.clock() * 1000));
end

function ServiceRelease()
    
    PLSInfoMgr:Release();
    PlayerInfoMgr:Release();
    ScheduleService:Release();
    print("Lua Release.");
end


function ServiceInfo()
    
    PrintTable(PlayerInfoMgr.PlayerInfoMap);
end


