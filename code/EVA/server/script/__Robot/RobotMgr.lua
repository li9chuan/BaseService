RobotMgr = {}

function RobotMgr:Init()
	
	self._EventRegister = EventRegister.new();
	self._EventRegister:RegisterEvent( "TestMainProc",  self, self.LuaTestCB );
    
    self._ThreadMap     = {};

end

function RobotMgr:Strat()
	
    
    local RobotSubPath = Misc.GetBasePath() .. "/script/__Robot/RobotSub/RobotSubStart.lua";

    for i=1003,1053 do
        
        local sub_thread    = bin_types.LuaThread.NewInstance("robot", 1000);
        local thread_handle = sub_thread:Start( RobotSubPath, Table2Json({uid=i, b=2}) );
    
        self._ThreadMap[thread_handle] = sub_thread;
        
        print(thread_handle);
    end
    
    
   
    local msg = CMessage("TestSubProc");
    msg:wint(111222);
    msg:wstring("string");

    self._ThreadMap[0]:Post(msg);
    
end




function RobotMgr:LuaTestCB( from, msgin )

    local msgstr  = msgin:rstring();
	
	print(from);
	print(msgstr);
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
