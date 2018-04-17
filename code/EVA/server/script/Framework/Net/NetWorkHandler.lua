NetWorkHandler = {};
local this = NetWorkHandler;

-- 收到网络消息，由C++调起
function NetWorkHandler.OnMessage( msg_from, msg_type, proto_buf )	
	EventController.Instance():TriggerEvent( msg_type, msg_from, proto_buf );
end

function NetWorkHandler.OnTestMessage( msg_from, lua_msg )	

    nlinfo(msg_from.."NetWorkHandler.OnTestMessage   " .. lua_msg:name());
    
    EventController.Instance():TriggerEvent( lua_msg:name(), msg_from, lua_msg );
    
end

function NetWorkHandler.OnError(state)

end

