BaseService = {}

function BaseService:SendPB( service_id, msg_type, proto_type, proto_msg )

    local code = protobuf.encode(proto_type, proto_msg)
    local msg_out = CMessage(msg_type);
    msg_out:wstring( code );
    ServerNet.Send( service_id, msg_out );

end

function BaseService:Send( service_id, msg_out )
    ServerNet.Send( service_id, msg_out );
end

function BaseService:Broadcast( service_name, msg_out )
    ServerNet.Broadcast( service_name, msg_out );
end

function BaseService:SendToClient( service_id, client_uid, msg_type, proto_type, proto_msg )

	local code = protobuf.encode(proto_type, proto_msg)
	local len  = string.len(code);
	
	local msg = { service_id, msg_type, len, client_uid };
	ServerNet.SendToClient( code, msg );

end


return BaseService

