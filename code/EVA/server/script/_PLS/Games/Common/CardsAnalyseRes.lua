local CardsAnalyseRes = class("CardsAnalyseRes")

--[[
        牌型分析结构，表示每张相同牌的信息。
--]]

function CardsAnalyseRes:ctor()
    self:Reset();
end

function CardsAnalyseRes:Reset()
    self.CardCount      = { 0, 0, 0, 0 };
    self.CardData       = { {}, {}, {}, {} };
end

function CardsAnalyseRes:Analyse( hand_cards )

    local hand_cnt = #hand_cards;
    if hand_cnt==0 then return; end
    
    for i=1,hand_cnt do

        local card = hand_cards[i];
        local same_cnt  = 1;
        local card_val  = 0;
        local logic_val = GetPokerLogicValue(card);
        
        -- 向后遍历同牌
        for j=i,hand_cnt do
            if( GetPokerLogicValue(hand_cards[j]) == logic_val ) then
                same_cnt = same_cnt + 1;
            end
        end

        -- 保存结果
        self.CardCount[same_cnt] = self.CardCount[same_cnt] + 1;
        table.insert( self.CardData[same_cnt], card );
        
        i = i+same_cnt;
    end

end


return CardsAnalyseRes;

