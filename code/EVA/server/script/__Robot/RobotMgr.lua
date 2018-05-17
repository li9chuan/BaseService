RobotMgr = {}

function RobotMgr:Init()
	
	self._EventRegister = EventRegister.new();
	self._EventRegister:RegisterEvent( "TestMainProc",  self, self.LuaTestCB );
end

function RobotMgr:LuaTestCB( from, msgin )

    local msgstr  = msgin:rstring();
	
	print(from);
	print(msgstr);
end

--释放函数
function RobotMgr:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return RobotMgr;
