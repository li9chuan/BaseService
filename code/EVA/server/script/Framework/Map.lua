
Map = class("Map")

local this = Map

function this:ctor()
	self.List = {};
    self.count = 0
end

-- 插入新值
function this:Insert(k,v)
    if nil == self.List[k] then
        self.List[k] = v
        self.count = self.count + 1
    end
end

-- 插入新值并且切换旧值
function this:Instead(k,v)
	if nil == self.List[k] then
		self.List[k] = v;
		self.count = self.count + 1;
	else
		self.List[k] = v;
	end
end

function this:Remove(k)
    if nil ~= self.List[k] then
        self.List[k] = nil
        if self.count >0 then
            self.count = self.count - 1
        end
    end
end

function this:GetPair(k)
    local value = nil
    if nil ~= self.List[k] then
        value = self.List[k]
    end
    return value
end

function this:Clear()
    for k,_ in pairs(self.List) do
        if nil ~= self.List[k] then
            self.List[k] = nil
        end
    end
    self.count = 0
end


-- 遍历所有成员
function this:ForEach(fun, ...)
	-- body
	for k,v in pairs(self.List) do
		fun(k, v, ...)
	end
end

-- 获取字典的count
function this:Count()
	return self.count;
end

return Map;


--local characters = Map:new()
--characters:insert("name1"," this Name:123")
--characters:instead("name1"," this Name:2" )
--local name2 = characters:getpair("name1")
--print(name2)
--print(characters.count)
--for k,v in pairs(characters) do
--print(k,v)
--end