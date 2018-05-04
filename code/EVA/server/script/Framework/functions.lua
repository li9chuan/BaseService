

--[[
实际上，除了 C++ 回调 Lua 函数之外，在其他所有需要回调的地方都可以使用 handler()。

@param mixed obj Lua 对象
@param function method 对象方法

@return function

]]--

function handler(obj, method)
	if (nil == obj or nil == method) then
		logError("handler param == nil");
		return nil;
	end
    return function(...)
        return method(obj, ...)
    end
end

function checktable(value)
    if type(value) ~= "table" then value = {} end
    return value
end

function md5(value)
    return Misc.MD5( value, string.len(value) );
end

function nldebug( str )
	Debug.Debug(str,2);
end

function nlinfo( str )
	Debug.Info(str,2);
end

function nlwarning( str )
	Debug.Warning(str,2);
end

function nlstop( str )
	Debug.Stop(str,2);
end

print   = nlinfo;

function enum(enum_type, enum_name)
	return protobuf.enum_id(enum_type, enum_name);
end


function shuffle(tbl)
    
    local tbl_count = #tbl;
    for i=1,tbl_count do
        local ridx  = math.random(1, tbl_count);
        if i~=ridx then
            local temp  = tbl[i];
            tbl[i]      = tbl[ridx];
            tbl[ridx]   = temp;
        end
    end
end

function SetBit( val, enum_type, enum_name )
    local enum_val = protobuf.enum_id(enum_type, enum_name);
    Misc.SetBit(val, enum_val);
    return val;
end
    
function GetServiceID()
	return ServerNet.GetServiceID();
end

function PostSub( thread_name, event_type, pb_name, pb_data, from )
	
	code = protobuf.encode(pb_name, pb_data)
	len  = string.len(code);
	
	msg = { thread_name, from, event_type, len };
	LuaThread.PostSub(code, msg);

end

function PostMain( thread_name, event_type, pb_name, pb_data, from )
	
	code = protobuf.encode(pb_name, pb_data)
	len  = string.len(code);
	
	msg = { thread_name, from, event_type, len };
	LuaThread.PostMain(code, msg);

end

function PrintTable( tbl, indent, depth )
    Debug.Info(JsonUtil.serialise_value(tbl, indent, depth), 2);
    --[[
    if tbl==nil then
        return;
    end
    
    local msg = ""
    depth = depth or 1
    local indent_str = ""
    
    for i = 2, depth do
        indent_str = indent_str.."    "
    end

    print(indent_str .. "{")
    for k,v in pairs(tbl) do
        if k ~= "class" then
            local item_str = string.format("%s%s = %s", indent_str .. " ",tostring(k), tostring(v))
            print(item_str)
            
            if type(v) == "table" then
                PrintTable(v, depth + 1)
            end
        end
    end
    print(indent_str .. "}")
    
    --]]
end

function Table2Json( tbl )
    return JsonUtil.serialise_value(tbl);
end

function Json2Table( str )
    return Json.decode(str);
end

function PB2Table( pb_stru, pb_data )
    return protobuf.decode(pb_stru, pb_data);
end

-- start --

--------------------------------
-- 从表格中删除指定值，返回删除的值的个数
-- @function [parent=#table] removebyvalue
-- @param table array 表格
-- @param mixed value 要删除的值
-- @param boolean removeall 是否删除所有相同的值
-- @return integer#integer 

--[[--

从表格中删除指定值，返回删除的值的个数

~~~ lua

local array = {"a", "b", "c", "c"}
print(table.removebyvalue(array, "c", true)) -- 输出 2

~~~

]]

-- end --
function table.removebyvalue(array, value, removeall)
    local c, i, max = 0, 1, #array
    while i <= max do
        if array[i] == value then
            table.remove(array, i)
            c = c + 1
            i = i - 1
            max = max - 1
            if not removeall then break end
        end
        i = i + 1
    end
    return c
end

function get_data_by_sec(sec)
    sec = sec < 0 and 0 or sec
    local data =
    {
        day = math.floor(sec / 3600 / 24),
        hour = math.floor(sec / 3600) % 24,
        min = math.floor(sec % 3600 / 60),
        sec = sec % 60,
    }
    return data
end

-- 单例模式
function singleton(classname, super)
    local cls = {}
    if super then
        for k,v in pairs(super) do cls[k] = v end
        cls.super = super
    else
        cls.ctor = function() end
    end

    cls.__cname = classname
    cls.__index = cls

    local Instance = setmetatable({class = cls}, cls)
    function cls.Instance()
        return Instance
    end
    return cls
end

-- 分割字符串
function SplitStr(str, reps)
	local resultStrList = {}
    string.gsub(str,'[^'..reps..']+',function ( w )
        table.insert(resultStrList,w)
    end)
    return resultStrList
end

-- 获取随机数
function GetRandom(min, max)
   math.randomseed(tostring(os.time()):reverse():sub(1, 7)) 
   return math.random(min, max);
end

-- 字符串加密
function urlEncode(s)  
     s = string.gsub(s, "([^%w%.%- ])", function(c) return string.format("%%%02X", string.byte(c)) end)  
    return string.gsub(s, " ", "+")  
end  
  
-- 解密
function urlDecode(s)  
    s = string.gsub(s, '%%(%x%x)', function(h) return string.char(tonumber(h, 16)) end)  
    return s  
end 

-- 检查表中是否存在
function IsInTable(value, tbl)
	if nil == tbl then return false; end
	for k,v in pairs(tbl) do
		if v == value then
			return true;
		end
	end
	return false;
end

function ReverseTable(tab)  
    local tmp = {}  
    for i = 1, #tab do  
        local key = #tab  
        tmp[i] = table.remove(tab)  
    end  
  
    return tmp  
end