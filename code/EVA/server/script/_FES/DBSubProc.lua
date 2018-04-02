--========================================================= 
-- 消息注册管理
--=========================================================
local DBSubProc = class("DBSubProc")

-- 构造函数;
function DBSubProc:ctor( Data )
	self:Init();
end

function DBSubProc:LuaTestCB( from, proto_buf )

	local msg_session = protobuf.decode("MsgSession" , proto_buf)
	
	print("Sub Thread DBSubProc Callback " .. from);
	print(msg_session.check);
	print(msg_session.session);
	print(msg_session.index);
	
	PostMain( "thd_player", "EVT_DB_MAIN", "MsgSession", msg_session, from );
	
end

function DBSubProc:Init()
	
	self._EventRegister = EventRegister.new();
	self._EventRegister:RegisterEvent( "EVT_DB_SUB",  self, self.LuaTestCB );
end

--释放函数
function DBSubProc:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end


return DBSubProc;