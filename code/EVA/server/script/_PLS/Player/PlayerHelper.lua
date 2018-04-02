local PlayerHalper = class("PlayerHalper")

-- 构造函数;
function PlayerHalper:ctor()
    self.PlayerDataHalper      = PlayerDataHalper:new();


    self.UID     = 0;
    self.FES     = nil;
end




return PlayerHalper;