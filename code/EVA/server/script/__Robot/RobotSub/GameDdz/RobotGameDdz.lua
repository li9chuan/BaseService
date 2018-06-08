local RobotGameDdz = class("RobotGameDdz", RobotGameBase)

function RobotGameDdz:ctor()
    self.super:ctor();
    self.Robot = nil;
    self.RoomInfo   = nil;
    self.IsCreate   = false;
end

function RobotGameDdz:GetFsmState()
    return self.Robot.GameFsm:GetState();
end

function RobotGameDdz:DoCreatePrvRoom()
    local create_prvroom = { room_type="RM_DDZ" };
    self.Robot:Send( "CPRM", "PB.MsgCreatePrivateRoom", create_prvroom );
end

function RobotGameDdz:DoJoinPrvRoom(open_room)
    local join_prvroom = { room_id=open_room.RoomID, room_type=open_room.RoomType };
    self.Robot:Send( "EPRM", "PB.MsgEnterPrivateRoom", join_prvroom );
end




function RobotGameDdz:cbDdzGameInfo( msgin )
    local ddz_gi = msgin:rpb("PB.MsgDDZRoom");

    self.RoomInfo = ddz_gi;
    
    nlinfo("RobotGameDdz:cbDdzGameInfo");
    PrintTable(ddz_gi);
    
    if self:GetFsmState()=="TCreatePrvRoom" then
        -- 房间是自己创建的，返回的是创建成功
        self.IsCreate = true;
        
        -- 把room_id加入到开放列表中，供其它机器人加入。
        
        local pb_room_info = PublicRoomInfo:new();
        pb_room_info.RoomType   = ddz_gi.private_room.room_type;
        pb_room_info.RoomID     = ddz_gi.room_id;
        pb_room_info.RoomRobots = ddz_gi.player_list;
        
        PublicRoomInfoMgr:PushOpenRoom(pb_room_info);
        
        self.Robot.GameFsm:SwitchState("TIdle");
    else
        
        nlinfo("收到房间内数据--非创建");
    end

    --[[
  "player_list" : {
    {
      "seats" : 1,
      "player_base" : {
        "UID" : 1005,
        "Nickname" : " "
      }
    }
  },
  "private_room" : {
    "room_type" : "RM_DDZ"
  },
  "game_count" : 1,
  "bottom_card" : {
    
  },
  "multiple" : 1,
  "room_id" : 963620
}

--]]
end


return RobotGameDdz;
