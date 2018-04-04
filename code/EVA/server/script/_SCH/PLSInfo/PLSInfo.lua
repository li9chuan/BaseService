local PLSInfo = class("PLSInfo")

-- 构造函数;
function PLSInfo:ctor()
	self.serviceId      = 0;
    self.serviceName    = "";
    self.maxPlayer      = 0;
    self.currPlayer     = 0;
    self.gameTypeList   = {};
end

function PLSInfo:LoadData( _service_info )
    
	self.serviceId      = _service_info.serviceId;
    self.serviceName    = _service_info.serviceName;
    self.maxPlayer      = _service_info.maxPlayer;
    self.currPlayer     = _service_info.currPlayer;
    self.gameTypeList   = {};
    
    if _service_info.gameTypeList~=nil then
    
        for _,v in ipairs(_service_info.gameTypeList) do
            
            local game_info = PLSGameInfo:new();
            game_info.type  = v.type;
            game_info.max   = v.max;
            game_info.curr  = v.curr;

            self.gameTypeList[game_info.type] = game_info;
        end
    end
    
end

function PLSInfo:IsFull()
    if( self.currPlayer > self.maxPlayer ) then
        return true;
    end
    return false;
end

return PLSInfo;
