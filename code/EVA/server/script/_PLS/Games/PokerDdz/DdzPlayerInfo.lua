local DdzPlayerInfo = class("DdzPlayerInfo", RoomPlayerBase)

--[[
        房间内玩家数据
--]]

function DdzPlayerInfo:ctor()

    self.super:ctor();
    
    self.QiangDiZhu     = 0;    -- 玩家抢地主选择     TDDZQiangDiZhu  TDDZJiaoFen
    
    self._TempCards     = {};

    self._OutCount      = 0;    -- 玩家出牌次数，地主被春天时使用
    self.Multi          = 1;
    self.StartScore     = 0;    -- 每局开始时玩家的身上分数
    
end

function DdzPlayerInfo:AddOutCount()
    self._OutCount = self._OutCount + 1;
end

function DdzPlayerInfo:GetOutCount()
    return self._OutCount;
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

function DdzPlayerInfo:RemoveCards( cards )
    
    self._TempCards = {};
    for i,c in ipairs(self.HandCards) do
        self._TempCards[i] = c;
    end

    for i,c in ipairs(cards) do
        for ih,ch in ipairs(self._TempCards) do
            if ch==c then
                table.remove(self._TempCards, ih);
                break;
            end
        end
    end
    
    if #self._TempCards + #cards == #self.HandCards then
        self.HandCards = {};
        for i,c in ipairs(self._TempCards) do
            self.HandCards[i] = c;
        end
        return true;
    end
    
    return false;
end


-- 清除每局临时数据
function DdzPlayerInfo:ClearOneGameData()
    self:ClearOneGameState();
    
    self.StartScore = self._Score;
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

function DdzPlayerInfo:GetCardCount()
    return #self.HandCards;
end


return DdzPlayerInfo;
