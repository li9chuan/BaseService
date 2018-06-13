--- Hotfix helper which hotfixes modified modules.

local M = class("HotfixHelper")

local hotfix = require("Hotfix/hotfix")

-- global_objects which must not hotfix.
local global_objects = {
    arg,
    assert,
    bit32,
    collectgarbage,
    coroutine,
    debug,
    dofile,
    error,
    getmetatable,
    io,
    ipairs,
    load,
    loadfile,
    loadstring,
    math,
    module,
    next,
    os,
    package,
    pairs,
    pcall,
    print,
    rawequal,
    rawget,
    rawlen,
    rawset,
    require,
    select,
    setmetatable,
    string,
    table,
    tonumber,
    tostring,
    type,
    unpack,
    utf8,
    xpcall,
}

function M:Init()
    --hotfix.log_debug = function(s) print(s) end
    hotfix.log_debug = nlinfo
    hotfix.add_protect(global_objects)
    self.uptick = 0;
    -- Map file path to file time to detect modification.
    self.path_to_time = { };
end

--- Check modules and hotfix.
function M:Update( curr_tick )

    if curr_tick~=nil then
        if curr_tick - self.uptick < 2000 then
            return;
        end
        
        self.uptick = curr_tick;    
    end


    local MOD_NAME = "hotfix_module_names"
    if not package.searchpath(MOD_NAME, package.path) then return end
    package.loaded[MOD_NAME] = nil  -- always reload it
    local module_names = require(MOD_NAME)

    for _, module_name in pairs(module_names) do
        local path, err = package.searchpath(module_name, package.path)
        -- Skip non-exist module.
        if not path then
            nlwarning(string.format("No such module: %s. %s", module_name, err))
            goto continue
        end

        --local file_time = lfs.attributes (path, "modification")
        local file_time = Misc.GetFileModificationDate(path)
        if file_time == self.path_to_time[path] then goto continue end

        nlinfo(string.format("Hot fix module %s (%s)", module_name, path))
        self.path_to_time[path] = file_time
        hotfix.hotfix_module(module_name)
        ::continue::
    end  -- for
end  -- check()

return M
