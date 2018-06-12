RoomMgr = {}

function RoomMgr:Init()
	
    self.GameRooms          = Map:new();               --  {roomid, room_ins}
    --self.RoomTypes          = {};               --  {roomtype, {roomids, room_ins} }
    self.PrvRoomTypes       = MapMap:new();               --  {roomtype, {prv_roomids, room_ins} }
    
    
    self.RoomIDGen          = IDGenerate.NewInstance(1020);
end

function RoomMgr:PrintInfo()
    nlinfo("==============  Rooms =============");
    self.GameRooms:ForEach( function(_,v) v:PrintInfo(); end );
end

function RoomMgr:CreatePrivateRoom( uid, prv_room_id, room_type )

    local player    = PlayerMgr:GetPlayer(uid);
    local ROOM_CFG  = StaticTableMgr:GetRoomConfigXml(room_type);
    
    if ROOM_CFG~=nil and player~=nil then
        
        if player.RoomID >0 then    -- 已经创建过房间了
            nlwarning("already create room.  PlayerID:".. uid.."  RoomInfo:");
            
            local room = self.GameRooms:Find(player.RoomID);
            if room~=nil then
                PrintTable(room);
            end
        else
            local room_base = RoomFactory:CreateRoom(room_type);
            
            if room_base~=nil then
                room_base.PrvRoomID = prv_room_id;

                if room_base:JoinRoom(player) then
                    room_base.OwenrID   = uid;
                    self.GameRooms:Insert(room_base.RoomID, room_base);
                    self.PrvRoomTypes:Insert(room_type, prv_room_id, room_base);
                else
                    nlwarning("player join room fail.  uid:"..uid)
                end
            end
        end
    end

    return nil;
end

function RoomMgr:EnterPrivateRoom( uid, prv_room_id, room_type )

    local player    = PlayerMgr:GetPlayer(uid);
    local ROOM_CFG  = StaticTableMgr:GetRoomConfigXml(room_type);
    
    if ROOM_CFG~=nil and player~=nil then
        
        if player.RoomID >0 then
            -- 已经在房间，自动返回
            local room = self.GameRooms:Find(player.RoomID);
            if room~=nil then
                room:JoinRoom(player);
            end
        else
            local room = self.PrvRoomTypes:Find(room_type, prv_room_id);

            if room~=nil then
                room:JoinRoom(player);
            end
        end
    end

    return nil;
end

function RoomMgr:GetRoom( room_id )
    return self.GameRooms:Find(room_id);
end

function RoomMgr:ReleaseRoom( room_id )
    local room = self.GameRooms:Find(room_id);
    
    if room~=nil then
        
        if room.PrvRoomID > 0 then
            self.PrvRoomTypes:Remove( room.RoomType, room.PrvRoomID );
        end
        
        self.GameRooms:Remove(room_id);
    end
end

function RoomMgr:GetRoomFromPID( uid )
    local player    = PlayerMgr:GetPlayer(uid);
    
    if player~=nil then
        return RoomMgr:GetRoom(player.RoomID);
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
