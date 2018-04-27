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
    self.RoomType       = "";
    self.RoomID         = 0;
end



function PlayerInfo:RemoveCachePlayerTimer()
    
    if self.LogoutTime then
    
        --self.TimerID = TimerMgr:AddTimer(7000, self, self.RemoveCachePlayerTimer);
    else
        --TimerMgr:RemoveTimer(self.TimerID);
    end
    
end

function PlayerInfo:Release()


end

--self.TimerID = TimerMgr:AddTimer(7000, self, self.UpdatePLSInfoTimer);
return PlayerInfo;
