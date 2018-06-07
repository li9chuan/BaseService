local BasePath = Misc.GetBasePath() .. "/script/";
package.path = BasePath .. "__Robot/RobotSub/?.lua;" .. BasePath .. "Framework/?.lua;";


require("InitFramework")
require("RobotMgr")
require("PublicRoomInfoMgr")

FSMRobot                = require("FSMRobot")
RobotGameBase           = require("RobotGameBase")
RobotData               = require("RobotData")
Robot                   = require("Robot")
PublicRoomInfo          = require("PublicRoomInfo")

FSMDdz                  = require("GameDdz/FSMDdz")
RobotGameDdz            = require("GameDdz/RobotGameDdz")

print("-=======DBSubStart==========-");


G_ThreadHandle = 999;

function ThreadInit( thread_handle, params )

    print("Lua ThreadInit:".. thread_handle);

    print(params);
    
    G_ThreadHandle = thread_handle;

    PublicRoomInfoMgr:Init();
    RobotMgr:Init();

    RobotMgr:StartLogic(1, 1);
end



function ThreadUpdate()
    TimerMgr:Update( Misc.GetLocalTime() );
    RobotMgr:Update();
end

