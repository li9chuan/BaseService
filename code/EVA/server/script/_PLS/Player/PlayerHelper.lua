local PlayerHelper = class("PlayerHelper")

-- 构造函数;
function PlayerHelper:ctor()
    
    self.PlayerDataHelper      = nil;


    self.UID                    = 0;
    self.ConFES                 = nil;
    
    
    self.IsRobot                = false;
    
    self.RoomID                 = 0;
    self.JoinRoomTime           = 0;

    self.OfflineTime            = 0;
    self.LogoutTime             = nil;
    self.PlayerState            = nil;
        
end


return PlayerHelper;
