local _M = {}

function _M.bind(func, ...)
	local args = {...}
	return function(...)
		func(table.unpack(args), ...)
	end
end

function _M.dump(value, dep)
	dep = dep or ""
	local ret = ""
	if type(value) == "table" then
		ret = ret .. "{\n"
		for k, v in pairs(value) do
			ret = string.format("%s%s\t[%s] = %s\n", ret, dep, k, dump(v, dep .. "\t"))
		end
		ret = ret .. dep .. "},\n"
	else
		ret = ret .. tostring(value) .. ", "
	end
	return ret
end

function _M.clone(src)
	local ret = {}
	if type(src) == "table" then
		for k, v in pairs(src) do
			ret[k] = _M.clone(v)
		end
	else
		ret = src
	end
	return ret
end

function _M.tinsert_n(src, val, n)
	for i = 1, n do
		table.insert(src, _M.clone(val))
	end
end

function _M.ms2t(cycle)
	local s = math.floor(cycle / 1000)
	local m = math.floor(cycle / 60000)
	local h = math.floor(cycle / 3600000)
	local ms = cycle - h * 3600000 - m * 60000 - s * 1000
	return math.floor(h % 24), math.floor(m % 60), math.floor(s % 60), math.floor(ms % 1000)
end

function _M.t2ms(h, m, s, ms)
	return h * 3600000 + m * 60000 + s * 1000 + ms
end

return _M