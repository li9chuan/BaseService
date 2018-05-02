PlayerLogicService = {}

function PlayerLogicService:Init()

    -- 注册其它服务器启动的回调
    ServerNet.SetConnectionCallback("SCH");
    ServerNet.SetDisConnectionCallback("SCH");
    
    
	self._EventRegister = EventRegister.new();
    self._EventRegister:RegisterEvent( "SCHCon",            self, self.SCHConnection );
	self._EventRegister:RegisterEvent( "SCHDis",            self, self.SCHDisConnection );
    

    self.TimerID    = 0;
    self.sch_sid    = 0;
end

function PlayerLogicService:UpdatePLSInfo( service_id )
    
    local MsgServiceInfo = {};
    MsgServiceInfo.MaxPlayer    = PlayerMgr:MaxPlayer();
    MsgServiceInfo.CurrPlayer   = PlayerMgr:Count();
    MsgServiceInfo.ServiceID    = ServerNet.GetServiceID();
    MsgServiceInfo.ServiceName  = ServerNet.GetServiceName();
    MsgServiceInfo.RoomList     = {};
    
    for k, v in pairs(PLSConfig.GameConfig) do  

        local MsgRoomType   = {};
        MsgRoomType.Type    = k;
        MsgRoomType.Max     = v.Max;
        MsgRoomType.Curr    = PlayerMgr:Count();
        
        table.insert( MsgServiceInfo.RoomList, MsgRoomType );
    end  

    BaseService:Send( service_id, "SvrInfo", "PB.MsgServiceInfo", MsgServiceInfo )

end


function PlayerLogicService:SCHConnection( service_id, service_name )
	print("PlayerLogicService:SCHConnection:"..service_name.." sid:"..service_id);
    
    self.sch_sid = service_id;
    self:UpdatePLSInfo(service_id);
    self.TimerID = TimerMgr:AddTimer(7000, self, self.UpdatePLSInfoTimer);
end

function PlayerLogicService:SCHDisConnection( service_id, service_name )
	print("PlayerLogicService:SCHDisConnection"..service_name.." sid:"..service_id);
    
    self.sch_sid = nil;
    TimerMgr:RemoveTimer(self.TimerID);
    
end

function PlayerLogicService:UpdatePLSInfoTimer()
    self:UpdatePLSInfo(self.sch_sid);
    self.TimerID = TimerMgr:AddTimer(7000, self, self.UpdatePLSInfoTimer);
end

--	释放函数
function PlayerLogicService:Release()
    self._EventRegister:UnRegisterAllEvent();
end

return PlayerLogicService

