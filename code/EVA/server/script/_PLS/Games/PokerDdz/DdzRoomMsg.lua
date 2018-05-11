MsgRoomDdz = {}

function MsgRoomDdz:Init()
	self._EventRegister = EventRegister.new();
    self._EventRegister:RegisterEvent( "DDZ_SR",            self, self.cbStartReady );
    self._EventRegister:RegisterEvent( "DDZ_CR",            self, self.cbCancleReady );
    self._EventRegister:RegisterEvent( "DDZ_JB",            self, self.cbDouDiZhuAddTimes );
    self._EventRegister:RegisterEvent( "DDZ_QDZ",           self, self.cbDouDiZhuQiangDiZhu );
end

function MsgRoomDdz:cbStartReady( fes_sid, msgin )
    local uid       = msgin:rint64();
    local room      = RoomMgr:GetRoomFromPID(uid);
    
    if room~=nil then
        room:UserStartReady(uid);
    end
end

function MsgRoomDdz:cbCancleReady( fes_sid, msgin )
    local uid       = msgin:rint64();
    local room      = RoomMgr:GetRoomFromPID(uid);
    
    if room~=nil then
        room:UserCancelReady(uid);
    end
end

function MsgRoomDdz:cbDouDiZhuAddTimes( fes_sid, msgin )
    local uid       = msgin:rint64();
    local room      = RoomMgr:GetRoomFromPID(uid);
    
    if room~=nil then
        local msg_jbr = msgin:rpb("PB.MsgJiaBeiResult");
        room:RefreshSelectJiaBei(uid, msg_jbr);
    end
end

function MsgRoomDdz:cbDouDiZhuQiangDiZhu( fes_sid, msgin )
    local uid       = msgin:rint64();
    local room      = RoomMgr:GetRoomFromPID(uid);
    
    if room~=nil then
        local msg_qdz = msg_login:rpb("PB.MsgQiangDiZhuResult");
        room:RefrshRoleQiangDiZhu(uid, msg_qdz);
    end
end

--释放函数
function MsgRoomDdz:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgRoomDdz;
