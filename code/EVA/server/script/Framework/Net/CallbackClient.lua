local CallbackClient = class("CallbackClient")

function CallbackClient:ctor()
	self.Service = nil;
	self.ConnectCallbackEvent = "";
	self.DisConnectCallbackEvent = "";
end

function CallbackClient:Listen( name, port )
	self.ConnectCallbackEvent     = name .. "Connection";
	self.DisConnectCallbackEvent  = name .. "DisConnection";
	
	self.Service = bin_types.WebSocketNetwork.NewInstance(name, port);
end

function CallbackClient:Send( sock_id, msg_type, proto_type, proto_msg )

	code = protobuf.encode(proto_type, proto_msg)
	len  = string.len(code);
	
	msg = { sock_id, msg_type, len };
	self.Service:Send( code, msg );

end

return CallbackClient

