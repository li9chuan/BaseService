-- # Example
-- ``` lua
-- local class    = require 'middleclass'
-- local Stateful = require 'stateful'
-- local Enemy = class('Enemy')
-- Enemy:include(Stateful)
-- function Enemy:initialize(health)
--   self.health = health
-- end
-- function Enemy:speak()
--   return 'My health is' .. tostring(self.health)
-- end
-- local Immortal = Enemy:addState('Immortal')
-- -- overriden function
-- function Immortal:speak()
--   return 'I am UNBREAKABLE!!'
-- end
-- -- added function
-- function Immortal:die()
--   return 'I can not die now!'
-- end
-- local peter = Enemy:new(10)
-- peter:speak() -- My health is 10
-- peter:gotoState('Immortal')
-- peter:speak() -- I am UNBREAKABLE!!
-- peter:die() -- I can not die now!
-- peter:gotoState(nil)
-- peter:speak() -- My health is 10
-- ```
local Stateful = {static = {}}

local _callbacks = {
    enteredState = 1,
    exitedState = 1,
    pushedState = 1,
    poppedState = 1,
    pausedState = 1,
    continuedState = 1
}

local _BaseState = {}

local function _addStatesToClass(klass, superStates)
    klass.static.states = {}
    for stateName, state in pairs(superStates or {}) do
        klass:addState(stateName, state)
    end
end

local function _getStatefulMethod(instance, name)
    if not _callbacks[name] then
        local stack = rawget(instance, '__stateStack')
        if not stack then return end
        for i = #stack, 1, -1 do
            if stack[i][name] then return stack[i][name] end
        end
    end
end

local function _getNewInstanceIndex(prevIndex)
    if type(prevIndex) == 'function' then
        return function(instance, name) return _getStatefulMethod(instance, name) or prevIndex(instance, name) end
    end
    return function(instance, name) return _getStatefulMethod(instance, name) or prevIndex[name] end
end

local function _getNewAllocateMethod(oldAllocateMethod)
    return function(klass, ...)
        local instance = oldAllocateMethod(klass, ...)
        instance.__stateStack = {}
        return instance
    end
end

local function _modifyInstanceIndex(klass)
    klass.__instanceDict.__index = _getNewInstanceIndex(klass.__instanceDict.__index)
end

local function _getNewSubclassMethod(prevSubclass)
    return function(klass, name)
        local subclass = prevSubclass(klass, name)
        _addStatesToClass(subclass, klass.states)
        _modifyInstanceIndex(subclass)
        return subclass
    end
end

local function _modifySubclassMethod(klass)
    klass.static.subclass = _getNewSubclassMethod(klass.static.subclass)
end

local function _modifyAllocateMethod(klass)
    klass.static.allocate = _getNewAllocateMethod(klass.static.allocate)
end

local function _assertType(val, name, expected_type, type_to_s)
    assert(type(val) == expected_type, "Expected " .. name .. " to be of type " .. (type_to_s or expected_type) .. ". Was " .. tostring(val) .. "(" .. type(val) .. ")")
end

local function _assertInexistingState(klass, stateName)
    assert(klass.states[stateName] == nil, "State " .. tostring(stateName) .. " already exists on " .. tostring(klass))
end

local function _assertExistingState(self, state, stateName)
    assert(state, "The state " .. stateName .. " was not found in " .. tostring(self.class))
end

local function _invokeCallback(self, state, callbackName, ...)
    if state and state[callbackName] then state[callbackName](self, ...) end
end

local function _getCurrentState(self)
    return self.__stateStack[#self.__stateStack]
end

local function _getStateFromClassByName(self, stateName)
    local state = self.class.static.states[stateName]
    _assertExistingState(self, state, stateName)
    return state
end

local function _getStateIndexFromStackByName(self, stateName)
    if stateName == nil then return #self.__stateStack end
    local target = _getStateFromClassByName(self, stateName)
    for i = #self.__stateStack, 1, -1 do
        if self.__stateStack[i] == target then return i end
    end
end

local function _getStateName(self, target)
    for name, state in pairs(self.class.static.states) do
        if state == target then return name end
    end
end

function Stateful:included(klass)
    _addStatesToClass(klass)
    _modifyInstanceIndex(klass)
    _modifySubclassMethod(klass)
    _modifyAllocateMethod(klass)
end

function Stateful.static:addState(stateName, superState)
    superState = superState or _BaseState
    _assertType(stateName, 'stateName', 'string')
    _assertInexistingState(self, stateName)
    self.static.states[stateName] = setmetatable({}, {__index = superState})
    return self.static.states[stateName]
end

function Stateful:gotoState(stateName, ...)
    
    self:popAllStates()
    
    if stateName == nil then
        self.__stateStack = {}
    else
        _assertType(stateName, 'stateName', 'string', 'string or nil')
        
        local newState = _getStateFromClassByName(self, stateName)
        self.__stateStack = {newState}
        _invokeCallback(self, newState, 'enteredState', ...)
    end

end

function Stateful:pushState(stateName)
    local oldState = _getCurrentState(self)
    _invokeCallback(self, oldState, 'pausedState')
    
    local newState = _getStateFromClassByName(self, stateName)
    table.insert(self.__stateStack, newState)
    
    _invokeCallback(self, newState, 'pushedState')
    _invokeCallback(self, newState, 'enteredState')
end

function Stateful:popState(stateName)
    
    local oldStateIndex = _getStateIndexFromStackByName(self, stateName)
    local oldState
    if oldStateIndex then
        oldState = self.__stateStack[oldStateIndex]
        
        _invokeCallback(self, oldState, 'poppedState')
        _invokeCallback(self, oldState, 'exitedState')
        
        table.remove(self.__stateStack, oldStateIndex)
    end
    
    local newState = _getCurrentState(self)
    
    if oldState ~= newState then
        _invokeCallback(self, newState, 'continuedState')
    end
end

function Stateful:popAllStates()
    local size = #self.__stateStack
    for i = 1, size do self:popState() end
end

function Stateful:getStateStackDebugInfo()
    local info, state = {}, nil
    for i = #self.__stateStack, 1, -1 do
        state = self.__stateStack[i]
        table.insert(info, _getStateName(self, state))
    end
    return info
end

return Stateful
