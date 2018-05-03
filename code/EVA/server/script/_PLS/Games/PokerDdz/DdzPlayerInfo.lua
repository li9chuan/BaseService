local DdzPlayerInfo = class("DdzPlayerInfo", RoomPlayerBase)

-- ¹¹Ôìº¯Êý;
function DdzPlayerInfo:ctor()

    self.super:ctor();
    
end

function DdzPlayerInfo:IsReady()
    return self:GetStateEnum("PB.TDDZPlayerState", "STATE_DDZ_READY");
end

function DdzPlayerInfo:SetReady()
    return self:SetStateEnum("PB.TDDZPlayerState", "STATE_DDZ_READY");
end

function DdzPlayerInfo:CancleReady()
    return self:ClearStateEnum("PB.TDDZPlayerState", "STATE_DDZ_READY");
end

return DdzPlayerInfo;
