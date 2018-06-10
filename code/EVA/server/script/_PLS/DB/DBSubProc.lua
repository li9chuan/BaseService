local DBSubProc = class("DBSubProc")

-- 构造函数;
function DBSubProc:ctor( Data )
	self:Init();
end

function DBSubProc:LuaTestCB( from, proto_buf )

	local msg_session = protobuf.decode("MsgSession" , proto_buf)
	
	nlinfo("Sub Thread DBSubProc Callback " .. from);
	nlinfo(msg_session.check);
	nlinfo(msg_session.session);
	nlinfo(msg_session.index);
	
	PostMain( "thd_player", "EVT_DB_MAIN", "MsgSession", msg_session, from );
	
end

function DBSubProc:Init()
	
	self._EventRegister = EventRegister.new();
	self._EventRegister:RegisterEvent( "EVT_DB_SUB",  self, self.LuaTestCB );
end

--释放函数
function DBSubProc:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return DBSubProc;
