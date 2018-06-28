DdzCardtypes = {}

function DdzCardtypes:Init()
    self.TempAnalyse    = CardsAnalyseRes:new();
end

-- out_cards      需要判断牌型的牌。
-- first_type     如果结果中有多个牌型，需要优先选择的牌型。
-- return  type,comp_val(max)
function DdzCardtypes:GetCardType( out_cards, first_type )

    local out_cnt = #out_cards;

    if out_cnt==0 then
        -- 空牌
        return enum.CT_DDZ_ERROR;
    elseif out_cnt==1 then
        -- 单牌
        return enum.CT_DDZ_SINGLE, GetPokerLogicValue(out_cards[1]);
    elseif out_cnt==2 then
        local val_1 = GetPokerLogicValue(out_cards[1]);
        
        if out_cards[1]>0x40 and out_cards[2]>0x40 then
            -- 火箭
            return enum.CT_DDZ_HUOJIAN, val_1;
        elseif val_1==GetPokerLogicValue(out_cards[2]) then
            -- 对子
            return enum.CT_DDZ_DOUBLE, val_1;
        end

        return enum.CT_DDZ_ERROR;
    end

    -- 牌分组
    local ddz_anly = self.TempAnalyse;
    ddz_anly:Reset();
    ddz_anly:Analyse( out_cards );

    local cnt_4 = ddz_anly:GetCount(4);

    if cnt_4>0 then
        local anly_datas    = ddz_anly:GetDatas(4);
        local first_val     = anly_datas[1];

        if cnt_4==1 and out_cnt==4 then
            -- 炸弹
            return enum.CT_DDZ_ZHADAN_SIZHANG, first_val;
        elseif cnt_4==1 and out_cnt==6 then
            -- 四带两个单牌
            return enum.CT_DDZ_FOUR_WITHDOUBLE, first_val;
        elseif cnt_4==1 and out_cnt==8 and ddz_anly:GetCount(2)==2 then
            -- 四带两对
            return enum.CT_DDZ_FOUR_LIANGDUI, first_val;
        elseif cnt_4==2 and out_cnt==8 then
            
            if first_type==enum.CT_DDZ_FOUR_LIANGDUI then
                -- 四带两对
                return enum.CT_DDZ_FOUR_LIANGDUI, first_val;
            end

            local second_val    = anly_datas[2];
            if (first_val+1)==second_val then 
                -- 飞机带两张
                return enum.CT_DDZ_FEIJI_WITH_ONE, first_val;
            end

            return enum.CT_DDZ_FOUR_LIANGDUI, first_val;
        end
    end

    local cnt_3 = ddz_anly:GetCount(3);
    local cnt_2 = ddz_anly:GetCount(2);
    local cnt_1 = ddz_anly:GetCount(1);

    if cnt_3>0 then
        local anly_datas    = ddz_anly:GetDatas(3);
        local first_val     = anly_datas[1];
        
        if out_cnt==3 then
            -- 三张
            return enum.CT_DDZ_THREE_TIAO, first_val;
        elseif cnt_3==1 and out_cnt==4 then
            -- 三带一
            return enum.CT_DDZ_THREE_TIAO_WITH_ONE, first_val;
        elseif cnt_3==1 and out_cnt==5 and cnt_2==1 then
            -- 三带二
            return enum.CT_DDZ_THREE_TIAO_WITH_YIDUI, first_val;
        elseif cnt_3==2 then
            -- 飞机
            if out_cnt==6 then
                -- 飞机不带
                return enum.CT_DDZ_FEIJI_WITH_NULL, first_val;
            else
                if ddz_anly:IsLink(3) then
                    if out_cnt==8 then
                        if cnt_2==1 or cnt_1==2 then
                            -- 飞机带两张
                            return enum.CT_DDZ_FEIJI_WITH_ONE, first_val;
                        end
                    elseif out_cnt==10 and cnt_2==2 then
                        -- 飞机带两对
                        return enum.CT_DDZ_FEIJI_WITH_YIDUI, first_val;
                    end
                end
            end
        end
    elseif cnt_2>=3 and cnt_2*2==out_cnt and ddz_anly:IsLink(2) then
        -- 连对
        local anly_datas    = ddz_anly:GetDatas(2);
        return enum.CT_DDZ_LIAN_DUI, anly_datas[1];
    elseif cnt_1>=5 and cnt_1==out_cnt and ddz_anly:IsLink(1) then
        -- 顺子
        local anly_datas    = ddz_anly:GetDatas(1);
        return enum.CT_DDZ_SHUN_ZI, anly_datas[1];
    end

    return enum.CT_DDZ_ERROR;
end



