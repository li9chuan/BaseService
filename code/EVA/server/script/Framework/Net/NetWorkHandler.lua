NetWorkHandler = {};
local this = NetWorkHandler;

-- 收到连接，断开事件
function NetWorkHandler.OnNetEvent( event_from, event_type, event_val )	
	EventController.Instance():TriggerEvent( event_type, event_from, event_val );
end

-- 收到网络消息
function NetWorkHandler.OnMessage( msg_from, lua_msg )	
    EventController.Instance():TriggerEvent( lua_msg:name(), msg_from, lua_msg );
end

function NetWorkHandler.OnError(state)

end

