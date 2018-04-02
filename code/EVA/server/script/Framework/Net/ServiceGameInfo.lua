local ServiceGameInfo = class("ServiceGameInfo")

-- 构造函数;
function ServiceGameInfo:ctor()
    self.type       = "";
	self.max        = 0;
	self.curr       = 0;
end

return ServiceGameInfo;