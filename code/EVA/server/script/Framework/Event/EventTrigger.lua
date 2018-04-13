EventTrigger = {};
local this = EventTrigger;

function EventTrigger.OnEvent( msg_type, ... )	
    EventController.Instance():TriggerEvent( msg_type, ... );
end


