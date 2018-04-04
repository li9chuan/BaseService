DBMgr = {}

function DBMgr:Init()
	
	self._EventRegister = EventRegister.new();
	self._EventRegister:RegisterEvent( "EVT_DB_MAIN",  self, self.LuaTestCB );
end

function DBMgr:LuaTest()

	print("DBMgr LuaTest ");
	

end




--释放函数
function DBMgr:OnRelease()
    
end


return DBMgr;