-- 如果 curr_out 大，返回true
function DdzCardtypes:CompareCards( curr_out, last_out )

    if curr_out.Type==enum.CT_DDZ_HUOJIAN or last_out.Type==enum.CT_DDZ_ERROR or curr_out.UID==last_out.UID then
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
            return curr_out.CompVal > last_out.CompVal;
        end
    end
    
    -- 基本规则判断:  1.牌型不相同时直接返回错误  2.牌的数量必须相同
    if curr_out.Type ~= last_out.Type  or  #curr_out.Cards ~= #curr_out.Cards then
        return false;
    end

    --[[
    local ctype = curr_out.Type;

    -- 对比其他牌型
    if ctype==enum.CT_DDZ_SINGLE or ctype==enum.CT_DDZ_DOUBLE or ctype==enum.CT_DDZ_LIAN_DUI or ctype==enum.CT_DDZ_SHUN_ZI then
        return curr_out.CompVal > last_out.CompVal;
    elseif ctype==enum.CT_DDZ_FEIJI_WITH_YIDUI or ctype==enum.CT_DDZ_FEIJI_WITH_ONE then

    elseif ctype==enum.CT_DDZ_FEIJI_WITH_NULL then

    elseif ctype==enum.CT_DDZ_FOUR_WITHDOUBLE or ctype==enum.CT_DDZ_FOUR_LIANGDUI then

    elseif ctype==enum.CT_DDZ_THREE_TIAO or ctype==enum.CT_DDZ_THREE_TIAO_WITH_ONE or ctype==enum.CT_DDZ_THREE_TIAO_WITH_YIDUI then

    end
    ]]

    return curr_out.CompVal > last_out.CompVal;
end

-- 检查手牌中是否有能大过 last_out 的牌.
function DdzCardtypes:CheckCanOutCards( last_out, hand_cards )

    local lastype = last_out.Type;
    
    if last_out.CompVal==0 or lastype==enum.CT_DDZ_ERROR then
        return true;
    end
    
    if lastype==enum.CT_DDZ_SINGLE then
        return self:__GetSingleCards(last_out.CompVal, hand_cards);
    elseif lastype==enum.CT_DDZ_DOUBLE then
        return self:__CheckHaveCards(last_out.CompVal, hand_cards, 2);
    elseif lastype==enum.CT_DDZ_THREE_TIAO then
        return self:__CheckHaveCards(last_out.CompVal, hand_cards, 3);
    elseif lastype==enum.CT_DDZ_THREE_TIAO_WITH_ONE then
        return self:__CheckHaveThreeOne(last_out.CompVal, hand_cards);
    elseif lastype==enum.CT_DDZ_THREE_TIAO_WITH_YIDUI then
        return self:__CheckHaveThreeDouble(last_out.CompVal, hand_cards);
    elseif lastype==enum.CT_DDZ_LIAN_DUI then
        return self:__CheckHaveDoubleLink(last_out.CompVal, hand_cards);
    elseif lastype==enum.CT_DDZ_SHUN_ZI then
        return self:__CheckHaveLinkCards(last_out.CompVal, hand_cards);
    elseif lastype==enum.CT_DDZ_FEIJI_WITH_NULL then
        return self:__CheckHaveFeiJiNull(last_out.CompVal, hand_cards);
    elseif lastype==enum.CT_DDZ_FEIJI_WITH_ONE then
        return self:__CheckHaveFeiJiOne(last_out.CompVal, hand_cards);
    elseif lastype==enum.CT_DDZ_FEIJI_WITH_YIDUI then
        return self:__CheckHaveFeiJiDouble(last_out.CompVal, hand_cards);
    elseif lastype==enum.CT_DDZ_FOUR_WITHDOUBLE then
        return self:__CheckHaveFourTwo(last_out.CompVal, hand_cards);
    elseif lastype==enum.CT_DDZ_FOUR_LIANGDUI then
        return self:__CheckHaveFourDouble(last_out.CompVal, hand_cards);
    elseif lastype==enum.CT_DDZ_ZHADAN_SIZHANG then
        return self:__CheckCanOutBomb(lastype, last_out.CompVal, hand_cards);
    elseif lastype==enum.CT_DDZ_HUOJIAN then
        return false;
    end

    return false;
end

function DdzCardtypes:__CheckCanOutBomb( last_type, last_compval, hand_cards )
   
    if last_type==enum.CT_DDZ_HUOJIAN or #hand_cards<2 then
        return false;
    end

    local group     = self:__GetCardValueGroup( hand_cards, 4 );
    local has_hj    = self:__HasHuoJian(hand_cards);
    
    if #group<1 and not has_hj then
        return false;
    end
    
    -- 如果上家出的是炸弹
    if last_type==enum.CT_DDZ_ZHADAN_SIZHANG and last_compval>0 then
        
        if group[1]>last_compval or has_hj then
            return true;
        end
        return false;
    end

    return true;
