local Client = class("Client")

-- 构造函数;
function Client:ctor()
	self.SockID     = nil;
	self.ConPLS     = nil;
	self.UID        = nil;

    self._LastHeartbeat = TimerMgr:GetTime();
    self._TimerHandle   = TimerMgr:AddTimer(7000, self, self.CheckTimeout);
end

function Client:ResetHeartbeat()
    self._LastHeartbeat = TimerMgr:GetTime();
end

function Client:CheckTimeout()
    if TimerMgr:GetTime() - self._LastHeartbeat > 1500000 then
        ClientService:DisConnect(self.SockID);
        -- nlinfo("CheckTimeout:DisConnect  "..self.SockID);
    else
        self._TimerHandle = TimerMgr:AddTimer(7000, self, self.CheckTimeout);
        nlinfo("CheckTimeout");
    end
end

function Client:Release()
    TimerMgr:RemoveTimer(self._TimerHandle);
end

return Client;
