PLSInfoMgr = {}

-- 初始化函数
function PLSInfoMgr:Init()
    self.PLSMap         = {};
    self._EventRegister = EventRegister.new();
    
    self._EventRegister:RegisterEvent( "SvrInfo",  self, self.SvrUpdateInfoCB );
end

function PLSInfoMgr:SvrUpdateInfoCB( msg_from, proto_buf )
    
    local pb_sinfo = protobuf.decode("PB_MSG.MsgServiceInfo" , proto_buf)
    local pls_info = PLSInfo:new();
    pls_info:LoadData(pb_sinfo);
    
    if pls_info.serviceName=="PLS" then
        self.PLSMap[pls_info.serviceId] = pls_info;
        
        print("PLSInfoMgr:SvrUpdateInfoCB  ----  update pls info.");
        print( pls_info.serviceId );
        print( pls_info.serviceName );
        print( pls_info.maxPlayer );
        print( pls_info.currPlayer );
        
        for k,v in pairs(pls_info.gameTypeList) do
            print( "type:"..v.type.."  max:"..v.max.."   curr:"..v.curr);
        end

    end

end

function PLSInfoMgr:AllocGame( game_type )

    for _,v in ipairs(self.PLSMap) do
        
        if v.gameTypeList[game_type] ~= nil then
            
            local game_info = v.gameTypeList[game_type];
            if ~game_info.IsFull() then
                return v.serviceId;
            end
            
        end
    end
    
    return 0;
end


--释放函数
function PLSInfoMgr:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end

return PLSInfoMgr


