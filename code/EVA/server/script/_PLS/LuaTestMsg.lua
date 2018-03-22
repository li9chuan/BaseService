--========================================================= 
-- 消息注册管理
--=========================================================
local LuaTestMsg = class("LuaTestMsg")

-- 构造函数;
function LuaTestMsg:ctor( Data )
	self:Init();
end

function LuaTestMsg:LuaTestCB( msg_from, proto_buf )

	local msg_session = protobuf.decode("MsgSession" , proto_buf)
	
	print(msg_session.check);
	print(msg_session.session);
	print(msg_session.index);
	
	code = protobuf.encode("MsgSession", msg_session)
	
	msg = { "LTst", "neirong" };
	ServerNet.Send( msg_from, msg );
	
end

function LuaTestMsg:Init()
	
	self._EventRegister = EventRegister.new();
	self._EventRegister:RegisterEvent( "LuaTestCB",  self, self.LuaTestCB );
end

--释放函数
function LuaTestMsg:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
	
	
	print("LuaTestMsg:OnRelease");
end


return LuaTestMsg;