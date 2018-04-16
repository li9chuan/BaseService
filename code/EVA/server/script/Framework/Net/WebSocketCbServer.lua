local WebSocketCbServer = class("WebSocketCbServer")

function WebSocketCbServer:ctor()
	self.Service = nil;
	self.ConnectCallbackEvent = "";
	self.DisConnectCallbackEvent = "";
end

function WebSocketCbServer:Listen( name, port )
	self.ConnectCallbackEvent     = name .. "Connection";
	self.DisConnectCallbackEvent  = name .. "DisConnection";
	
	self.Service = NetworkWebSocket.NewInstance(name, port);
end

function WebSocketCbServer:SetUIDMap( uid, sock_id )
	self.Service:SetUIDMap(uid, sock_id);
end

function WebSocketCbServer:RemoveUIDMap( uid )
	self.Service:RemoveUIDMap(uid);
end

function WebSocketCbServer:Send( sock_id, msg_type, proto_type, proto_msg )

	local code = "";
	local len  = 0;
    
    if proto_type~=nil then
        code = protobuf.encode(proto_type, proto_msg);
        len  = string.len(code);
    end

	
	msg = { sock_id, msg_type, len };
	self.Service:Send( code, msg );

end

return WebSocketCbServer

