MapMap = class("MapMap")

local this = MapMap

function this:ctor()
	self._map = {};
    self._count = 0
end

-- MapMap 插入新值
function this:Insert(k1, k2, v)
    if nil == self._map[k1] then
        local maptmp = Map:new();
        maptmp:Insert(k2,v);
        
        self._map[k1] = maptmp;
        self._count = self._count + 1
    else
        self._map[k1]:Insert(k2,v);
    end
end

-- MapMap 插入新值并且切换旧值
function this:Replace(k1,k2,v)
	if nil == self._map[k1] then
        local maptmp = Map:new();
        maptmp:Insert(k2,v);
        
        self._map[k1] = maptmp;
		self._count = self._count + 1;
	else
		self._map[k1]:Replace(k2,v);
	end
end

function this:Remove(k1, k2)
    if nil ~= self._map[k1] then
        if k2==nil then
            self._map[k1] = nil
            self._count = self._count - 1
        else
            self._map[k1]:Remove(k2);
        end
    end
end

function this:Find(k1, k2)
    local value = nil
    if nil ~= self._map[k1] then
        value = self._map[k1]:Find(k2);
    end
    return value
end

function this:Clear()
    for k,_ in pairs(self.List) do
        if nil ~= self.List[k] then
            self.List[k] = nil
        end
    end
    self._count = 0
end


-- 遍历所有成员
function this:ForEach(fun, ...)
	-- body
	for k,v in pairs(self._map) do
		fun(k, v, ...)
	end
end

-- MapMap 获取字典的count
function this:Count()
	return self._count;
end

return Map;


--local characters = Map:new()
--characters:Insert("name1"," this Name:123")
--characters:Replace("name1"," this Name:2" )
--local name2 = characters:Find("name1")
--print(name2)
--print(characters._count)
--for k,v in pairs(characters) do
--print(k,v)
--end