PublicRoomInfoMgr = {}

-- 公共信息，robot间交换信息使用。
function PublicRoomInfoMgr:Init()
	
    self.Rooms      =   {};  -- room_tyype, {room_prvid,  room_ins}
end


function PublicRoomInfoMgr:PushOpenRoom( room_info )
    
    if self.Rooms[room_info.RoomType] == nil then
        self.Rooms[room_info.RoomType] = {};
    end
    
    self.Rooms[room_info.RoomType][room_info.RoomID] = room_info;
end

function PublicRoomInfoMgr:RemoveOpenRoom( room_type, room_prvid )
	self.Rooms[room_prvid] = nil;
end

function PublicRoomInfoMgr:GetOpenRoom( room_type )
    local rooms = self.Rooms[room_type];
    
    if rooms~=nil then
        for _,v in pairs(rooms) do
            return v;
        end    
    end

    return nil;
end

return PublicRoomInfoMgr;
