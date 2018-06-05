DdzCardtypes = {}

function DdzCardtypes:Init()


end

-- out_cards      需要判断牌型的牌。
-- first_type     如果结果中有多个牌型，需要优先选择的牌型。
function DdzCardtypes:GetCardType( out_cards, first_type )

    local out_cnt = #out_cards;

    if out_cnt==0 then
        -- 空牌
        return enum.CT_DDZ_ERROR;
    elseif out_cnt==1 then
        -- 单牌
        return enum.CT_DDZ_SINGLE;
    elseif out_cnt==2 then
        if out_cards[1]>0x40 and out_cards[2]>0x40 then
            -- 火箭
            return enum.CT_DDZ_HUOJIAN;
        elseif GetPokerLogicValue(out_cards[1])==GetPokerLogicValue(out_cards[2]) then
            -- 对子
            return enum.CT_DDZ_DOUBLE;
        end

        return enum.CT_DDZ_ERROR;
    end

    -- 牌分组
    local ddz_anly = CardsAnalyseRes:new();
    ddz_anly:Analyse( out_cards );


    local cnt_4 = ddz_anly:GetCount(4);

    if cnt_4>0 then

        if cnt_4==1 and out_cnt==4 then
            -- 炸弹
            return enum.CT_DDZ_ZHADAN_SIZHANG;
        elseif cnt_4==1 and out_cnt==6 then
            -- 四带两个单牌
            return enum.CT_DDZ_FOUR_WITHDOUBLE;
        elseif cnt_4==1 and out_cnt==8 and ddz_anly:GetCount(2)==2 then
            -- 四带两对
            return enum.CT_DDZ_FOUR_LIANGDUI;
        elseif cnt_4==2 and out_cnt==8 then
            
            if first_type==enum.CT_DDZ_FOUR_LIANGDUI then
                -- 四带两对
                return enum.CT_DDZ_FOUR_LIANGDUI;
            end

            local anly_datas    = ddz_anly:GetDatas(4);
            local first_val     = GetPokerLogicValue(anly_datas[1]);
            local second_val    = GetPokerLogicValue(anly_datas[2]);

            if (first_val+1)==second_val then 
                -- 飞机带两张
                return enum.CT_DDZ_FEIJI_WITH_ONE;
            end

            return enum.CT_DDZ_FOUR_LIANGDUI;
        end
    end

    local cnt_3 = ddz_anly:GetCount(3);
    local cnt_2 = ddz_anly:GetCount(2);
    local cnt_1 = ddz_anly:GetCount(1);

    if cnt_3>0 then
        if out_cnt==3 then
            -- 三张
            return enum.CT_DDZ_THREE_TIAO;
        elseif cnt_3==1 and out_cnt==4 then
            -- 三带一
            return enum.CT_DDZ_THREE_TIAO_WITH_ONE;
        elseif cnt_3==1 and out_cnt==5 and cnt_2==1 then
            -- 三带二
            return enum.CT_DDZ_THREE_TIAO_WITH_YIDUI;
        elseif cnt_3==2 then
            -- 飞机
            if out_cnt==6 then
                -- 飞机不带
                return enum.CT_DDZ_FEIJI_WITH_NULL;
            else
                if ddz_anly:IsLink(3) then
                    if out_cnt==8 then
                        if cnt_2==1 or cnt_1==2 then
                            -- 飞机带两张
                            return enum.CT_DDZ_FEIJI_WITH_ONE;
                        end
                    elseif out_cnt==10 and cnt_2==2 then
                        -- 飞机带两对
                        return enum.CT_DDZ_FEIJI_WITH_YIDUI;
                    end
                end
            end
        end
    elseif cnt_2>=3 and cnt_2*2==out_cnt and ddz_anly:IsLink(2) then
        -- 连对
        return enum.CT_DDZ_LIAN_DUI;
    elseif cnt_1>=5 and cnt_1==out_cnt and ddz_anly:IsLink(1) then
        -- 顺子
        return enum.CT_DDZ_SHUN_ZI;
    end

    return enum.CT_DDZ_ERROR;
end




function DdzCardtypes:CompareCards( curr_out, last_out )

    if curr_out.Type == enum.CT_DDZ_HUOJIAN then
        return true;
    end

    if curr_out.Type==enum.CT_DDZ_ERROR or last_out.Type==enum.CT_DDZ_HUOJIAN  then
        return false; 
    end

    -- 比较炸弹
    if curr_out.Type==enum.CT_DDZ_ZHADAN_SIZHANG or last_out.Type==enum.CT_DDZ_ZHADAN_SIZHANG then

        local curr_isbomb = curr_out.Type==enum.CT_DDZ_ZHADAN_SIZHANG;
        local last_isbomb = last_out.Type==enum.CT_DDZ_ZHADAN_SIZHANG;

        if last_isbomb and not curr_isbomb then
            -- 上家出的是炸弹，自己出的不是炸弹
            return false;
        elseif not last_isbomb and curr_isbomb then
            -- 上家出的不是炸弹，自己出的是炸弹
            return true;
        elseif last_isbomb and curr_isbomb then
            -- 都是炸弹，比较炸弹大小
            return (GetPokerLogicValue(curr_out.Cards[0]) > GetPokerLogicValue(last_out.Cards[0]));
        end

    end
    
    -- 1.牌型不相同时直接放回错误  2.牌的数量必须相同
    if curr_out.Type ~= last_out.Type  or  #curr_out.Cards ~= curr_out.Cards then
        return false;
    end

    local ctype = curr_out.Type;

    -- 对比其他牌型
    if ctype==enum.CT_DDZ_SINGLE or ctype==enum.CT_DDZ_DOUBLE or ctype==enum.CT_DDZ_LIAN_DUI or ctype==enum.CT_DDZ_SHUN_ZI then
        return (GetPokerLogicValue(curr_out.Cards[0]) > GetPokerLogicValue(last_out.Cards[0]));
    elseif ctype==enum.CT_DDZ_FEIJI_WITH_YIDUI or ctype==enum.CT_DDZ_FEIJI_WITH_ONE then

    elseif ctype==enum.CT_DDZ_FEIJI_WITH_NULL then

    elseif ctype==enum.CT_DDZ_FOUR_WITHDOUBLE or ctype==enum.CT_DDZ_FOUR_LIANGDUI then

    elseif ctype==enum.CT_DDZ_THREE_TIAO or ctype==enum.CT_DDZ_THREE_TIAO_WITH_ONE or ctype==enum.CT_DDZ_THREE_TIAO_WITH_YIDUI then

    end

    return false;
end

