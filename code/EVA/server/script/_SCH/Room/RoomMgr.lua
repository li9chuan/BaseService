RoomMgr = {}

function RoomMgr:Init()
    
    self._RoomIDPool            = {};
    
    self._EventRegister         = EventRegister.new();
    

end








function RoomMgr:Release()
    self._EventRegister:UnRegisterAllEvent();
end


function RoomMgr:InitPrivateRoomID()




end

return RoomMgr


