local DBProc = class("DBProc")

-- 构造函数;
function DBProc:ctor( Data )
	self:Init();
end


function DBProc:LuaTest()

	print("DBProc LuaTest ");
	
	msg_session = {  
		check = 13,  
		session = 4611686020574871551,  
		index = 21
	}
	
	PostSub( "thd_player", "EVT_DB_SUB", "MsgSession", msg_session, 0xffff );

end

function DBProc:LuaTestCB( from, proto_buf )

	local msg_session = protobuf.decode("MsgSession" , proto_buf)
	
	print("Main Thread DBProc Callback " .. from);
	print(msg_session.check);
	print(msg_session.session);
	print(msg_session.index);
	
	code = protobuf.encode("MsgSession", msg_session)
	len  = string.len(code);

end

function DBProc:Init()
	
	self._EventRegister = EventRegister.new();
	self._EventRegister:RegisterEvent( "EVT_DB_MAIN",  self, self.LuaTestCB );
end

--释放函数
function DBProc:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return DBProc;
