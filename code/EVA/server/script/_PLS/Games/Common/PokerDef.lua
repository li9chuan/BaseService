



ConstCardsPoker = {
        0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,   -- 方块;
        0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,   -- 梅花;
        0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,   -- 红桃;
        0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,   -- 黑桃;
        0x41,0x42,                                                          -- 大小王;
}

-- 获得牌的逻辑数值，牌值大小。
function GetPokerLogicValue( card )
    
    local value = CardValue(card);
    
    -- 大小王
    if card >= 0x40 then
        return value+15;
    end
    
    if value==1 then
        return 14;
    end
    
    if value==2 then
        return 15;
    end
end


-- 按牌的逻辑值大小排序
function SortPokerLogicValue( cards )
    
    local cnt = #cards;
    if cnt>=1 then  return; end

    local logic_cards = {};

    for i,card in ipairs(cards) do
        logic_cards[i] = GetPokerLogicValue(card);
    end

    local is_sorted = false;
    local last_n    = cnt - 1;

    while ( ~is_sorted ) do
        
        is_sorted = true;

        for i=1,last_n do
            local ccurr = logic_cards[i];
            local cnext = logic_cards[i+1];

	        if ( ccurr<cnext ) or ( ccurr==cnext and cards[i]<cards[i+1] ) then

                logic_cards[i]      = cnext;
                logic_cards[i+1]    = ccurr;
                
                ccurr       = cards[i];
                cards[i]    = cards[i+1];
                cards[i+1]  = ccurr;

                -- cards[i], cards[i+1]  = cards[i+1], cards[i]

                is_sorted = false;
            end
        end

        last_n = last_n - 1;
    end
end


