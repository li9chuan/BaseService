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
            nlwarning("player.ConPLS    PB_MSG.MsgSvrLogin");
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
        
        nlwarning("pls_sid:"..pls_sid);
    
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

--释放函数
function MsgLogin:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgLogin;
