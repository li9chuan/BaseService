-- Player PLS Info
local PlayerInfo = class("PlayerInfo")

-- 构造函数;
function PlayerInfo:ctor()
	self.PLSID      = nil;
	self.UID        = nil;
end

return PlayerInfo;
