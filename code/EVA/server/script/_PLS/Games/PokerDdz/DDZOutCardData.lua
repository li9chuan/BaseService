local DdzOutCardData = class("DdzOutCardData")

-- 构造函数;
function DdzOutCardData:ctor()
    self:ClearData();
end

function DdzOutCardData:ClearData()
    self.UID            = 0;
    self.Type           = 0;
    self.Cards          = {};
end

function DdzOutCardData:IsEmpty()
    if self.UID==0 or self.Type==0 then
        return true;
    end
    return false;
end


return DdzOutCardData;
