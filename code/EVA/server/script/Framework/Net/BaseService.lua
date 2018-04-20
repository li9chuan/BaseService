BaseService = {}

function BaseService:Send( service_id, msg_or_type, proto_type, proto_msg )

    local param_type = type(msg_or_type)
    
    if param_type=="string" then
               
        local lua_msg   = CMessage(msg_or_type);
        
        if type(proto_type)=="table" then           --  send table => json str
            local json_str = Table2Json(msg_or_type);
            lua_msg:wstring(json_str);
        elseif type(proto_type)=="string" then      --  send proto msg
            local proto_code      = protobuf.encode(proto_type, proto_msg);
            lua_msg:wstring(proto_code);
        end
        ServerNet.Send( service_id, lua_msg );
        
    elseif param_type=="userdata" then              --  send cmessage
        ServerNet.Send( service_id, msg_or_type );
    end
    
end

function BaseService:Broadcast( service_name, msg_out )
    ServerNet.Broadcast( service_name, msg_out );
end

function BaseService:SendToClient( service_id, client_uid, msg_type, proto_type, proto_msg )

	local send_info = { service_id, client_uid };
	local code      = protobuf.encode(proto_type, proto_msg)
    local lua_msg   = CMessage(msg_type);
    lua_msg:wstring(code);
	ServerNet.SendToClient( lua_msg, send_info );

end


return BaseService

