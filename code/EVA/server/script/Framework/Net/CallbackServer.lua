local CallbackServer = class("CallbackServer")

function CallbackServer:ctor()
	self.Service = nil;
	self.ConnectCallbackEvent = "";
	self.DisConnectCallbackEvent = "";
end

function CallbackServer:Init( name, protocal )
	self.ConnectCallbackEvent     = name .. "Con";
	self.DisConnectCallbackEvent  = name .. "Dis";
	
    local listen_proc = string.lower(protocal);
    self.Service = bin_types.LuaCallbackServer.NewInstance( name, listen_proc );
end

function CallbackServer:Listen( port )
    self.Service:Listen(port);
end

function CallbackServer:LoadSslCA( ssl_ca )
    self.Service:LoadSslCA(ssl_ca);
end
function CallbackServer:LoadSslCrt( ssl_crt )
    self.Service:LoadSslCrt(ssl_crt);
end
function CallbackServer:LoadSslPrivateKey( ssl_prvkey )
    self.Service:LoadSslPrivateKey(ssl_prvkey);
end

function CallbackServer:SetClientData( client_data )
	self.Service:SetClientData( client_data );
end

function CallbackServer:RemoveClientData( uid )
	self.Service:RemoveClientData(uid);
end

function CallbackServer:Send( sock_id, msg_or_type, proto_type, proto_msg )
    
    local param_type = type(msg_or_type)
    
    if param_type=="string" then
        local lua_msg   = CMessage(msg_or_type);
        if proto_type~=nil then
            local code      = protobuf.encode(proto_type, proto_msg);
            lua_msg:wstring(code);
        end
        self.Service:Send( sock_id, lua_msg );
    else
        self.Service:Send( sock_id, msg_or_type );
    end

end

return CallbackServer

