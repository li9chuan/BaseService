local DdzPlayerInfo = class("DdzPlayerInfo", RoomPlayerBase)

-- 构造函数;
function DdzPlayerInfo:ctor()

    self.super:ctor();
    
    self.HandCards = {};
    
    self.QiangDiZhu = 0;    -- 抢地主权限     TDDZQiangDiZhu  TDDZJiaoFen
end

function DdzPlayerInfo:AddHandCards( tbl, start_idx, end_idx )
    self.HandCards = {};
    local i=1;
    for idx=start_idx, end_idx do
        self.HandCards[i] = tbl[idx];
        i = i+1;
    end
end

-- 清除每局临时数据
function DdzPlayerInfo:ClearOneGameData()
    self:ClearAllState();
end


function DdzPlayerInfo:IsSelectJiaBei()
    return self:GetStateEnum("PB.TDDZPlayerState", "STATE_DDZ_SELECT_JIABEI");
end


function DdzPlayerInfo:IsQiangDiZhu()
    return self:GetStateEnum("PB.TDDZPlayerState", "STATE_DDZ_QIANGDIZHU");
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
