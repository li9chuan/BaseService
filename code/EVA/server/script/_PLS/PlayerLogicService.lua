PlayerLogicService = {}

function PlayerLogicService:Init()

    -- 注册其它服务器启动的回调
    ServerNet.SetConnectionCallback("SCH");
    ServerNet.SetDisConnectionCallback("SCH");
    
    
	self._EventRegister = EventRegister.new();
    self._EventRegister:RegisterEvent( "SCHConnection",         self, self.SCHConnection );
	self._EventRegister:RegisterEvent( "SCHDisConnection",      self, self.SCHDisConnection );
    

    self.timerid    = 0;
    self.sch_sid    = 0;
end

function PlayerLogicService:UpdatePLSInfo( service_id )
    
    local MsgServiceInfo = {};
    MsgServiceInfo.maxPlayer    = PlayerMgr:MaxPlayer();
    MsgServiceInfo.currPlayer   = PlayerMgr:Count();
    MsgServiceInfo.serviceId    = ServerNet.GetServiceID();
    MsgServiceInfo.serviceName  = ServerNet.GetServiceName();
    MsgServiceInfo.gameTypeList = {};
    
    local MsgGameType   = {};
    MsgGameType.type    = "GM_TST";
    MsgGameType.max     = 5;
    MsgGameType.curr    = 1;
    
    table.insert( MsgServiceInfo.gameTypeList, MsgGameType );

    PrintTable(MsgServiceInfo.gameTypeList);

    BaseService:Send( service_id, "SvrInfo", "PB_MSG.MsgServiceInfo", MsgServiceInfo )

end


function PlayerLogicService:SCHConnection( service_id, service_name )
	print("PlayerLogicService:SCHConnection:"..service_name.." sid:"..service_id);
    
    self.sch_sid = service_id;
    self:UpdatePLSInfo(service_id);
    self.timerid = TimerMgr:AddTimer(7000, self, self.UpdatePLSInfoTimer);
end

function PlayerLogicService:SCHDisConnection( service_id, service_name )
	print("PlayerLogicService:SCHDisConnection"..service_name.." sid:"..service_id);
    
    self.sch_sid = nil;
    TimerMgr:RemoveTimer(self.timerid);
    
end

function PlayerLogicService:UpdatePLSInfoTimer()
    self:UpdatePLSInfo(self.sch_sid);
    self.timerid = TimerMgr:AddTimer(7000, self, self.UpdatePLSInfoTimer);
end

--	释放函数
function PlayerLogicService:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end

return PlayerLogicService

