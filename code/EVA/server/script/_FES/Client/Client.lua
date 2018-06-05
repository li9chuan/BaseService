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

-- 通知逻辑服务器，客户端网络断开。
function Client:_NotifyClientOffline()
    if  self.ConPLS ~= nil  then
        local msg = CMessage("ClientOffline");
        msg:wint(self.UID)
        BaseService:Send( self.ConPLS, msg )
    end
end

function Client:Release()

    nlinfo("Client:Release  UID:" .. self.UID);
    self:_NotifyClientOffline();
    TimerMgr:RemoveTimer(self._TimerHandle);
end

return Client;
