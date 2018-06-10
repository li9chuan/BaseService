FrontEndService = {}

function FrontEndService:Init()
	self._EventRegister = EventRegister.new();
	
	self._EventRegister:RegisterEvent( "FESCon",            self, self.Connection );
	self._EventRegister:RegisterEvent( "FESDis",            self, self.DisConnection );
    self._EventRegister:RegisterEvent( "PLSCon",            self, self.Connection );
	self._EventRegister:RegisterEvent( "PLSDis",            self, self.DisConnection );
    
    -- 注册其它服务器启动的回调
    Net.SetConnectionCallback("FES");
    Net.SetDisConnectionCallback("FES");
    
    Net.SetConnectionCallback("PLS");
    Net.SetDisConnectionCallback("PLS");
    
end


function FrontEndService:Connection( service_id, service_name )
	nlinfo("FrontEndService:Connection:"..service_name.." sid:"..service_id);
end

function FrontEndService:DisConnection( service_id, service_name )
	nlinfo("FrontEndService:DisConnection"..service_name.." sid:"..service_id);
end

--	释放函数
function FrontEndService:Release()
    self._EventRegister:UnRegisterAllEvent();
end

return FrontEndService

