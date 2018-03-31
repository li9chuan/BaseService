local ServiceInfo = class("ServiceInfo")

-- ¹¹Ôìº¯Êý;
function ServiceInfo:ctor()
	self.serviceId      = 0;
    self.serviceName    = "";
    self.maxPlayer      = 0;
    self.currPlayer     = 0;
    self.gameTypeList   = {};
end

function ServiceInfo:LoadData( _service_info )
	self.serviceId      = _service_info.serviceId;
    self.serviceName    = _service_info.serviceName;
    self.maxPlayer      = _service_info.maxPlayer;
    self.currPlayer     = _service_info.currPlayer;
    
    if _service_info.gameTypeList~=nil then
    
        for _,v in ipairs(_service_info.gameTypeList) do
            
            game_info = ServiceGameInfo:new();
            game_info.type  = v.type;
            game_info.max   = v.max;
            game_info.curr  = v.curr;

            self.gameTypeList[v.type] = game_info;
        end
    end
    
end

return ServiceInfo;