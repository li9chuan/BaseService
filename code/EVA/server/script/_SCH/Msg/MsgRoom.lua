MsgRoom = {}

function MsgRoom:Init()
	self._EventRegister = EventRegister.new();
    self._EventRegister:RegisterEvent( "CPRM",          self, self.cbCreatePrivateRoom );
    self._EventRegister:RegisterEvent( "EPRM",          self, self.cbEnterPrivateRoom );   
    
    self._EventRegister:RegisterEvent( "PLS=>LURT",     self, self.cbLeaveRoomType );   
    
end


function MsgRoom:cbCreatePrivateRoom( sid, msgin )
    local uid       = msgin:rint64();
    local msg_cpr   = msgin:rpb("PB.MsgCreatePrivateRoom");
    RoomMgr:CreatePrivateRoom(uid, msg_cpr)
end

function MsgRoom:cbEnterPrivateRoom( sid, msgin )
    local uid       = msgin:rint64();
    local msg_epr   = msgin:rpb("PB.MsgEnterPrivateRoom");
    RoomMgr:EnterPrivateRoom(uid, msg_epr);
end

function MsgRoom:cbLeaveRoomType( sid, msgin )
    local uid       = msgin:rint();
    local prv_rmtp  = msgin:rstring();
    local prv_rmid  = msgin:rint();
    
    RoomMgr:ResetPrivateRoom(uid, prv_rmtp, prv_rmid);
end


--释放函数
function MsgRoom:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgRoom;
