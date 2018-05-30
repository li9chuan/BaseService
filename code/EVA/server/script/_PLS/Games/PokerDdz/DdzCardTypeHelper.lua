DdzCardtypes = {}

function DdzCardtypes:Init()


end


function DdzCardtypes:GetCardType( hand_cards, analyse_res )

    for i,card in ipairs(hand_cards) do

        local same_cnt  = 1;
        local card_val  = 0;
        local logic_val = GetPokerLogicValue(card);
        
        --for _,card in ipairs(hand_cards) do

    end

end



return DdzCardtypes;
