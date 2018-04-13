--========================================================= 
-- 初始化游戏内共享工具，游戏逻辑相关。
--=========================================================

local BasePath = Utility.GetBasePath() .. "/script/";
package.path = package.path .. BasePath .. "Framework/?.lua;";

require("InitFramework")


require("StaticTableMgr");
require("Event/EventType");





-- 初始化单例
function OnInitSharedLib()
	
    StaticTableMgr:Init();
    
end



OnInitSharedLib();


