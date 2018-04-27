MsgRoom = {}

-- 构造函数;
function MsgRoom:Init()
	self._EventRegister = EventRegister.new();

    self._EventRegister:RegisterEvent( "ER",                self, self.cbEnterRoom );
    self._EventRegister:RegisterEvent( "CPRM=>PLS",         self, self.cbCreatePrivateRoom );

   
end

function MsgRoom:cbEnterRoom( fes_sid, msg_enter_room )


    print("MsgRoom:cbEnterRoom");
	
end

function MsgRoom:cbCreatePrivateRoom( sch_sid, msgin )


    local uid           = msgin:rint();
    local prv_room_id   = msgin:rint();
    local pls_sid       = msgin:rint();
    local room_type     = msgin:rstring();

    print("MsgRoom:cbCreatePrivateRoom");
    
    print(uid);
    print(prv_room_id);
    print(pls_sid);
    print(room_type);
	
    RoomMgr:CreatePrivateRoom(uid, prv_room_id, room_type);
end


--释放函数
function MsgRoom:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgRoom;
