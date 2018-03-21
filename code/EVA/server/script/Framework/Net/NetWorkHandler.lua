NetWorkHandler = {};
local this = NetWorkHandler;

-- 收到网络消息，由C++调起
function NetWorkHandler.OnMessage( msg_type, proto_buf )	
	EventController.Instance():TriggerEvent( msg_type, proto_buf );
end


function NetWorkHandler.OnError(state)

end

