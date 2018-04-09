local PlayerHelper = class("PlayerHelper")

-- 构造函数;
function PlayerHelper:ctor()
    
    self.PlayerDataHelper      = nil;


    self.UID            = 0;
    self.ConFES         = nil;
end


return PlayerHelper;
