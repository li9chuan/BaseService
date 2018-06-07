MsgRoom = {}

function MsgRoom:Init()
	self._EventRegister = EventRegister.new();
    self._EventRegister:RegisterEvent( "CPRM",          self, self.cbCreatePrivateRoom );
    self._EventRegister:RegisterEvent( "EPRM",          self, self.cbEnterPrivateRoom );   
    
end


function MsgRoom:cbCreatePrivateRoom( sid, msgin )
    local uid       = msgin:rint64();
    local msg_cpr   = msgin:rpb("PB.MsgCreatePrivateRoom");
    RoomMgr:CreatePrivateRoom(uid, msg_cpr.room_type)
end

function MsgRoom:cbEnterPrivateRoom( sid, msgin )
    local uid       = msgin:rint64();
    local msg_epr   = msgin:rpb("PB.MsgEnterPrivateRoom");
    RoomMgr:EnterPrivateRoom(uid, msg_epr.room_type, msg_epr.room_id);
end


--释放函数
function MsgRoom:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgRoom;
