MsgRoom = {}

function MsgRoom:Init()
	self._EventRegister = EventRegister.new();
    self._EventRegister:RegisterEvent( "CPRM",          self, self.CreatePrivateRoom );
end


function MsgRoom:CreatePrivateRoom( sid, msgin )

    local uid       = msgin:rint64();
    local msg_cpr   = msgin:rpb("PB_MSG.MsgCreatePrivateRoom");
    
    print(uid);
    print(msg_cpr.room_type);

	
end




--释放函数
function MsgRoom:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgRoom;