end

function DdzCardtypes:__GetSingleCards( last_compval, hand_cards )
    if #hand_cards >= 1 then
        if GetPokerLogicValue(hand_cards[1]) > last_compval then
            return true;    
        end
    end
    return self:__CheckCanOutBomb( 0, last_cards, hand_cards );
end

function DdzCardtypes:__CheckHaveLinkCards( last_compval, hand_cards )

    local hand_cnt = #hand_cards;

    -- 顺子至少五张牌
    if hand_cnt>=5 then
        
        local first_val = GetPokerLogicValue(hand_cards[1]);
        local group_1   = { first_val };
        
        for i=2,hand_cnt do
            local curr = GetPokerLogicValue(hand_cards[i]);
            if first_val ~= curr then
                table.insert( group_1, curr );
                first_val = curr;
            end
        end
        
        local max_val   = self:__CheckHaveLink( group_1, 5 );
        if max_val>last_compval then
            return true;
        end
    end
    return self:__CheckCanOutBomb( 0, last_cards, hand_cards );
end

function DdzCardtypes:__CheckHaveFeiJiNull( last_compval, hand_cards )
    if #hand_cards>=6 then
        local group_3   = self:__GetCardValueGroup( hand_cards, 3 );
        local max_val   = self:__CheckHaveLink( group_3, 2 );
        
        if max_val>last_compval then
            return true;
        end
    end
    return self:__CheckCanOutBomb( 0, last_cards, hand_cards );
end

function DdzCardtypes:__CheckHaveFeiJiOne( last_compval, hand_cards )
    if #hand_cards>=8 then
        local group_3   = self:__GetCardValueGroup( hand_cards, 3 );
        local max_val   = self:__CheckHaveLink( group_3, 2 );
        
        if max_val>last_compval then
            return true;
        end
    end
    return self:__CheckCanOutBomb( 0, last_cards, hand_cards );
end

function DdzCardtypes:__CheckHaveFeiJiDouble( last_compval, hand_cards )
    if #hand_cards>=10 then
        local group_3   = self:__GetCardValueGroup( hand_cards, 3 );
        local max_val   = self:__CheckHaveLink( group_3, 2 );
        
        if max_val>last_compval then
            local lk_group  = self:__GetLinkGroup( group_3, 2 );
            local group_2   = self:__GetCardValueGroup( hand_cards, 2 );
            local count     = 0;
            
            for _,gp in ipairs(lk_group) do     -- 遍历所有的 2连分组
                if gp[1]>last_compval then      -- 首个牌值大于上家的飞机
                    local double_cnt = 0;
                    
                    for _,v2 in ipairs(group_2) do
                        if gp[1]~=v2 and gp[2]~=v2 then
                            double_cnt = double_cnt+1;
                        end
                    end
                    
                    if double_cnt>=2 then
                        return true;
                    end
                end
            end
        end
    end
    return self:__CheckCanOutBomb( 0, last_cards, hand_cards );
end

function DdzCardtypes:__CheckHaveThreeOne( last_compval, hand_cards )
    if #hand_cards>=4 then
        local group_3 = self:__GetCardValueGroup( hand_cards, 3 );
        if #group_3>0 and group_3[1]>last_compval then
            return true;
        end
    end
    return self:__CheckCanOutBomb( 0, last_cards, hand_cards );
end

function DdzCardtypes:__CheckHaveThreeDouble( last_compval, hand_cards )
    if #hand_cards>=5 then
        local group_3 = self:__GetCardValueGroup( hand_cards, 3 );
        local group_2 = self:__GetCardValueGroup( hand_cards, 2 );
        
        if #group_3>0 and #group_2>0 then
            for _,v3 in ipairs(group_3) do
                if v3>last_compval then
                    for _,v2 in ipairs(group_2) do
                        if v3~=v2 then
                            return true;
                        end
                    end
                end
            end
        end
    end

    -- 如果没有大的，检查炸弹
    return self:__CheckCanOutBomb( 0, last_cards, hand_cards );
end

-- 检查四带二
function DdzCardtypes:__CheckHaveFourTwo( last_compval, hand_cards )
    if #hand_cards>=6 then
        local group_4 = self:__GetCardValueGroup( hand_cards, 4 );
        if #group_4>0 and group_4[1]>last_compval then
            return true;
        end
    end
    return self:__CheckCanOutBomb( 0, last_cards, hand_cards );
end

