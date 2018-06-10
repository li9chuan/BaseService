ClientMgr = {}

-- 初始化函数
function ClientMgr:Init()
	self.ClientMap      = {};
    self.SocketMap      = {};
end

function ClientMgr:GetClient( _uid )
    return self.ClientMap[_uid];
end

function ClientMgr:SetClient( _uid, _sock_id )
    if self.ClientMap[_uid] == nil then
        local client    = Client:new();
        client.SockID   = _sock_id;
        client.UID      = _uid;

        self.ClientMap[_uid] = client;
        self.SocketMap[_sock_id] = client;
    else
        local client    = self.ClientMap[_uid];

        self.SocketMap[client.SockID] = nil;
        ClientService:RemoveClientData(_uid);
        ClientService:DisConnect(client.SockID);
        
        client.ConPLS   = nil;
        client.SockID   = _sock_id;
        self.SocketMap[_sock_id] = client;
    end
end

-- 客户端逻辑层心跳，刷新心跳时间。
function ClientMgr:ResetHeartbeat( _uid )
    local client = self.ClientMap[_uid];

    if client ~= nil then
        client:ResetHeartbeat();
    end
end

function ClientMgr:RemoveClient( _uid )
	if self.ClientMap[_uid] ~= nil then
        local old_client = self.ClientMap[_uid];
        self.SocketMap[old_client.SockID] = nil;
        self.ClientMap[_uid] = nil;
        
        ClientService:RemoveClientData(_uid);
        old_client:Release();
    end
end

function ClientMgr:RemoveSockID( _sockid )
	if self.SocketMap[_sockid] ~= nil then
        local old_client = self.SocketMap[_sockid];
        
        nlinfo("ClientMgr:RemoveSockID:".._sockid.."   UID:"..old_client.UID);

        ClientService:RemoveClientData(old_client.UID);
        self.ClientMap[old_client.UID] = nil;
        self.SocketMap[_sockid] = nil;
        old_client:Release();
    end
end

return ClientMgr
