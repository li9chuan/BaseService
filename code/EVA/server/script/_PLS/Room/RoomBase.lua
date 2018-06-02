local RoomBase = class("RoomBase")

-- 构造函数;
function RoomBase:ctor()

    self.RoomID                 = RoomMgr:GenerateRoomID();
    self.PrvRoomID              = 0;
    self.RoomType               = "";
    self._RoomMin               = 9999;
    
    self.RoomPlayerData         = Map:new();
    self.SeatPlayers            = {};
    self.ViewPlayers            = {};

    self._TimerHandle           = 0;
    self._TimerTick             = 1000;
    print("RoomBase:ctor");
    

end

function RoomBase:PrintInfo()
    nlinfo(self.RoomID);
    nlinfo(self.PrvRoomID);
    PrintTable(self.SeatPlayers);
    PrintTable(self.RoomPlayerData:GetTable());
end

function RoomBase:Init( room_type, update_tick )
    self:BaseInit(room_type, update_tick);
end

function RoomBase:BaseInit( room_type, update_tick )
    
    self.RoomType               = room_type;
    
    local CFG = StaticTableMgr:GetRoomConfigXml(room_type);
    
    PrintTable(CFG);
    self._RoomMin               = CFG.room_min;
    
    if update_tick~=nil then
        self._TimerTick = update_tick;
    end
    
    local ROOM_CFG = StaticTableMgr:GetRoomConfigXml(room_type);
    
    if ROOM_CFG~=nil then
        for i=1,ROOM_CFG.room_max do
            table.insert(self.SeatPlayers, 0);
        end
        
        self._TimerHandle = TimerMgr:AddTimer(self._TimerTick, self, self.TickUpdate);
    end
end

function RoomBase:TickUpdate()
    print("RoomBase:TickUpdate");
    self:BaseTickUpdate();
end

function RoomBase:BaseTickUpdate()
    --print("RoomBase:BaseTickUpdate");
    self._TimerHandle = TimerMgr:AddTimer(self._TimerTick, self, self.TickUpdate);
end


-- 玩家加入房间
function RoomBase:BaseJoinRoom( player )

    player.RoomID   = self.RoomID;
    self:__AddRoomPlayer(player.UID);
    
end

-- 玩家离开房间，子类可重写
function RoomBase:LeaveRoom( uid )
    self:BaseLeaveRoom(uid);
    self:ReleaseRoomPlayer(uid);
end

function RoomBase:BaseLeaveRoom( uid )
    local msg_int = { value = uid };
    self:BroadcastMsg( "LR", "PB.MsgInt", msg_int );
end

function RoomBase:ReleaseRoomPlayer( uid )
    
    -- 通知其它服务器离开房间
    self:__NotifyOtherServiceLevel(uid);
    
    self:__RemoveRoomPlayer(uid);

    local player = PlayerMgr:GetPlayer(uid);
    
    if player~=nil then
        player.RoomID = 0;
    end
    
    self.RoomPlayerData:Remove(uid);

end

function RoomBase:IsRoomPlayer( uid )
    for _,v in ipairs(self.SeatPlayers) do
        if v==uid then
            return true;
        end
    end
    return false;
end

function RoomBase:GetRoomPlayer( uid )
    for _,v in ipairs(self.SeatPlayers) do
        if v==uid then
            return PlayerMgr:GetPlayer(uid);
        end
    end
    return nil;
end

function RoomBase:GetPlayerSeatIdx( uid )
    for k,v in ipairs(self.SeatPlayers) do
        if v==uid then
            return k;
        end
    end
    return 0;
end

function RoomBase:GetRoomPlayerNum()
    local count = 0;
    for _,v in ipairs(self.SeatPlayers) do
        if v~=0 then
            count = count + 1;
        end
    end
    return count;
end

function RoomBase:__NotifyOtherServiceLevel( uid )

    local msgout = CMessage("LURT");
    msgout:wint(uid);
    msgout:wstring(self.RoomType);
    msgout:wint(self.PrvRoomID);

    BaseService:Broadcast( "SCH", msgout );
end

function RoomBase:__AddRoomPlayer( uid )

    local seat_idx = self:GetPlayerSeatIdx(uid);
    
    if seat_idx==0 then
        
        for k,v in ipairs(self.SeatPlayers) do
            if v==0 then
                self.SeatPlayers[k] = uid;
                seat_idx = k;
                break;
            end
        end
    end
    
    return seat_idx;
end

function RoomBase:__RemoveRoomPlayer( uid )
    
    for k,v in ipairs(self.SeatPlayers) do
        if v==uid then
            self.SeatPlayers[k] = 0;
        end
    end

end

function RoomBase:IsFull()
    
    local ROOM_CFG = StaticTableMgr:GetRoomConfigXml(self.RoomType);
    
    if ROOM_CFG~=nil then

        if self:GetRoomPlayerNum()<ROOM_CFG.room_max then
            return false;
        end
    end
    
    return true;
end


function RoomBase:Release()
    self:BaseRelease();
end

function RoomBase:BaseRelease()
    self.Fsm = nil;
    TimerMgr:RemoveTimer(self._TimerHandle);
end


--  广播消息给房间内桌上所有玩家  如有except_id，那么广播给除except_id的其它玩家。
function RoomBase:BroadcastMsg( msg_name, proto_name, proto_stru, except_id )
    
    for _,v in ipairs(self.SeatPlayers) do
        if v~=0 then
            if except_id~=v then
                local player = PlayerMgr:GetPlayer(v);
                
                if player~=nil then
                    BaseService:SendToClient( player, msg_name, proto_name, proto_stru )
                end
            end
        end
    end
    
end     
  
--  广播消息给房间内观战所有玩家  如有except_id，那么广播给除except_id的其它玩家。
function RoomBase:BroadcastViewer( msg_name, msg_stru, except_id )
    
    
    
end   

function RoomBase:__GetViewPlayerNum()
    return #self.ViewPlayers;
end

function RoomBase:__IsViewPlayer( playerid )
    if self.ViewPlayers[playerid]~=nil then
        return true;
    end
    return false;
end

return RoomBase;
