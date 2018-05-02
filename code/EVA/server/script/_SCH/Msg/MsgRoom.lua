MsgRoom = {}

function MsgRoom:Init()
	self._EventRegister = EventRegister.new();
    self._EventRegister:RegisterEvent( "CPRM",          self, self.cbCreatePrivateRoom );
end


function MsgRoom:cbCreatePrivateRoom( sid, msgin )
    local uid       = msgin:rint64();
    local msg_cpr   = msgin:rpb("PB.MsgCreatePrivateRoom");
    RoomMgr:CreatePrivateRoom(uid, msg_cpr.room_type)
end




--释放函数
function MsgRoom:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgRoom;
