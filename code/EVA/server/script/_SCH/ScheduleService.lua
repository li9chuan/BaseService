ScheduleService = {}

function ScheduleService:Init()
	self._EventRegister = EventRegister.new();
	
	self._EventRegister:RegisterEvent( "FESCon",            self, self.CbConnection );
	self._EventRegister:RegisterEvent( "FESDis",            self, self.CbFESDisConnection );
    self._EventRegister:RegisterEvent( "PLSCon",            self, self.CbConnection );
	self._EventRegister:RegisterEvent( "PLSDis",            self, self.CbPLSDisConnection );
    
    -- 注册其它服务器启动的回调
    Net.SetConnectionCallback("FES");
    Net.SetDisConnectionCallback("FES");
    
    Net.SetConnectionCallback("PLS");
    Net.SetDisConnectionCallback("PLS");
    
    print("ScheduleService:Init");
end

function ScheduleService:CbConnection( service_id, service_name )
	print("ScheduleService:CbConnection:"..service_name.." sid:"..service_id);
end

function ScheduleService:CbFESDisConnection( service_id, service_name )
	print("ScheduleService:CbFESDisConnection"..service_name.." sid:"..service_id);
end

function ScheduleService:CbPLSDisConnection( service_id, service_name )
    PlayerInfoMgr.RemovePLS(service_id);
end

--	释放函数
function ScheduleService:Release()
    self._EventRegister:UnRegisterAllEvent();
end

return ScheduleService

