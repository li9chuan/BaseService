RoomMgr = {}

function RoomMgr:Init()
	
    self.GameRooms          = {};
    self.RoomTypes          = {};
    
    self.RoomIDGen          = IDGenerate.NewInstance(1020);
end

function RoomMgr:CreateRoom( room_type )

    local ROOM_CFG = StaticTableMgr:GetRoomConfigXml(room_type);
    
    if ROOM_CFG~=nil then
    
    
        PrintTable(ROOM_CFG);
        
        room_ddz = RoomDdz:new();
        room_ddz:Init();
        
    end

    return nil;
end




--释放函数
function RoomMgr:OnRelease()
    
end


return RoomMgr;
