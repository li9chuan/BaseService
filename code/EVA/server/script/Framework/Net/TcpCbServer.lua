local TcpCbServer = class("TcpCbServer")

function TcpCbServer:ctor()
	self.Service = nil;
	self.ConnectCallbackEvent = "";
	self.DisConnectCallbackEvent = "";
end

function TcpCbServer:Listen( name, port )
	self.ConnectCallbackEvent     = name .. "Connection";
	self.DisConnectCallbackEvent  = name .. "DisConnection";
	
	self.Service = NetworkTcp.NewInstance(name, port);
end

function TcpCbServer:Send( sock_id, lua_msg )



	self.Service:Send( sock_id, lua_msg );

end

return TcpCbServer

