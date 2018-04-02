--========================================================= 
-- 消息注册管理
--=========================================================
local MsgLogin = class("MsgLogin")

-- 构造函数;
function MsgLogin:ctor( Data )
	self._EventRegister = EventRegister.new();
	self._EventRegister:RegisterEvent( "AuthOk",  self, self.AuthOkCB );
end

function MsgLogin:AuthOkCB( msg_from, proto_buf )

	local pb_authok = protobuf.decode("MsgData" , proto_buf)
	
    print("MsgLogin:AuthOkCB");
	print(pb_authok.ext640);
	print(pb_authok.ext641);
	


	
end




--释放函数
function MsgLogin:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgLogin;
