local ServiceGameInfo = class("ServiceGameInfo")

-- ¹¹Ôìº¯Êý;
function ServiceGameInfo:ctor()
    self.type       = "";
	self.max        = 0;
	self.curr       = 0;
end

return ServiceGameInfo;