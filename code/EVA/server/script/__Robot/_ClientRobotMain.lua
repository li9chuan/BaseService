--========================================================= 
-- 加载常用模块
--=========================================================

local BasePath = Misc.GetBasePath() .. "/script/";
package.path = BasePath .. "__Robot/?.lua;" .. BasePath .. "SharedLib/?.lua;";


require("InitSharedLib")
require("ThreadMgr");

--CppTimerBase = require("Test/CppTimerBase");
--CppTimerTest = require("Test/CppTimerTest");


-- 主入口函数。从这里开始lua逻辑
function ServiceInit()

    nlinfo("Lua Robot Init");
    
    ThreadMgr:Init();
    
    ThreadMgr:Strat();
    

    
end

-- 主循环
function ServiceUpdate()

    local curr_tick = Misc.GetLocalTime();
    TimerMgr:Update( curr_tick );
end

function ServiceRelease()

    ThreadMgr:Release();
    nlinfo("Lua Release.");
end

function ServiceInfo()
    

    
end

