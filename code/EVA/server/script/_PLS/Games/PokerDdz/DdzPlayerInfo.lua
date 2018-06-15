local DdzPlayerInfo = class("DdzPlayerInfo", RoomPlayerBase)

--[[
        房间内玩家数据
--]]

function DdzPlayerInfo:ctor()

    self.super:ctor();
    
    self.HandCards = {};
    
    self.QiangDiZhu = 0;    -- 抢地主权限     TDDZQiangDiZhu  TDDZJiaoFen
end

function DdzPlayerInfo:AddHandCards( tbl, start_idx, end_idx )
    if start_idx ~=nil then
        for idx=start_idx, end_idx do
            table.insert( self.HandCards, tbl[idx] );
        end
    else
        for _,v in ipairs(tbl) do
            table.insert( self.HandCards, v );
        end
    end
end

-- 清除每局临时数据
function DdzPlayerInfo:ClearOneGameData()
    
    self.HandCards = {};
    self:ClearAllState();
end

function DdzPlayerInfo:IsSelectJiaBei()
    return self:GetState( enum.STATE_DDZ_SELECT_JIABEI );
end

function DdzPlayerInfo:IsQiangDiZhu()
    return self:GetState( enum.STATE_DDZ_QIANGDIZHU );
end

function DdzPlayerInfo:IsReady()
    return self:GetState( enum.STATE_DDZ_READY );
end

function DdzPlayerInfo:SetReady()
    self:SetState( enum.STATE_DDZ_READY );
end

function DdzPlayerInfo:CancleReady()
    self:ClearState( enum.STATE_DDZ_READY );
end

return DdzPlayerInfo;
