local Client = class("Client")

-- 构造函数;
function Client:ctor()
	self.SockID     = nil;
	self.ConPLS     = nil;
	self.UID        = nil;
end


return Client;
