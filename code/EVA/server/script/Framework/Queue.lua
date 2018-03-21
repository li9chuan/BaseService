
Queue = class("Queue")

-- 构造函数
function Queue:ctor()
	-- body
	-- 元素列表
	self._list = {};
end

-- 添加一个元素
function Queue:Push(value)
	-- body
	if value ~= nil then
		--todo
        local nPos = #self._list + 1;
		table.insert(self._list, nPos, value);
	end
end

-- 弹出一个函数
function Queue:Pop()
	-- body
	if self:Count() <= 0 then
		--todo
		return nil;
	end

	local nValue = self._list[1];
	table.remove(self._list, 1);

	return nValue;
end

-- 删除对象
function Queue:Remove(value)
	-- body
	for i=1,self:Count() do
		local nValue = self._list[i];
		if v == value then
			--todo
			table.remove(self._list, i);
			i = i - 1;
		end
	end
end

-- 数量
function Queue:Count()
	-- body
	return #self._list;
end

-- 清除
function Queue:Clear()
	-- body
	self._list={};
end

return Queue;