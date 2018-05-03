MsgRoomDdz = {}

function MsgRoomDdz:Init()
	self._EventRegister = EventRegister.new();
    self._EventRegister:RegisterEvent( "DDZ_SR",            self, self.CBStartReady );
    self._EventRegister:RegisterEvent( "DDZ_CR",            self, self.CBCancleReady );
end

function MsgRoomDdz:CBStartReady( fes_sid, msgin )
    local uid       = msgin:rint64();
    local player    = PlayerMgr:GetPlayer(uid);
    
    if player~=nil then
        local room = RoomMgr:GetRoom(player.RoomID);
        
        if room~=nil then
            room:UserStartReady(uid);
        end
    end
end

function MsgRoomDdz:CBCancleReady( fes_sid, msgin )
    local uid       = msgin:rint64();
    local player    = PlayerMgr:GetPlayer(uid);
    
    if player~=nil then
        local room = RoomMgr:GetRoom(player.RoomID);
        
        if room~=nil then
            room:UserCancelReady(uid);
        end
    end
end

--释放函数
function MsgRoomDdz:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgRoomDdz;
