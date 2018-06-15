local RobotGameDdz = class("RobotGameDdz", RobotGameBase)

function RobotGameDdz:ctor()
    self.super:ctor();
    self.Robot = nil;
    self.RoomInfo   = nil;
    self.SelfData   = nil;          -- 服务器上 player_list中自己的房间内数据
end

function RobotGameDdz:GetFsmState()
    return self.Robot.GameFsm:GetState();
end

function RobotGameDdz:GetRobotState( enum_idx )
    return Misc.GetBit( self.SelfData.state, enum_idx );
end

function RobotGameDdz:DoCreatePrvRoom()
    local create_prvroom = { room_type="RM_DDZ" };
    self.Robot:Send( "CPRM", "PB.MsgCreatePrivateRoom", create_prvroom );
end

function RobotGameDdz:DoJoinPrvRoom(open_room)
    local join_prvroom = { room_id=open_room.RoomID, room_type=open_room.RoomType };
    self.Robot:Send( "EPRM", "PB.MsgEnterPrivateRoom", join_prvroom );
end

function RobotGameDdz:DoReady()
    self.Robot:Send( "DDZ_SR" );
end

function RobotGameDdz:cbDdzGameInfo( msgin )
    
    local ddz_gi = msgin:rpb("PB.MsgDDZRoom");
    
    if ddz_gi==nil then
        nlwarning("ddz_gi==nil !!!!!!!!!!!!");
        return
    end

    -- 刷新房间内数据
    self.RoomInfo = ddz_gi;
    
    -- 刷新自己的数据
    for _,v in ipairs(ddz_gi.player_list) do
        if v.player_base.UID == self.Robot.Data.UID then
            self.SelfData = v;
        end
    end
    
    
    self.Robot:PrintTable(ddz_gi);
    self.Robot:Print("=======>  RobotGameDdz:cbDdzGameInfo  UID:"..self.Robot.Data.UID);
    
    if self:GetFsmState()=="TCreatePrvRoom" then
        
        --if self:GetRobotState(enum.STATE_DDZ_ROOM_OWNER) then
            -- 房间是自己创建的，返回的是创建成功
        --    self.IsCreate = true;
        --end
        
        
        self.Robot:Print("=========>Create private room.   UID:"..self.Robot.Data.UID);
        -- 把room_id加入到开放列表中，供其它机器人加入。
        
        local pb_room_info = PublicRoomInfo:new();
        pb_room_info.RoomType   = ddz_gi.private_room.room_type;
        pb_room_info.RoomID     = ddz_gi.room_id;
        pb_room_info.RoomRobots = ddz_gi.player_list;
        
        PublicRoomInfoMgr:PushOpenRoom(pb_room_info);
        
        self.Robot.GameFsm:SwitchState("TInRoomIdlem");
    elseif self:GetFsmState()=="TJoinPrvRoom" then
        self.Robot:Print("=========>Join private room.   UID:"..self.Robot.Data.UID);
        self.Robot.GameFsm:SwitchState("TInRoomIdlem");
    else
        
        self.Robot:Print("=========>Refresh private room.   UID:"..self.Robot.Data.UID);
    end
end

-- 有玩家点击了准备，如果是自己就更新状态
function RobotGameDdz:cbDdzUserStartReady( msgin )
    
    local ready_uid = msgin:rpb("PB.MsgInt");
    
    if self.Robot.Data.UID==ready_uid then
        
        self.SelfData.state = Misc.SetBit( self.SelfData.state, enum.STATE_DDZ_READY );
        self.Robot:Print("cbDdzUserStartReady");
    end
end

return RobotGameDdz;
