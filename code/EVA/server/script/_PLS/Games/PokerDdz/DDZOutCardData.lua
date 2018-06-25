local DdzOutCardData = class("DdzOutCardData")

-- 构造函数;
function DdzOutCardData:ctor()
    self:ClearData();
end

function DdzOutCardData:ClearData()
    self.UID            = 0;
    self.Type           = enum.CT_DDZ_ERROR;
    self.Cards          = {};
    self.CompVal        = 0;
end

function DdzOutCardData:GetCardCount()
    return #self.Cards;
end

function DdzOutCardData:SetCards( cards )
    self.Cards = {};
    for i,v in ipairs(cards) do
        self.Cards[i] = v;
    end
end

function DdzOutCardData:Copy( out_data )
    self.UID    = out_data.UID;
    self.Type   = out_data.Type;
    self:SetCards( out_data.Cards );
end

return DdzOutCardData;
