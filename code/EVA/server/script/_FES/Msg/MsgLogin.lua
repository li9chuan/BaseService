local MsgLogin = class("MsgLogin")

-- 构造函数;
function MsgLogin:ctor( Data )

	self._EventRegister = EventRegister.new();
	
	self._EventRegister:RegisterEvent( ClientService.ConnectCallbackEvent,  	self, self.Connect );
	self._EventRegister:RegisterEvent( ClientService.DisConnectCallbackEvent,   self, self.DisConnect );
	
    --  客户端消息
	self._EventRegister:RegisterEvent( "LOGIN",     self, self.CBLogin );
    
    --  服务器间消息
    self._EventRegister:RegisterEvent( "AuthOk",    self, self.CBAuthOk );          -- 有客户端在其它FES上登录成功。RemoveClient
    self._EventRegister:RegisterEvent( "LoginPLS",  self, self.CBLoginPLS );        -- 在PLS上登录成功。
    
	
end

function MsgLogin:CBLogin( sock_id, proto_buf )

	local msg_login = protobuf.decode("PB_MSG.MsgLogin", proto_buf)
	
	PrintTable(msg_login);
	
	local sign_str = msg_login.UID .. msg_login.Channel .. msg_login.RoomType .. msg_login.AppName;
          sign_str = sign_str .. msg_login.User .. msg_login.NonceStr .. msg_login.Timestamp;
          sign_str = sign_str .. "BLACKSHEEPWALL";
        
	local sign     = string.upper( md5(sign_str) );
    local sig1     = sign_str;
    
    
	print("sig:"..sign);
	print("sig:"..sig1);
    
    
    
    --------------  账号认证通过
    
    local MsgSvrLogin = {};
    MsgSvrLogin["UID"]          = msg_login.UID;
    MsgSvrLogin["RoomType"]     = msg_login.RoomType;
    
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
    
    ClientService:SetUIDMap(client.UID, client.SockID);
    
	--  通知客户端 账号认证通过.
    ClientService:Send( sock_id, "AuthOk" );

end

-- 有客户端在其它FES上登录成功。RemoveClient
function MsgLogin:CBAuthOk( sock_id, proto_buf )

	local MsgSvrLogin = protobuf.decode("PB_MSG.MsgSvrLogin", proto_buf)
    
	print("MsgLogin:AuthOk"..MsgSvrLogin.UID);
    ClientMgr:RemoveClient(MsgSvrLogin.UID);
    
end

function MsgLogin:CBLoginPLS( sock_id, proto_buf )
    
	local MsgSvrLogin = protobuf.decode("PB_MSG.MsgSvrLogin", proto_buf)
    local client = ClientMgr:GetClient(MsgSvrLogin.UID);
    
    if( client ~= nil ) then
        client.ConPLS = sock_id;
    end
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
