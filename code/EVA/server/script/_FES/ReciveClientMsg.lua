--========================================================= 
-- 消息注册管理
--=========================================================
local ReciveClientMsg = class("ReciveClientMsg")

-- 构造函数;
function ReciveClientMsg:ctor( Data )
	self:Init();
end

function ReciveClientMsg:LuaTestCB( sock_id, proto_buf )

	local msg_session = protobuf.decode("MsgSession" , proto_buf)
	
	print(msg_session.check);
	print(msg_session.session);
	print(msg_session.index);
	
	code = protobuf.encode("MsgSession", msg_session)
	len  = string.len(code);
	
	msg = { sock_id, "LTst", len };
	web_sock:Send( code, msg );
	
end

function ReciveClientMsg:Init()
	
	self._EventRegister = EventRegister.new();
	self._EventRegister:RegisterEvent( "TEST_MSG",  self, self.LuaTestCB );
end

--释放函数
function ReciveClientMsg:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end


return ReciveClientMsg;