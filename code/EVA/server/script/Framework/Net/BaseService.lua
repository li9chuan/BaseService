BaseService = {}

function BaseService:Send( service_id, msg_type, proto_type, proto_msg )

	code = protobuf.encode(proto_type, proto_msg)
	len  = string.len(code);
	
	msg = { service_id, msg_type, len };
	ServerNet.Send( code, msg );

end

function BaseService:Broadcast( service_name, msg_type, proto_type, proto_msg )

	code = protobuf.encode(proto_type, proto_msg)
	len  = string.len(code);
	
	msg = { service_name, msg_type, len };
	ServerNet.Broadcast( code, msg );

end

function BaseService:SendToClient( service_id, client_uid, msg_type, proto_type, proto_msg )

	code = protobuf.encode(proto_type, proto_msg)
	len  = string.len(code);
	
	msg = { service_id, msg_type, len, client_uid };
	ServerNet.SendToClient( code, msg );

end


return BaseService

