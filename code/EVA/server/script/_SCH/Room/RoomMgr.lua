RoomMgr = {}

function RoomMgr:Init()
    
    self._RoomIDPool            = {};
    self._EventRegister         = EventRegister.new();

    self._RoomsInfo             = {};
    
    self:__InitRoomID();

end


function RoomMgr:CreatePrivateRoom( uid, room_type )
    
    local player = PlayerInfoMgr:GetPlayerInfo(uid);
    
    if player~=nil then
        if player.RoomID>0 then
        
            --local room_info
        end
    
    
    end
    
end





function RoomMgr:Release()
    self._EventRegister:UnRegisterAllEvent();
end

function RoomMgr:__NewRoomID( room_type )

    local idpool = self._RoomIDPool[room_type];
    local new_id = 0;
    
    if idpool~=nil then
        new_id = idpool:AllocID();
    end
    
    return new_id;
end

function RoomMgr:__InitRoomID()

    for k, v in pairs(StaticTableMgr._RoomConfig) do  
        if v.match == "private" then
            self._RoomIDPool[k] = RoomIDAlloter.new();
        end
    end 

end

return RoomMgr


