NetWorkHandler = {};
local this = NetWorkHandler;

-- 收到网络消息，由C++调起
function NetWorkHandler.OnMessage( msg_from, msg_type, proto_buf )	
	EventController.Instance():TriggerEvent( msg_type, msg_from, proto_buf );
end

function NetWorkHandler.OnTestMessage( msg_from, lua_msg )	
	
    local  param_1 = lua_msg:ReadSint32();
    local  param_2 = lua_msg:ReadSint32();
    
    nlinfo(msg_from.."NetWorkHandler.OnTestMessage" .. param_1 .. param_2);
    
    lua_msg:Invert();
    --lua_msg:Clear();
    --lua_msg:SetType("CBSP");
    
    PHPService:Send(msg_from, lua_msg);
    
end

function NetWorkHandler.OnError(state)

end

