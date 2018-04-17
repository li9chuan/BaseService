--========================================================= 
-- 消息派发
--=========================================================
EventController = singleton("EventController");

function EventController:Init()
	self._MessageQue = {};
end

function EventController:RegisterEvent(messageType, callback)
    --if EventName[messageType] == nil or type(callback) ~= "function" then
    --    return
	--end
	if (messageType == nil) then
		logError("EventController.RegisterEvent messageType == nil");
		return;
	end
	
	if (callback == nil) then
		local sError = "EventController.RegisterEvent callback == nil ";
		if (nil ~= messageType) then
			sError = sError .. "messageType = "..messageType;
		end
		logError(sError);
		return;
	end
	
    if self._MessageQue[messageType] == nil then
        self._MessageQue[messageType] = {}
    end
	
    local index = #self._MessageQue[messageType];			--  table.getn =>  #   lua5.3
    self._MessageQue[messageType][index+1] = callback
end

function EventController:TriggerEvent( msg_type, ... )

    if self._MessageQue[msg_type] == nil then
        return
    end
    for i,v in pairs(self._MessageQue[msg_type]) do
		if (v ~= nil)then
			v(...)
		else
		end
    end
	
end

function EventController:RemoveEvent(messageType,callback)
    --if EventName[messageType] == nil or type(callback) ~= "function" then
    --    return
    --end
    for i,v in pairs(self._MessageQue[messageType]) do
        if callback == v then
            table.remove(self._MessageQue[messageType],i)
            return
        end
    end
end

return EventController
