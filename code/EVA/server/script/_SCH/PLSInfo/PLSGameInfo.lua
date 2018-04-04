local PLSGameInfo = class("PLSGameInfo")

-- 构造函数;
function PLSGameInfo:ctor()
    self.type       = "";
	self.max        = 0;
	self.curr       = 0;
end

function PLSGameInfo:IsFull()
    if( self.curr > self.max ) then
        return true;
    end
    return false;
end

return PLSGameInfo;