-- 检查四带两对
function DdzCardtypes:__CheckHaveFourDouble( last_compval, hand_cards )
    if #hand_cards>=8 then
        local group_4 = self:__GetCardValueGroup( hand_cards, 4 );
        local group_2 = self:__GetCardValueGroup( hand_cards, 2 );
        
        if #group_4>0 and #group_2>0 then
            for _,v4 in ipairs(group_4) do
                if v4>last_compval then
                    local double_cnt = 0;
                    for _,v2 in ipairs(group_2) do
                        if v4~=v2 then
                            double_cnt = double_cnt+1;
                        end
                    end
                    
                    if double_cnt>=2 then
                        return true;
                    end
                end
            end
        end
    end
    return self:__CheckCanOutBomb( 0, last_cards, hand_cards );
end

function DdzCardtypes:__CheckHaveCards( last_compval, hand_cards, same_cnt )
   
    if #hand_cards < same_cnt then
        return false;
    end
    
    local group = self:__GetCardValueGroup( hand_cards, same_cnt );
    
    if #group>0 and group[1]>last_compval then
        return true;
    end
    
    -- 如果没有大的，检查炸弹
    return self:__CheckCanOutBomb( 0, last_cards, hand_cards );
end

-- cards已从大到小排序,如果有顺子，返回顺子的最大点数，否则返回0.
function DdzCardtypes:__CheckHaveLink( cvs, link_num )
   
    if #cvs < link_num then
        return 0;
    end
    
    local cont      = 1;    -- 连续相同的张数统计
    local card_val  = cvs[1];
    local max_val   = card_val;
    
    for i=2,#cvs do
        local curr  = cvs[i];
        
        if curr+1==card_val then
            cont = cont + 1;
        end
        
        if cont==link_num then
            return max_val;
        end
        
        if curr+1~=card_val then
            cont    = 1;
            max_val = curr;
        end
        
        card_val = curr;
    end
    
    return 0;
end

-- 获得符合条件的组合  local cvs={ 5, 3, 2, 1 }  __GetLinkGroup(cvs, 2)   => { {3,2}, {2,1} }
-- card_vals 排序去重 后的list
function DdzCardtypes:__GetLinkGroup( card_vals, link_num )

    local res       = {};
    local vals_len  = #card_vals;

    if vals_len < link_num then
        return res;
    end


    local x = 1;
    local find = true;
    
    while find do
        local cont      = 1;    -- 连续相同的张数统计
        local card_val  = card_vals[x];
        find            = false;

        for i=x+1,vals_len do
            local curr  = card_vals[i];
            
            if curr+1==card_val then
                cont = cont + 1;
            end
            
            if cont==link_num then
                
                local group = {};
                for gi=i-link_num+1,i do
                    table.insert( group, card_vals[gi] );
                end
                table.insert( res, group );

                if i==vals_len then
                    return res;
                else
                    x = i - link_num + 2;
                    find = true;
                    break;
                end
            end
            
            if curr+1~=card_val then
                cont    = 1;
            end
            
            card_val = curr;
        end
    end

    return res;
end

-- tbl去重
function DdzCardtypes:__UniqueList(tbl)
    local res = {};
    
    if #tbl<1 then
        return res;
    end
    
    res[1] = tbl[1];
    local value = tbl[1];
    
    for i=2,#tbl do
        
        if value~=tbl[i]then
            table.insert(res, tbl[i])
        end
        
        value = tbl[i];
    end
    
    return res;
end

-- cards 已从大到小排序   统计cards相同点数的牌
-- local cards={4,4,4,4,5,5} __GetCardValueGroup( cards, 2 )  => {4,4,5}
-- local cards={4,4,4,4,5,5} __GetCardValueGroup( cards, 3 )  => {4}
function DdzCardtypes:__GetCardValueGroup( cards, same_cnt )
    
    local res_group = {};
    if #cards<1 then  return res_group;  end
    
    local cont      = 1;    -- 连续相同的张数统计
    local card      = GetPokerLogicValue(cards[1]);
    
    for i=2,#cards do
        local curr  = GetPokerLogicValue(cards[i]);
        
        if curr==card then
            cont = cont + 1;
        end
        
        if cont==same_cnt then
            cont = 0;
            table.insert(res_group, card)
        end
        
        if curr~=card then
            cont = 1;
            card = curr;
        end
    end
    
    return res_group;
end

function DdzCardtypes:__HasHuoJian( cards )
    if #cards<2 then  return false;  end
    
    local cont  = 0;    -- 大小王张数统计
    
    for i=1,#cards do
        if cards[i]>0x40 then
            cont = cont + 1;
            if cont == 2 then
                return true;
            end
        end
    end
    
    return false;
end



