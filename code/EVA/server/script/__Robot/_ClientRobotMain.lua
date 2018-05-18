--========================================================= 
-- 加载常用模块
--=========================================================

local BasePath = Misc.GetBasePath() .. "/script/";
package.path = BasePath .. "__Robot/?.lua;" .. BasePath .. "SharedLib/?.lua;";


require("InitSharedLib")

require("RobotMgr");

-- 主入口函数。从这里开始lua逻辑
function ServiceInit()

    print("Lua Robot Init");
    
    RobotMgr:Init();
    
    RobotMgr:Strat();
    
    
    
end

-- 主循环
function ServiceUpdate()


end

function ServiceRelease()

    RobotMgr:Release();
    print("Lua Release.");
end

function ServiceInfo()
    

    
end

