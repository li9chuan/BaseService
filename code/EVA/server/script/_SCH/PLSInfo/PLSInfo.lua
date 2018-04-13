local PLSInfo = class("PLSInfo")

-- 构造函数;
function PLSInfo:ctor()
	self.ServiceID      = 0;
    self.ServiceName    = "";
    self.MaxPlayer      = 0;
    self.CurrPlayer     = 0;
    self.RoomList       = {};
end

function PLSInfo:LoadData( _service_info )
    
	self.ServiceID      = _service_info.ServiceID;
    self.ServiceName    = _service_info.ServiceName;
    self.MaxPlayer      = _service_info.MaxPlayer;
    self.CurrPlayer     = _service_info.CurrPlayer;
    self.RoomList   = {};
    
    if _service_info.RoomList~=nil then
    
        for _,v in ipairs(_service_info.RoomList) do
            
            local game_info = PLSGameInfo:new();
            game_info.Type  = v.Type;
            game_info.Max   = v.Max;
            game_info.Curr  = v.Curr;

            self.RoomList[game_info.Type] = game_info;
        end
    end
    
end

function PLSInfo:IsFull()
    if( self.CurrPlayer > self.MaxPlayer ) then
        return true;
    end
    return false;
end

return PLSInfo;
