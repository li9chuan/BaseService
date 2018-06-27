PLSInfoMgr = {}

-- 初始化函数
function PLSInfoMgr:Init()
    self.PLSMap         = Map:new();
    self._EventRegister = EventRegister.new();
    
    self._EventRegister:RegisterEvent( "SvrInfo",  self, self.SvrUpdateInfoCB );
    
    nlinfo("PLSInfoMgr:Init");
end

function PLSInfoMgr:SvrUpdateInfoCB( msg_from, msg_svrinfo )
    
    local proto_buf = msg_svrinfo:rstring();
    local pb_sinfo  = protobuf.decode("PB.MsgServiceInfo" , proto_buf)
    local pls_info  = PLSInfo:new();
    pls_info:LoadData(pb_sinfo);
    
    if pb_sinfo.ServiceName=="PLS" then
        self.PLSMap:Replace(pls_info.ServiceID, pls_info);
    end

end

function PLSInfoMgr:AllocPLS( room_type )

    for _,v in pairs(self.PLSMap.map) do
        
        if v.RoomList[room_type] ~= nil then
            local game_info = v.RoomList[room_type];
            
            if not game_info:IsFull() then
                game_info.Curr = game_info.Curr + 1;
                return v.ServiceID;
            end;
        end;
    end;
    
    return -1;
end

function PLSInfoMgr:IsInPLS( room_type, pls_sid )
    local pls_info = self.PLSMap:Find(pls_sid);
    
    if pls_info~=nil then
        if pls_info[room_type]~=nil then
            return true;
        end
    end
    
    return false;
end


--释放函数
function PLSInfoMgr:Release()
    self._EventRegister:UnRegisterAllEvent();
end

return PLSInfoMgr


