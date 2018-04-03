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

function PlayerLogicService:UpdateServiceInfo( service_id )
    
    MsgServiceInfo = {};
    MsgServiceInfo.maxPlayer    = PlayerMgr:MaxPlayer();
    MsgServiceInfo.currPlayer   = PlayerMgr:Count();
    MsgServiceInfo.serviceId    = ServerNet.GetServiceID();
    MsgServiceInfo.serviceName  = ServerNet.GetServiceName();
    
    BaseService:Send( service_id, "SvrInfo", "PB_MSG.MsgServiceInfo", MsgServiceInfo )

end


function PlayerLogicService:Connection( service_id, service_name )
	print("PlayerLogicService:Connection:"..service_name.." sid:"..service_id);
    
    self:UpdateServiceInfo(service_id);
end

function PlayerLogicService:DisConnection( service_id, service_name )
	print("PlayerLogicService:DisConnection"..service_name.." sid:"..service_id);
end

--	释放函数
function PlayerLogicService:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end

return PlayerLogicService

