local CallbackServer = class("CallbackServer")

function CallbackServer:ctor()
	self.Service = nil;
	self.ConnectCallbackEvent = "";
	self.DisConnectCallbackEvent = "";
end

function CallbackServer:Listen( name, protocal, port )
	self.ConnectCallbackEvent     = name .. "Con";
	self.DisConnectCallbackEvent  = name .. "Dis";
	
    local listen_proc = string.lower(protocal);
    self.Service = bin_types.LuaCallbackServer.NewInstance( {name, listen_proc, port} );
end

function CallbackServer:SetUIDMap( uid, sock_id )
	self.Service:SetUIDMap(uid, sock_id);
end

function CallbackServer:RemoveUIDMap( uid )
	self.Service:RemoveUIDMap(uid);
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

function CallbackServer:ToMsg( msg_table )
    
    local msg_out = CMessage();
    local tbl_type = type(msg_table);
    
    if tbl_type == "nil" then
        return "json.null"
    elseif type(value) == "string" then
        return ("%q"):format(value)
    elseif type(value) == "nil" or type(value) == "number" or
           type(value) == "boolean" then
        return tostring(value)
    elseif type(value) == "table" then
        return serialise_table(value, indent, depth)
    else
        return "\"<" .. type(value) .. ">\""
    end

end

return CallbackServer

