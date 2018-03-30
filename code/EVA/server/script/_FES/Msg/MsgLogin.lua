--========================================================= 
-- 消息注册管理
--=========================================================
local MsgLogin = class("MsgLogin")

-- 构造函数;
function MsgLogin:ctor( Data )

	self._EventRegister = EventRegister.new();
	
	self._EventRegister:RegisterEvent( ClientService.ConnectCallbackEvent,  	self, self.Connect );
	self._EventRegister:RegisterEvent( ClientService.DisConnectCallbackEvent,   self, self.DisConnect );
	
	self._EventRegister:RegisterEvent( "LOGIN",  self, self.Login );
	
end

function MsgLogin:Login( sock_id, proto_buf )

	local msg_login = protobuf.decode("PB_MSG.MsgLogin", proto_buf)
	
	print(msg_login.version);
	print(msg_login.authType);
	print(msg_login.appid);
	print(msg_login.user);
	print(msg_login.key);
	print(msg_login.token);
	print(msg_login.timestamp);
	
	local sign_str = msg_login.version .. msg_login.authType .. msg_login.appid .. "BLACKSHEEPWALL";
	local sign     = md5( sign_str );
	
	print("sig:"..sign);
	
	ClientService:Send( sock_id, "AuthOk", "PB_MSG.MsgLogin", msg_login );
    
    proto_authok = { ext64_0=msg_login.uid, ext64_1=sock_id };
    
    FrontEndService:Broadcast( "FES", "AuthOk", "MsgData", proto_authok )      -- 通知其它网关有玩家登录成功。
    FrontEndService:Broadcast( "PLS", "AuthOk", "MsgData", proto_authok )      -- 玩家认证通过，请求发送数据。
	
end

function MsgLogin:Connect( sock_id, proto_buf )

	print("CallbackClient:Connect"..sock_id);
end

function MsgLogin:DisConnect( sock_id, proto_buf )
	print("CallbackClient:DisConnect"..sock_id);

end


--释放函数
function MsgLogin:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgLogin;