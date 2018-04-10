local WebSocketCbClient = class("WebSocketCbClient")

function WebSocketCbClient:ctor()
	self.Service = nil;
	self.ConnectCallbackEvent = "";
	self.DisConnectCallbackEvent = "";
end

function WebSocketCbClient:Listen( name, port )
	self.ConnectCallbackEvent     = name .. "Connection";
	self.DisConnectCallbackEvent  = name .. "DisConnection";
	
	self.Service = WebSocketNetwork.NewInstance(name, port);
end

function WebSocketCbClient:SetUIDMap( uid, sock_id )
	self.Service:SetUIDMap(uid, sock_id);
end

function WebSocketCbClient:RemoveUIDMap( uid )
	self.Service:RemoveUIDMap(uid);
end

function WebSocketCbClient:Send( sock_id, msg_type, proto_type, proto_msg )

	local code = "";
	local len  = 0;
    
    if proto_type~=nil then
        code = protobuf.encode(proto_type, proto_msg);
        len  = string.len(code);
    end

	
	msg = { sock_id, msg_type, len };
	self.Service:Send( code, msg );

end

return WebSocketCbClient

