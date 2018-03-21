stack = { }

function stack:create()
    local t = { }
    t._et = { }

    function t:push(...)
        if ... then
            local targs = { ...}
            for _, v in ipairs(targs) do
                table.insert(self._et, v)
            end
        end
    end

    function t:pop(num)
        local num = num or 1
        local entries = { }
        for i = 1, num do
            if #self._et ~= 0 then
                table.insert(entries, self._et[#self._et])
                table.remove(self._et)
            else
                break
            end
        end
        return unpack(entries)
    end

    function t:peek()
        if #self._et ~= 0 then
            return self._et[#self._et]
        end
        return nil
    end

    function t:clear()
        while #self._et ~= 0 do
            table.remove(self._et)
        end
    end

    function t:getn()
        return #self._et
    end

    function t:list()
        for i, v in pairs(self._et) do
            print(i, v)
        end
    end

    return t
end