local RobotGameDdz = class("RobotGameDdz", RobotGameBase)

function RobotGameDdz:ctor()
    self.super:ctor();
    self.Robot = nil;
    self.IsInRoom   = false;
end

function RobotGameDdz:DoCreatePrvRoom()
    local create_prvroom = { room_type="RM_DDZ" };
    self.Robot:Send( "CPRM", "PB.MsgCreatePrivateRoom", create_prvroom );
end

function RobotGameDdz:cbDdzGameInfo( msgin )
    local ddz_gi = msgin:rpb("PB.MsgDDZRoom");

    nlinfo("RobotGameDdz:cbDdzGameInfo");
    PrintTable(ddz_gi);
    
    if not self.IsInRoom then
        
    end
    if self.Robot.Data.PrvID == 0 then
     
        
    end
end


return RobotGameDdz;
