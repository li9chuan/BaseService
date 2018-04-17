PLSInfoMgr = {}

-- 初始化函数
function PLSInfoMgr:Init()
    self.PLSMap         = {};
    self._EventRegister = EventRegister.new();
    
    self._EventRegister:RegisterEvent( "SvrInfo",  self, self.SvrUpdateInfoCB );
    
    print("PLSInfoMgr:Init");
end

function PLSInfoMgr:SvrUpdateInfoCB( msg_from, msg_svrinfo )
    
    local proto_buf = msg_svrinfo:rstring();
    local pb_sinfo  = protobuf.decode("PB_MSG.MsgServiceInfo" , proto_buf)
    local pls_info  = PLSInfo:new();
    pls_info:LoadData(pb_sinfo);
    
    if pls_info.ServiceName=="PLS" then
        self.PLSMap[pls_info.ServiceID] = pls_info;
--[[        
        print("PLSInfoMgr:SvrUpdateInfoCB  ----  update pls info.");
        print( pls_info.ServiceID );
        print( pls_info.ServiceName );
        print( pls_info.MaxPlayer );
        print( pls_info.CurrPlayer );
        
        for k,v in pairs(pls_info.RoomList) do
            print( "type:"..v.Type.."  max:"..v.Max.."   curr:"..v.Curr );
        end
--]]
    end

end

function PLSInfoMgr:AllocPLS( game_type )

    for _,v in ipairs(self.PLSMap) do
        
        if v.RoomList[game_type] ~= nil then
            
            local game_info = v.RoomList[game_type];
            
            if game_info:IsFull()==false then
                game_info.Curr = game_info.Curr + 1;
                return v.ServiceID;
            end;
            
        end;
    end;
    
    return nil;
end


--释放函数
function PLSInfoMgr:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end

return PLSInfoMgr


