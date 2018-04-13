-- Player SCH Info
local PlayerInfo = class("PlayerInfo")

-- 构造函数;
function PlayerInfo:ctor()
	self.ConPLS         = nil;
    self.ConFES         = nil;
	self.UID            = nil;
    self.IsOffline      = false;
    self.GameID         = nil;
    self.LogoutTime     = nil;
    self.TimerID        = nil;
    self.RoomType       = nil;
end



function PlayerInfo:RemoveCachePlayerTimer()
    
    if self.LogoutTime then
    
        --self.TimerID = TimerMgr:AddTimer(7000, self, self.RemoveCachePlayerTimer);
    else
        --TimerMgr:RemoveTimer(self.TimerID);
    end
    
end

function PlayerInfo:OnRelease()


end

--self.TimerID = TimerMgr:AddTimer(7000, self, self.UpdatePLSInfoTimer);
return PlayerInfo;
