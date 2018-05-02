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
    self._EventRegister:RegisterEvent( "SyncData",  self, self.CBLoginPLS );        -- 在PLS上登录成功。
    
	
end

function MsgLogin:CBLogin( sock_id, msg_login )

    local tbl_login = msg_login:rpb("PB.MsgLogin");
	PrintTable(tbl_login);
    
    -- 验证签名
	local sign_str = tbl_login.UID .. tbl_login.Channel .. tbl_login.RoomType .. tbl_login.AppName;
          sign_str = sign_str .. tbl_login.User .. tbl_login.NonceStr .. tbl_login.Timestamp;
          sign_str = sign_str .. "BLACKSHEEPWALL";
        
	local sign     = string.upper( md5(sign_str) );


	print("sign_str:"..sign_str);
	print("sign:"..sign);
    
    
    
    --------------  账号认证通过
    local msg_authok = CMessage("AuthOk");
    msg_authok:wint64(tbl_login.UID);
    BaseService:Broadcast( "FES", msg_authok )      -- 通知其它网关有玩家登录成功。
    
    msg_authok:wstring(tbl_login.RoomType);
    BaseService:Broadcast( "SCH", msg_authok )      -- 玩家认证通过，请求发送数据。
    
    -- 保存客户端连接
    local client = ClientMgr:GetClient(tbl_login.UID);
    
    if( client ~= nil ) then
        client.SockID = sock_id;
    else
        client              = Client:new();
        client.SockID       = sock_id;
        client.UID          = tbl_login.UID;
        
        ClientMgr:SetClient(tbl_login.UID, client);
    end

	--  通知客户端 账号认证通过.
    ClientService:Send( sock_id, "AuthOk" );

end

-- 有客户端在其它FES上登录成功。RemoveClient
function MsgLogin:CBAuthOk( sock_id, msg_authok )

    local uid = msg_authok:rint64();
	print("MsgLogin:AuthOk"..uid);
    ClientMgr:RemoveClient(uid);
    
end

function MsgLogin:CBLoginPLS( pls_id, msg_sdata_2 )
    
	local uid           = msg_sdata_2:rint64();
    local client = ClientMgr:GetClient(uid);
    
    if( client ~= nil ) then
        client.ConPLS = pls_id;
        
        -- 设置UID相关信息，用于底层转发消息。   msg.xml  
        ClientService:SetClientData( {client.UID, client.SockID, pls_id} );
    end
end


function MsgLogin:Connect( sock_id )
	print("CallbackClient:Connect"..sock_id);
end

function MsgLogin:DisConnect( sock_id )
	print("CallbackClient:DisConnect"..sock_id);
    ClientMgr:RemoveSockID(sock_id);
end


--释放函数
function MsgLogin:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgLogin;
