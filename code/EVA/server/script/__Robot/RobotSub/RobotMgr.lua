RobotMgr = {}

function RobotMgr:Init()
	
	self._EventRegister = EventRegister.new();
	self._EventRegister:RegisterEvent( "TestSubProc",  self, self.LuaTestCB );

    self.robot          = Robot:new();
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

function RobotMgr:Update()
    self.robot:Update()
end

function RobotMgr:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return RobotMgr;
