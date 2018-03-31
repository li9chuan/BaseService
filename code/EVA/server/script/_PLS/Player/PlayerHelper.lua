local PlayerHalper = class("PlayerHalper")

-- ¹¹Ôìº¯Êý;
function PlayerHalper:ctor()
    self.PlayerDataHalper      = PlayerDataHalper:new();


    self.UID     = 0;
    self.FES     = nil;
end




return PlayerHalper;