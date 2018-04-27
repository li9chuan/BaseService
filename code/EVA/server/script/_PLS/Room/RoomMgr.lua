RoomMgr = {}

function RoomMgr:Init()
	
    self.GameRooms          = {};               --  {roomid, room_ins}
    --self.RoomTypes          = {};               --  {roomtype, {roomids, room_ins} }
    --self.PrvRoomTypes       = {};               --  {roomtype, {prv_roomids} }
    
    self.RoomIDGen          = IDGenerate.NewInstance(1020);
end

function RoomMgr:CreatePrivateRoom( uid, prv_room_id, room_type )

    local player    = PlayerMgr:GetPlayer(uid);
    local ROOM_CFG  = StaticTableMgr:GetRoomConfigXml(room_type);
    
    if ROOM_CFG~=nil and player~=nil then
        
        
        local room_base = RoomFactory:CreateRoom(room_type);

        room_base.PrvRoomID = prv_room_id;
        room_base.RoomType  = room_type;
        
        self.GameRooms[room_base.RoomID] = room_base;
        
        player.RoomID   = room_base.RoomID;
        
        PrintTable(self.GameRooms[room_base.RoomID]);
    end

    return nil;
end

function RoomMgr:GenerateRoomID()
    return self.RoomIDGen:Generate();
end


--释放函数
function RoomMgr:Release()
    
end


return RoomMgr;
