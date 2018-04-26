local RoomIDAlloter = class("RoomIDAlloter")

-- 构造函数;
function RoomIDAlloter:ctor()
	self.IDPool         = {};
    self.RoomType       = "";
    self.IDMax          = 999999;
    self.__AllocIdx     = 1;

    self.Init();
end



function RoomIDAlloter:Init()
    
    for i=1,self.IDMax do
        self.IDPool[i] = i;
    end
    
    for i=1,self.IDMax do
        local ridx  = math.random(1, self.IDMax);
        if i~=ridx then
            local temp          = self.IDPool[i];
            self.IDPool[i]      = self.IDPool[ridx];
            self.IDPool[ridx]   = temp;
        end
    end
    
end

function RoomIDAlloter:AllocID()
    
    if self.__AllocIdx == self.IDMax then
        self.__AllocIdx = 1;
    end
    
    return self.IDPool[__AllocIdx];
end

return RoomIDAlloter;


