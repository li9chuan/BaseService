local Client = class("Client")

-- 构造函数;
function Client:ctor()
	self.SockID     = 0;
	self.PLSID      = 0;
	self.UID        = 0;
end


function Client:LuaTest()

	
    
    print("----------"..self.SockID);

end



return Client;