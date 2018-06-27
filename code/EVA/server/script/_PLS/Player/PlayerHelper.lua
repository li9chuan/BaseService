local PlayerHelper = class("PlayerHelper")

-- 构造函数;
function PlayerHelper:ctor()
    
    self.PlayerDataHelper      = nil;


    self.UID                    = 0;
    self.ConFES                 = nil;
     
    
    self.IsRobot                = false;
    
    self.RoomID                 = 0;
    self.JoinRoomTime           = 0;

    self.OfflineTime            = 0;
    self.LogoutTime             = nil;
    self.PlayerState            = nil;
        
    self.LastUpdateTime         = TimerMgr:GetTime();
    self._TimerHandle           = TimerMgr:AddTimer( 30000, self, self.TickUpdate );
end

function PlayerHelper:TickUpdate()

    local curr_time     = TimerMgr:GetTime();

    if curr_time - self.LastUpdateTime > 7*24*60*60*1000 then
        self._TimerHandle = nil;    -- Release 时不再删除定时器
        PlayerMgr:RemovePlayer(self.UID);
    else
        self._TimerHandle = TimerMgr:AddTimer( 10*60*1000, self, self.TickUpdate );
    end
end

-- 从缓存中移除玩家,由PlayerMgr调用。
function PlayerHelper:Release()

    local msg = CMessage("RemovePlayer");
    msg:wint(self.UID);
    BaseService:Broadcast("SCH", msg);

    if self._TimerHandle ~= nil then
        TimerMgr:RemoveTimer(self._TimerHandle);
    end
end

function PlayerHelper:Offline()
    self.OfflineTime            = TimerMgr:GetTime();
    self.ConFES                 = nil;
    
    if self.RoomID>0 then
        local room = RoomMgr:GetRoom(self.RoomID);
        
		if room~=nil then
			room:PlayerOffline( self.UID );
		else
			nlwarning("room==nil");
		end
    end

    nlwarning("Offline================");
end
function PlayerHelper:Online()
    self.OfflineTime            = 0;
end



return PlayerHelper;
