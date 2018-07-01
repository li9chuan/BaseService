BaseService = {}

function BaseService:Send( service_id, cmsg_or_type, proto_type, proto_msg )

    local param_type = type(cmsg_or_type)
    
    if param_type=="string" then
               
        local lua_msg   = CMessage(cmsg_or_type);
        
        if type(proto_type)=="table" then           --  send table => json str
            local json_str = Table2Json(proto_type);
            lua_msg:wstring(json_str);
        elseif type(proto_type)=="string" then      --  send proto msg
            local proto_code      = protobuf.encode(proto_type, proto_msg);
            lua_msg:wbuffer(proto_code, #proto_code);
        end
        Net.Send( service_id, lua_msg );
        
    elseif param_type=="userdata" then              --  send cmessage
        Net.Send( service_id, cmsg_or_type );
    end
    
end

function BaseService:Broadcast( service_name, msg_out )
    Net.Broadcast( service_name, msg_out );
end

function BaseService:SendToClient( player, cmsg_or_type, proto_type, proto_msg )

	if player==nil then
		return
	end
	
    if player.ConFES==nil then
		return
	end

	local send_info = { player.ConFES, player.UID };
    local param_type = type(cmsg_or_type)
    
    if param_type=="string" then
               
        local lua_msg   = CMessage(cmsg_or_type);
        
        if type(proto_type)=="table" then           --  send table => json str
            local json_str = Table2Json(proto_type);
            lua_msg:wstring(json_str);
            nlwarning(json_str);
        elseif type(proto_type)=="string" then      --  send proto msg
            local proto_code      = protobuf.encode(proto_type, proto_msg);
            lua_msg:wbuffer(proto_code, #proto_code);
--            nlinfo("Msg:"..cmsg_or_type.." Buffer Len : "..#proto_code);
        end

        Net.SendToClient( lua_msg, send_info );
    elseif param_type=="userdata" then              --  send cmessage
        Net.SendToClient( cmsg_or_type, send_info );
    end

end


return BaseService

