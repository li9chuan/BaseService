local RoomInfo = class("RoomInfo")

-- 构造函数;
function RoomInfo:ctor()
    
	self.PLSID          = -1;
    self.RoomType       = "";
    self.RoomID         = 0;
    self.PlayerID       = 0;    -- 创建者ID


end



return RoomInfo;


