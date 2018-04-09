local PLSGameInfo = class("PLSGameInfo")

-- 构造函数;
function PLSGameInfo:ctor()
    self.Type       = "";
	self.Max        = 0;
	self.Curr       = 0;
end

function PLSGameInfo:IsFull()
    if( self.Curr > self.Max ) then
        return true;
    end
    return false;
end

return PLSGameInfo;
