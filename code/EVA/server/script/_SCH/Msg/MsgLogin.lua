--========================================================= 
-- SCH   负责分配到一个PLS
--=========================================================

local MsgLogin = class("MsgLogin")

-- 构造函数;
function MsgLogin:ctor( Data )

	self._EventRegister = EventRegister.new();
	
    --  服务器间消息
    self._EventRegister:RegisterEvent( "AuthOk",  self, self.DispatchPLS );
	
end

-- 分配PLS
function MsgLogin:DispatchPLS( fes_id, proto_buf )

	local MsgSvrLogin = protobuf.decode("PB_MSG.MsgSvrLogin", proto_buf)
    MsgSvrLogin.ConFES = fes_id;
    local uid = MsgSvrLogin.UID;
    local player = PlayerInfoMgr:GetPlayerInfo(uid);
    
    if player~=nil then

        player.ConFES = fes_id;
        
        if player.ConPLS~=nil then
            nlwarning("player.ConPLS    PB_MSG.MsgSvrLogin");
            BaseService:Send( player.ConPLS, "SyncData", "PB_MSG.MsgSvrLogin", MsgSvrLogin )
        else
            local pls_sid = PLSInfoMgr:AllocPLS(player.GameType);
            
            if pls_sid~=nil then
                player.ConPLS = pls_sid;
                BaseService:Send( player.ConPLS, "SyncData", "PB_MSG.MsgSvrLogin", MsgSvrLogin )
            else
                nlwarning("======");
            end
        end
    else
        local pls_sid = PLSInfoMgr:AllocPLS(MsgSvrLogin.GameType);
        
        nlwarning("pls_sid:"..pls_sid);
    
        if pls_sid~=nil then
            player = PlayerInfoMgr:CreatePlayerInfo(uid);
            
            player.ConFES   = fes_id;
            player.ConPLS   = pls_sid;
            player.GameType = MsgSvrLogin.GameType;
            
            BaseService:Send( player.ConPLS, "SyncData", "PB_MSG.MsgSvrLogin", MsgSvrLogin )
            
            nlwarning("fes_id:"..player.GameType);
        end
    end
end

--释放函数
function MsgLogin:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgLogin;
