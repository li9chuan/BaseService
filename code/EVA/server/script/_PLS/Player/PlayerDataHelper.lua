local PlayerDataHelper = class("PlayerDataHelper")

-- 构造函数;
function PlayerDataHelper:ctor()
    self.f_uid          = nil;
    self.f_nickname     = nil;
    self.f_portrait     = nil;
    self.f_money        = nil;
    self.f_rmb          = nil;
    self.f_main         = nil;
    self.f_flag_bit     = nil;
end



return PlayerDataHelper;