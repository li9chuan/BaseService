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

function PlayerDataHelper:ToProtoMsg()
    
    local MsgPlayerInfo = {};
    MsgPlayerInfo["UID"]        = self.f_uid;
    MsgPlayerInfo["Nickname"]   = self.f_nickname;
    MsgPlayerInfo["Portrait"]   = self.f_portrait;
    MsgPlayerInfo["Money"]      = self.f_money;
    MsgPlayerInfo["RMB"]        = self.f_rmb;
    MsgPlayerInfo["Main"]       = self.f_main;
    MsgPlayerInfo["FlagBit"]    = self.f_flag_bit;
    
    return MsgPlayerInfo;
end

return PlayerDataHelper;