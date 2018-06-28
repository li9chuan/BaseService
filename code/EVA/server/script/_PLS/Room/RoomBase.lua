local RoomBase = class("RoomBase")

-- 构造函数;
function RoomBase:ctor()
    self.RoomID                 = RoomMgr:GenerateRoomID();
    self.PrvRoomID              = 0;
    self.RoomType               = "";

    self.IsGameStart            = false;
    self.CreatorID              = 0;                -- 房间归属
    self.CreateInfo             = nil;
    self.RoomPlayerData         = Map:new();
    self.SeatPlayers            = {};
    self.ViewPlayers            = {};
    self.ShowDownEvent          = {};
    
    self._TimerHandle           = 0;
    self._TimerTick             = 1000;
    self._CreateTime            = os.time();
    
    self.CFG_RM_MIN             = 9999;
    self.CFG_LIMIT_TIME         = 28800;
end


function RoomBase:PrintInfo()
    nlinfo("============== RoomID:"..self.RoomID .. "  PrvID:"..self.PrvRoomID.."  Creator:"..self.CreatorID);
    nlinfo("==>SeatPlayers:")
    PrintTable(self.SeatPlayers);
    nlinfo("==>RoomPlayers:")
    PrintTable(self.RoomPlayerData.map);
end

function RoomBase:Init( room_type, update_tick )
    
end

function RoomBase:BaseInit( room_type, update_tick )

    if update_tick~=nil then
        self._TimerTick = update_tick;
    end
    
    local ROOM_CFG = StaticTableMgr:GetRoomConfig(room_type);
    
    if ROOM_CFG~=nil then
        
        self.RoomType               = room_type;
        self.CFG_RM_MIN             = ROOM_CFG.room_min;
        self.CFG_LIMIT_TIME         = ROOM_CFG.room_time;
        
        for i=1,ROOM_CFG.room_max do
            table.insert(self.SeatPlayers, 0);
        end
        
        self._TimerHandle = TimerMgr:AddTimer(self._TimerTick, self, self.BaseTickUpdate);
        
        self:Init();
    end
end

function RoomBase:TickUpdate()
    nlinfo("RoomBase:TickUpdate  "..self._TimerHandle);
end


function RoomBase:PlayerOffline(uid)
    if not self.IsGameStart then
        self:LeaveRoom(uid);
    end
end

function RoomBase:BaseTickUpdate()
    
    if os.time()-self._CreateTime > self.CFG_LIMIT_TIME then
        RoomMgr:ReleaseRoom( self.RoomID );
        return;
    end
    
    if self._TimerHandle > 0 then
        -- BaseInit 之后才继续timer
        self._TimerHandle = TimerMgr:AddTimer(self._TimerTick, self, self.BaseTickUpdate);
    end
    
    self:TickUpdate();
end


-- 玩家加入房间
function RoomBase:BaseJoinRoom( player )
    player.RoomID   = self.RoomID;
    self:__AddRoomPlayer(player.UID);
end

-- 玩家离开房间，子类可重写
function RoomBase:LeaveRoom( uid )
    self:BrLeaveRoom(uid);
    
    if not self.IsGameStart and uid~=self.CreatorID then
        self:ReleaseRoomPlayer(uid);
    end
end

-- 广播玩家离开房间
function RoomBase:BrLeaveRoom( uid )
    local msg_int = { value = uid };
    self:BroadcastMsg( "LR", "PB.MsgInt", msg_int );
end

function RoomBase:ReleaseRoomPlayer( uid )
    
    -- 通知其它服务器离开房间
    self:__NotifyOtherServiceLeave(uid);
    
    self:__RemoveSeatPlayer(uid);

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

function RoomBase:GetNextUID( curr_id )

    local curr_seat = self:GetPlayerSeatIdx(curr_id);
    
    if curr_seat>0 then

        -- 当前位置向后找
        for i=curr_seat+1, #self.SeatPlayers do
            local next_uid = self.SeatPlayers[i];
            if next_uid>0 then
                return next_uid;
            end
        end

        -- 后面没有了，从头开始找
        for i=1, #self.SeatPlayers do
            local next_uid = self.SeatPlayers[i];
            if next_uid>0 then
                return next_uid;
            end
        end
    end
    
    return 0;
end

function RoomBase:RelieveRequestRoom( uid, is_relieve )
end

function RoomBase:RelieveAutoRoom()
end

function RoomBase:RelieveForceRoom()
end

-- 新房间特殊玩法都用此方法判断
function RoomBase:CheckRoomSpecialKind( special_kind )
    if self.CreateInfo~=nil and self.CreateInfo.special_kind~=nil then
        return Misc.GetBit(self.CreateInfo.special_kind, special_kind);
    end
    return false;
end

function RoomBase:IsFull()
    local ROOM_CFG = StaticTableMgr:GetRoomConfig(self.RoomType);
    
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
    
    if self.IsGameStart then
        self:RelieveAutoRoom();
    end

    for _,uid in ipairs(self.SeatPlayers) do
        if uid~=0 then
            self:ReleaseRoomPlayer(uid);
        end
    end
    
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

-- 添加结算事件
function RoomBase:__AddShowDownEvent( TShowDownEvent, count )
    local event_cnt = self.ShowDownEvent[TShowDownEvent];
    
    if event_cnt==nil then
        self.ShowDownEvent[TShowDownEvent] = count;
    else
        event_cnt = event_cnt + count;
    end
end

function RoomBase:__GetSpecialCfg( field )
    local ROOM_CFG  = StaticTableMgr:GetSpecialCfg(self.RoomType);
    
    if ROOM_CFG~=nil then
        if field~=nil then
            return ROOM_CFG[field];
        end
        return ROOM_CFG;
    end
    
    return nil;
end

function RoomBase:__NotifyOtherServiceLeave( uid )

    local msgout = CMessage("PLS=>LURT");
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

function RoomBase:__RemoveSeatPlayer( uid )
    for k,v in ipairs(self.SeatPlayers) do
        if v==uid then
            self.SeatPlayers[k] = 0;
        end
    end
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
