ScheduleService = {}

function ScheduleService:Init()
	self._EventRegister = EventRegister.new();
	
	self._EventRegister:RegisterEvent( "FESConnection",         self, self.CbConnection );
	self._EventRegister:RegisterEvent( "FESDisConnection",      self, self.CbDisConnection );
    self._EventRegister:RegisterEvent( "PLSConnection",         self, self.CbConnection );
	self._EventRegister:RegisterEvent( "PLSDisConnection",      self, self.CbDisConnection );
    
    -- 注册其它服务器启动的回调
    ServerNet.SetConnectionCallback("FES");
    ServerNet.SetDisConnectionCallback("FES");
    
    ServerNet.SetConnectionCallback("PLS");
    ServerNet.SetDisConnectionCallback("PLS");
    
end

function ScheduleService:CbConnection( service_id, service_name )
	print("ScheduleService:CbConnection:"..service_name.." sid:"..service_id);
end

function ScheduleService:CbDisConnection( service_id, service_name )
	print("ScheduleService:CbDisConnection"..service_name.." sid:"..service_id);
end

--	释放函数
function ScheduleService:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end

return ScheduleService

