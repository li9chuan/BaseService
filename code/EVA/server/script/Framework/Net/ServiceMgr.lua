ServiceMgr = {}

-- 初始化函数
function ServiceMgr:Init()
	self.PLSMap         = {};
	self.FESMap         = {};
    
    self._EventRegister = EventRegister.new();
    self._EventRegister:RegisterEvent( "SvrInfo",  self, self.SvrUpdateInfoCB );
end

function ServiceMgr:SvrUpdateInfoCB( msg_from, proto_buf )

	local pb_sinfo = protobuf.decode("MsgServiceInfo" , proto_buf)
	
    if _service_info.serviceName=="FES" then
        
        service_info = ServiceInfo:new();
        service_info.LoadData(pb_sinfo);
        self.FESMap[_service_info.serviceID] = service_info;
    end
	
end

--释放函数
function ServiceMgr:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end

return ServiceMgr



