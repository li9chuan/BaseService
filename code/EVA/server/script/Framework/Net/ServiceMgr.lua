ServiceMgr = {}

-- 初始化函数
function ServiceMgr:Init()
    self.PLSMap         = {};
    self.FESMap         = {};
    
    self._EventRegister = EventRegister.new();
    self._EventRegister:RegisterEvent( "SvrInfo",  self, self.SvrUpdateInfoCB );
end

function ServiceMgr:SvrUpdateInfoCB( msg_from, proto_buf )
    
    local pb_sinfo = protobuf.decode("PB_MSG.MsgServiceInfo" , proto_buf)
    local service_info = ServiceInfo:new();
    service_info:LoadData(pb_sinfo);
    
    if service_info.serviceName=="FES" then
        self.FESMap[service_info.serviceId] = service_info;
    elseif service_info.serviceName=="PLS" then
        self.PLSMap[service_info.serviceId] = service_info;
    end
    
    
    
end

--释放函数
function ServiceMgr:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end

return ServiceMgr


