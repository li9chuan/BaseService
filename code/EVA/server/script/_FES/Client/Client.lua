local Client = class("Client")

-- 构造函数;
function Client:ctor()
	self.SockID     = 0;
	self.PLSID      = 0;
	self.UID        = 0;
end


return Client;
