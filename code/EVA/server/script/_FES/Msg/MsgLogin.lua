local MsgLogin = class("MsgLogin")

-- 构造函数;
function MsgLogin:ctor( Data )

	self._EventRegister = EventRegister.new();
	
	self._EventRegister:RegisterEvent( ClientService.ConnectCallbackEvent,  	self, self.Connect );
	self._EventRegister:RegisterEvent( ClientService.DisConnectCallbackEvent,   self, self.DisConnect );
	
    --  客户端消息
	self._EventRegister:RegisterEvent( "LOGIN",  self, self.Login );
    
    --  服务器间消息
    self._EventRegister:RegisterEvent( "AuthOk",  self, self.AuthOk );
	
end

function MsgLogin:Login( sock_id, proto_buf )

	local msg_login = protobuf.decode("PB_MSG.MsgLogin", proto_buf)
	
	print(msg_login.Version);
	print(msg_login.AuthType);
	print(msg_login.APPID);
	print(msg_login.User);
	print(msg_login.NonceStr);
	print(msg_login.Token);
	print(msg_login.Timestamp);
    print(msg_login.UID);
    print(msg_login.GameType);
	
	local sign_str = msg_login.Version .. msg_login.AuthType .. msg_login.APPID .. "BLACKSHEEPWALL";
	local sign     = md5( string.upper(sign_str) );
	
	print("sig:"..sign);
	
	--  通知客户端 账号认证通过.
    ClientService:Send( sock_id, "AuthOk" );
    
    
    local MsgSvrLogin = {};
    MsgSvrLogin["UID"]          = msg_login.UID;
    MsgSvrLogin["GameType"]     = msg_login.GameType;
    
    BaseService:Broadcast( "FES", "AuthOk", "PB_MSG.MsgSvrLogin", MsgSvrLogin )      -- 通知其它网关有玩家登录成功。
    BaseService:Broadcast( "SCH", "AuthOk", "PB_MSG.MsgSvrLogin", MsgSvrLogin )      -- 玩家认证通过，请求发送数据。
    
    
    local client = ClientMgr:GetClient(msg_login.UID);
    
    if( client ~= nil ) then
        client.SockID = sock_id;
    else
        
        client              = Client:new();
        client.SockID       = sock_id;
        client.UID          = msg_login.UID;
        
        ClientMgr:SetClient(msg_login.UID, client);
    end
    

end

-- 有客户端在其它FES上登录成功。RemoveClient
function MsgLogin:AuthOk( sock_id, proto_buf )

	local MsgSvrLogin = protobuf.decode("PB_MSG.MsgSvrLogin", proto_buf)
    
	print("MsgLogin:AuthOk"..MsgSvrLogin.UID);
    ClientMgr:RemoveClient(MsgSvrLogin.UID);
    
end

function MsgLogin:Connect( sock_id, proto_buf )

	print("CallbackClient:Connect"..sock_id);
end

function MsgLogin:DisConnect( sock_id, proto_buf )
	print("CallbackClient:DisConnect"..sock_id);
    ClientMgr:RemoveSockID(sock_id);
end


--释放函数
function MsgLogin:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgLogin;