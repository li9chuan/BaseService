MsgRoomDdz = {}

--[[
        斗地主消息处理。
--]]

function MsgRoomDdz:Init()
	self._EventRegister = EventRegister.new();
    self._EventRegister:RegisterEvent( "DDZ_SR",            self, self.cbStartReady );              -- 斗地主开始准备
    self._EventRegister:RegisterEvent( "DDZ_CR",            self, self.cbCancleReady );             -- 斗地主取消准备
    self._EventRegister:RegisterEvent( "DDZ_JB",            self, self.cbDouDiZhuAddTimes );        -- 加倍选择
    self._EventRegister:RegisterEvent( "DDZ_QDZ",           self, self.cbDouDiZhuQiangDiZhu );      -- 抢地主选择
    self._EventRegister:RegisterEvent( "DDZ_OC",            self, self.cbDouDiZhuOutCard );         -- 斗地主出牌
    self._EventRegister:RegisterEvent( "DDZ_PS",            self, self.cbDouDiZhuPassCards );       -- 斗地主过牌
    self._EventRegister:RegisterEvent( "DDZ_SMPS",          self, self.cbPreStartMingPai );         -- 游戏开始前选择是否明牌
    self._EventRegister:RegisterEvent( "DDZ_DZMP",          self, self.cbPreMingPai );              -- 出牌前地主选择是否明牌
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
        local msg_qdz = msgin:rpb("PB.MsgQiangDiZhuResult");
        room:RefrshRoleQiangDiZhu(uid, msg_qdz);
    end
end

function MsgRoomDdz:cbPreStartMingPai( fes_sid, msgin )
    local uid       = msgin:rint64();
    local room      = RoomMgr:GetRoomFromPID(uid);
    
    if room~=nil then
        
        
    end
end

function MsgRoomDdz:cbPreMingPai( fes_sid, msgin )
    local uid       = msgin:rint64();
    local room      = RoomMgr:GetRoomFromPID(uid);
    
    if room~=nil then
        
        
    end
end

function MsgRoomDdz:cbDouDiZhuOutCard( fes_sid, msgin )
    local uid       = msgin:rint64();
    local room      = RoomMgr:GetRoomFromPID(uid);
    
    if room~=nil then
        local msg_oc = msgin:rpb("PB.MsgDDZUserOutCard");
        room:UserOutCard( uid, msg_oc );
    end
end

function MsgRoomDdz:cbDouDiZhuPassCards( fes_sid, msgin )
    local uid       = msgin:rint64();
    local room      = RoomMgr:GetRoomFromPID(uid);
    
    if room~=nil then
        room:UserPassOutCard( uid );
    end
end

--释放函数
function MsgRoomDdz:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgRoomDdz;
