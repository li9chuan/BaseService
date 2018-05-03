RoomMgr = {}

function RoomMgr:Init()
	
    self.GameRooms          = {};               --  {roomid, room_ins}
    --self.RoomTypes          = {};               --  {roomtype, {roomids, room_ins} }
    self.PrvRoomTypes       = {};               --  {roomtype, {prv_roomids, room_ins} }
    
    
    self.RoomIDGen          = IDGenerate.NewInstance(1020);
end

function RoomMgr:CreatePrivateRoom( uid, prv_room_id, room_type )

    local player    = PlayerMgr:GetPlayer(uid);
    local ROOM_CFG  = StaticTableMgr:GetRoomConfigXml(room_type);
    
    if ROOM_CFG~=nil and player~=nil then
        
        if player.RoomID >0 then    -- 已经创建过房间了
            nlwarning("已经创建过房间了");
        else
            local room_base = RoomFactory:CreateRoom(room_type);
            
            if room_base~=nil then
                room_base.PrvRoomID = prv_room_id;
                self.GameRooms[room_base.RoomID] = room_base;
                self.PrvRoomTypes[room_type] = { [prv_room_id] = room_base};
                room_base:JoinRoom(player);
            end
        end

        PrintTable(self.GameRooms[room_base.RoomID]);
    end

    return nil;
end

function RoomMgr:EnterPrivateRoom( uid, prv_room_id, room_type )

    local player    = PlayerMgr:GetPlayer(uid);
    local ROOM_CFG  = StaticTableMgr:GetRoomConfigXml(room_type);
    
    if ROOM_CFG~=nil and player~=nil then
        
        if player.RoomID >0 then
            nlwarning("已经在房间了");
        else
            local rooms = self.PrvRoomTypes[room_type];
            
            if rooms~=nil then
                local room = rooms[prv_room_id];
                
                if room~=nil then
                    room:JoinRoom(player);
                end
            end
        end
    end

    return nil;
end

function RoomMgr:GetRoom( room_id )
    return self.GameRooms[room_id];
end

function RoomMgr:GenerateRoomID()
    return self.RoomIDGen:Generate();
end


--释放函数
function RoomMgr:Release()
    
end


return RoomMgr;
