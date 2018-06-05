RobotMgr = {}

function RobotMgr:Init()
	
	self._EventRegister = EventRegister.new();
    -- 线程间消息
	self._EventRegister:RegisterEvent( "TestSubProc",  self, self.LuaTestCB );


    -- 网络消息
    self._EventRegister:RegisterEvent( "AuthOk",  self, self.cbAuthOk );

    self.TotalRobot     = nil;
    self.RobotList      = {};
end

function RobotMgr:StartLogic( start_num, total_num )
	
    self.TotalRobot = total_num;

    for i=1,total_num do
        local robot                         = Robot:new();
        robot.DataCommon.User               = "test_" .. start_num+i-1;
        robot.DataCommon.Game               = "RM_DDZ";

        robot:Init(i);
        self.RobotList[robot:GetHandle()]   = robot;
    end
end

function RobotMgr:Update()
    for _,v in pairs(self.RobotList) do
        v:Update();
    end
end

function RobotMgr:Release()
    
end

function RobotMgr:LuaTestCB( from, msgin )

	local msgint  = msgin:rint();
    local msgstr  = msgin:rstring();
	
	print(from);
	print(msgint);
	print(msgstr);
    
    
    local msgout = CMessage("TestMainProc");
    msgout:wstring("TestMainProc");
	Misc.PostMain( G_ThreadHandle, msgout );
end

function RobotMgr:cbAuthOk( from, msgin )

    local robot = self.RobotList[from];

    if robot~=nil then
        nlinfo("RobotMgr:cbAuthOk");
    end
end




return RobotMgr;
