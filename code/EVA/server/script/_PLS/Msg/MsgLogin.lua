local MsgLogin = class("MsgLogin")

-- 构造函数;
function MsgLogin:ctor( Data )
	self._EventRegister = EventRegister.new();

    self._EventRegister:RegisterEvent( "SyncData",          self, self.CBSyncData );
    
    
    self._EventRegister:RegisterEvent( "RemovePlayer",      self, self.CBRemovePlayer );
    
end

function MsgLogin:CBSyncData( sch_sid, proto_buf )

	local MsgSvrLogin   = protobuf.decode("PB_MSG.MsgSvrLogin" , proto_buf)
    local player_helper = PlayerMgr:LoadDBPlayer(MsgSvrLogin.UID);
    
    if player_helper~=nil then
        
        player_helper.ConFES = MsgSvrLogin.ConFES;
        
        
        PrintTable(player_helper);
        BaseService:Send( player_helper.ConFES, "LoginPLS", "PB_MSG.MsgSvrLogin" , MsgSvrLogin);
        
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
