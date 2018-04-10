RoomMgr = {}

function RoomMgr:Init()
	
    self.GameRooms          = {};
    self.RoomTypes          = {};
    
end

function RoomMgr:CreateRoom( room_type )

    local ROOM_CFG = StaticTableMgr:GetRoomConfigXml(room_type);
    
    if ROOM_CFG~=nil then
    
        
    end

    return nil;
end




--释放函数
function RoomMgr:OnRelease()
    
end


return RoomMgr;
