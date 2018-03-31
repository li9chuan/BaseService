--========================================================= 
-- 消息注册管理
--=========================================================
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
	
	print(msg_login.version);
	print(msg_login.authType);
	print(msg_login.appid);
	print(msg_login.user);
	print(msg_login.key);
	print(msg_login.token);
	print(msg_login.timestamp);
    print(msg_login.uid);
	
	local sign_str = msg_login.version .. msg_login.authType .. msg_login.appid .. "BLACKSHEEPWALL";
	local sign     = md5( sign_str );
	
	print("sig:"..sign);
	
	--  通知客户端 账号认证通过.
    ClientService:Send( sock_id, "AuthOk" );
    
    
    local MsgData = {};
    MsgData["ext640"] = msg_login.uid;
    MsgData["ext641"] = sock_id;
    
    FrontEndService:Broadcast( "FES", "AuthOk", "PB_MSG.MsgData", MsgData )      -- 通知其它网关有玩家登录成功。
    FrontEndService:Broadcast( "PLS", "AuthOk", "PB_MSG.MsgData", MsgData )      -- 玩家认证通过，请求发送数据。
    
    
    local client = ClientMgr:GetClient(msg_login.uid);
    
    if( client ~= nil ) then
        client.SockID = sock_id;
    else
        
        client              = Client:new();
        client.SockID       = sock_id;
        client.UID          = msg_login.uid;
        
        ClientMgr:SetClient(msg_login.uid, client);
    end
    

end

-- 有客户端在其它FES上登录成功。RemoveClient
function MsgLogin:AuthOk( sock_id, proto_buf )

	local MsgData = protobuf.decode("PB_MSG.MsgData", proto_buf)
    
	print(MsgData.ext640);
	print(MsgData.ext641);
    
    local uid = MsgData.exe640;
    ClientMgr:RemoveClient(uid);
    
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