--========================================================= 
-- 加载常用模块
--=========================================================

local BasePath = Misc.GetBasePath() .. "/script/";
package.path = BasePath .. "_ClientRobot/?.lua;" .. BasePath .. "SharedLib/?.lua;";


require("InitSharedLib")



-- 主入口函数。从这里开始lua逻辑
function ServiceInit()

    print("Lua Robot Init");

    
    
    bin_types.LuaThread.NewInstance();
    
end

-- 主循环
function ServiceUpdate()


end

function ServiceRelease()


    print("Lua Release.");
end

function ServiceInfo()
    

    
end

