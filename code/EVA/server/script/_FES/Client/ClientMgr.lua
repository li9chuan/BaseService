ClientMgr = {}

-- 初始化函数
function ClientMgr:Init()
	self.ClientMap      = {};
    self.SocketMap      = {};
end

function ClientMgr:GetClient( _uid )
    if self.ClientMap[_uid] ~= nil then
        return self.ClientMap[_uid];
    end
    return nil;
end

function ClientMgr:SetClient( _uid, _client )
    
    if _uid ~= _client.UID then  assert()  end;
    
	if self.ClientMap[_uid] == nil then
        self.ClientMap[_uid] = _client;
        self.SocketMap[_client.SockID] = self.ClientMap[_uid];
    else
        local old_client = self.ClientMap[_client.UID];
        self.SocketMap[old_client.SockID] = nil;

        self.ClientMap[_uid] = _client;
        self.SocketMap[_client.SockID] = self.ClientMap[_uid];
    end
end

function ClientMgr:RemoveClient( _uid )
	if self.ClientMap[_uid] ~= nil then
        local old_client = self.ClientMap[_uid];
        self.SocketMap[old_client.SockID] = nil;
        self.ClientMap[_uid] = nil;
        
        ClientService:RemoveClientData(_uid);
    end
end

function ClientMgr:RemoveSockID( _sockid )
	if self.SocketMap[_sockid] ~= nil then
        local old_client = self.SocketMap[_sockid];
        
        print("ClientMgr:RemoveSockID:".._sockid.."   UID:"..old_client.UID);
        
        ClientService:RemoveClientData(old_client.UID);
        self.ClientMap[old_client.UID] = nil;
        self.SocketMap[_sockid] = nil;
    end
end

return ClientMgr
