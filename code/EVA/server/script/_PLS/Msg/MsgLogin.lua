local MsgLogin = class("MsgLogin")

-- 构造函数;
function MsgLogin:ctor( Data )
	self._EventRegister = EventRegister.new();

    self._EventRegister:RegisterEvent( "SyncData",          self, self.CBSyncData );
    
    
    self._EventRegister:RegisterEvent( "RemovePlayer",      self, self.CBRemovePlayer );
    
   
end

function MsgLogin:CBSyncData( sch_sid, msg_sdata_1 )

    local uid           = msg_sdata_1:rint64();
    local fes_sid       = msg_sdata_1:rint32();
    
    local player_helper = PlayerMgr:LoadDBPlayer(uid);
    
    if player_helper~=nil then
        
        player_helper.ConFES = fes_sid;
        PrintTable(player_helper);
        
        msg_sdata_1:invert();
        BaseService:Send( player_helper.ConFES, msg_sdata_1);
        
        BaseService:SendToClient( player_helper.ConFES, player_helper.UID, "SyncPlayerInfo",
                                "PB_MSG.MsgPlayerInfo", player_helper.PlayerDataHelper:ToProtoMsg() )
    end
	
end

function MsgLogin:CBRemovePlayer( sid, proto_buf )



	
end

--释放函数
function MsgLogin:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgLogin;
