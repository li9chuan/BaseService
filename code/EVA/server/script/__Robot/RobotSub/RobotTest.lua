RobotTest = {}

function RobotTest:Init()
	
	self._EventRegister = EventRegister.new();
	self._EventRegister:RegisterEvent( "TestSubProc",  self, self.LuaTestCB );
end

function RobotTest:LuaTestCB( from, msgin )

	local msgint  = msgin:rint();
    local msgstr  = msgin:rstring();
	
	print(from);
	print(msgint);
	print(msgstr);
    
    
    local msgout = CMessage("TestMainProc");
    msgout:wstring("TestMainProc");
	Misc.PostMain( G_ThreadHandle, msgout );
end

--释放函数
function RobotTest:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return RobotTest;
