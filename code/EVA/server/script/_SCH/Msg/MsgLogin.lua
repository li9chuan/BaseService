--========================================================= 
-- SCH   负责分配到一个PLS
--=========================================================

MsgLogin = {}

function MsgLogin:Init()
	self._EventRegister = EventRegister.new();
	
    --  服务器间消息
    self._EventRegister:RegisterEvent( "AuthOk",            self, self.DispatchPLS );

    -- PLS 删除缓存中的玩家
    self._EventRegister:RegisterEvent( "RemovePlayer",      self, self.cbRemovePlayer );
end

-- 分配PLS
function MsgLogin:DispatchPLS( fes_id, msg_authok )

    local uid       = msg_authok:rint64();
    local room_type = msg_authok:rstring();
    
    local msg_sdata_0 = CMessage("SyncData");
    msg_sdata_0:wint64(uid);
    msg_sdata_0:wint32(fes_id);

    local player = PlayerInfoMgr:GetPlayerInfo(uid);
    
    if player~=nil then

        player.ConFES = fes_id;
        
        if player.ConPLS~=nil then
            BaseService:Send( player.ConPLS, msg_sdata_0 )
        else
            local pls_sid = PLSInfoMgr:AllocPLS(player.RoomType);
            
            if pls_sid~=nil then
                player.ConPLS = pls_sid;
                BaseService:Send( player.ConPLS, msg_sdata_0 )
            else
                nlwarning(player.RoomType.." not in pls config.");
            end
        end
    else
        local pls_sid = PLSInfoMgr:AllocPLS(room_type);

        if pls_sid~=nil then
            player = PlayerInfoMgr:CreatePlayerInfo(uid);
            
            player.ConFES   = fes_id;
            player.ConPLS   = pls_sid;
            player.RoomType = room_type;
            
            BaseService:Send( player.ConPLS, msg_sdata_0 )
        else
            nlwarning(room_type.." not in pls config.");
        end
    end
end

function MsgLogin:cbRemovePlayer( fesid, msgin )
    local uid       = msgin:rint();

end


return MsgLogin;
