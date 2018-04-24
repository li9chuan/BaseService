MsgRoom = {}

-- 构造函数;
function MsgRoom:Init()
	self._EventRegister = EventRegister.new();

    self._EventRegister:RegisterEvent( "ER",                self, self.CBEnterRoom );
   
end

function MsgRoom:CBEnterRoom( sch_sid, msg_enter_room )


    print("MsgRoom:cbEnterRoom");
	
end




--释放函数
function MsgRoom:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgRoom;
