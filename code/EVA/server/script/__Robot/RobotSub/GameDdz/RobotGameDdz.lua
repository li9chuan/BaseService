local RobotGameDdz = class("RobotGameDdz", RobotGameBase)

function RobotGameDdz:ctor()

end

function RobotGameDdz:Update()
    self.Fsm:TickUpdate();
end



return RobotGameDdz;
