PlayerLogicService = {}

function PlayerLogicService:Init()
	self._EventRegister = EventRegister.new();
	
	self._EventRegister:RegisterEvent( "FESConnection",         self, self.Connection );
	self._EventRegister:RegisterEvent( "FESDisConnection",      self, self.DisConnection );
    self._EventRegister:RegisterEvent( "PLSConnection",         self, self.Connection );
	self._EventRegister:RegisterEvent( "PLSDisConnection",      self, self.DisConnection );
    
    -- 注册其它服务器启动的回调
    ServerNet.SetConnectionCallback("FES");
    ServerNet.SetDisConnectionCallback("FES");
    
    ServerNet.SetConnectionCallback("PLS");
    ServerNet.SetDisConnectionCallback("PLS");
    
end


function PlayerLogicService:Connection( service_id, service_name )
	print("PlayerLogicService:Connection:"..service_name.." sid:"..service_id);
end

function PlayerLogicService:DisConnection( service_id, service_name )
	print("PlayerLogicService:DisConnection"..service_name.." sid:"..service_id);
end

--	释放函数
function PlayerLogicService:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end

return PlayerLogicService

