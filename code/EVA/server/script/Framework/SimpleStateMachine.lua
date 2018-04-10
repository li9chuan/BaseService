-- region *.lua
-- Date
-- 此文件由[BabeLua]插件自动生成
--local class = require("MiddleClass")

local simple_state_machine = class("SimpleStateMachine")

simple_state_machine.VERSION = "2.2.0"
-- the event transitioned successfully from one state to another
simple_state_machine.SUCCEEDED = 1
-- the event was successfull but no state transition was necessary
simple_state_machine.NOTRANSITION = 2
-- the event was cancelled by the caller in a beforeEvent callback
simple_state_machine.CANCELLED = 3
-- the event is asynchronous and the caller is in control of when the transition occurs
simple_state_machine.PENDING = 4
-- the event was failure
simple_state_machine.FAILURE = 5
-- caller tried to fire an event that was innapropriate in the current state
simple_state_machine.INVALID_TRANSITION_ERROR = "INVALID_TRANSITION_ERROR"
-- caller tried to fire an event while an async transition was still pending
simple_state_machine.PENDING_TRANSITION_ERROR = "PENDING_TRANSITION_ERROR"
-- caller provided callback function threw an exception
simple_state_machine.INVALID_CALLBACK_ERROR = "INVALID_CALLBACK_ERROR"

simple_state_machine.WILDCARD = "*"
simple_state_machine.ASYNC = "ASYNC"

function simple_state_machine:initialize()
end

function simple_state_machine:setup_state(cfg)
    assert(type(cfg) == "table", "simple_state_machine:setup_state() - invalid config")
    
    -- cfg.initial allow for a simple string,
    -- or an table with { state = "foo", event = "setup", defer = true|false }
    if type(cfg.initial) == "string" then
        self._initial = {state = cfg.initial}
    else
        self._initial = cfg.initial
    end
    
    self._terminal = cfg.terminal or cfg.final
    self._events = cfg.events or {}
    self._callbacks = cfg.callbacks or {}
    self._map = {}
    self._current = "none"
    self._in_transition = false
    
    if self._initial then
        self._initial.event = self._initial.event or "startup"
        self:_add_event({name = self._initial.event, from = "none", to = self._initial.state})
    end
    
    for _, event in ipairs(self._events) do
        self:_add_event(event)
    end
    
    if self._initial and not self._initial.defer then
        self:do_event(self._initial.event)
    end
    
    return self._target
end

function simple_state_machine:is_ready()
    return self._current ~= "none"
end

function simple_state_machine:get_state()
    return self._current
end

function simple_state_machine:is_state(state)
    if type(state) == "table" then
        for _, s in ipairs(state) do
            if s == self._current then
                return true
            end
        end
        return false
    else
        return self._current == state
    end
end

function simple_state_machine:can_do_event(event_name)
    return not self._in_transition and
        (self._map[event_name][self._current] ~= nil or self._map[event_name][simple_state_machine.WILDCARD] ~= nil)
end

function simple_state_machine:cannot_do_event(event_name)
    return not self:can_do_event(event_name)
end

function simple_state_machine:is_finished_state()
    return self:is_state(self._terminal)
end

function simple_state_machine:do_event_force(name, ...)
    local from = self._current
    local map = self._map[name]
    local to = (map[from] or map[simple_state_machine.WILDCARD]) or from
    local args = {...}
    
    local event = {
        name = name,
        from = from,
        to = to,
        args = args
    }
    
    if self._in_transition then
        self._in_transition = false
    end
    
    self:_before_event(event)
    
    if from == to then
        self:_after_event(event)
        return simple_state_machine.NOTRANSITION
    end
    
    self._current = to
    self:_enter_state(event)
    self:_change_state(event)
    self:_after_event(event)
    return simple_state_machine.SUCCEEDED
end

function simple_state_machine:do_event(name, ...)
    assert(self._map[name] ~= nil, string.format("simple_state_machine:DoEvent() - invalid event %s", tostring(name)))
    
    local from = self._current
    local map = self._map[name]
    local to = (map[from] or map[simple_state_machine.WILDCARD]) or from
    local args = {...}
    
    local event = {
        name = name,
        from = from,
        to = to,
        args = args,
    }
    
    if self._in_transition then
        self:_on_error(event, simple_state_machine.PENDING_TRANSITION_ERROR,
            "event " .. name .. " inappropriate because previous transition did not complete")
        return simple_state_machine.FAILURE
    end
    
    if self:cannot_do_event(name) then
        self:_on_error(event, simple_state_machine.INVALID_TRANSITION_ERROR,
            "event " .. name .. " inappropriate in current state " .. self._current)
        return simple_state_machine.FAILURE
    end
    
    if self:_before_event(event) == false then
        return simple_state_machine.CANCELLED
    end
    
    if from == to then
        self:_after_event(event)
        return simple_state_machine.NOTRANSITION
    end
    
    event.transition = function()
        self._in_transition = false
        self._current = to
        -- this method should only ever be called once
        self:_enter_state(event)
        self:_change_state(event)
        self:_after_event(event)
        return simple_state_machine.SUCCEEDED
    end
    
    event.cancel = function()
            -- provide a way for caller to cancel async transition if desired
            event.transition = nil
            self:_after_event(event)
    end
    
    self._in_transition = true
    local leave = self:_leave_state(event)
    if leave == false then
        event.transition = nil
        event.cancel = nil
        self._in_transition = false
        return simple_state_machine.CANCELLED
    elseif string.upper(tostring(leave)) == simple_state_machine.ASYNC then
        return simple_state_machine.PENDING
    else
        -- need to check in case user manually called transition()
        -- but forgot to return StateMachine.ASYNC
        if event.transition then
            return event.transition()
        else
            self._in_transition = false
        end
    end
end

function simple_state_machine:_add_event(event)
    local from = {}
    if type(event.from) == "table" then
        for _, name in ipairs(event.from) do
            from[name] = true
        end
    elseif event.from then
        from[event.from] = true
    else
        -- allow "wildcard" transition if "from" is not specified
        from[simple_state_machine.WILDCARD] = true
    end
    
    self._map[event.name] = self._map[event.name] or {}
    local map = self._map[event.name]
    for fromName, _ in pairs(from) do
        map[fromName] = event.to or fromName
    end
end

local function _do_callback(callback, event)
    if callback then
        return callback(event)
    end
end

function simple_state_machine:_before_any_event(event)
    return _do_callback(self._callbacks["onbeforeevent"], event)
end

function simple_state_machine:_after_any_event(event)
    return _do_callback(self._callbacks["onafterevent"], event)
end

function simple_state_machine:_leave_any_state(event)
    return _do_callback(self._callbacks["onleavestate"], event)
end

function simple_state_machine:_enter_any_state(event)
    return _do_callback(self._callbacks["onenterstate"] or self._callbacks["onstate"], event)
end

function simple_state_machine:_change_state(event)
    return _do_callback(self._callbacks["onchangestate"], event)
end

function simple_state_machine:_before_this_event(event)
    return _do_callback(self._callbacks["onbefore" .. event.name], event)
end

function simple_state_machine:_after_this_event(event)
    return _do_callback(self._callbacks["onafter" .. event.name] or self._callbacks["on" .. event.name], event)
end

function simple_state_machine:_leave_this_state(event)
    return _do_callback(self._callbacks["onleave" .. event.from], event)
end

function simple_state_machine:_enter_this_state(event)
    return _do_callback(self._callbacks["onenter" .. event.to] or self._callbacks["on" .. event.to], event)
end

function simple_state_machine:_before_event(event)
    if self:_before_this_event(event) == false or self:_before_any_event(event) == false then
        return false
    end
end

function simple_state_machine:_after_event(event)
    self:_after_this_event(event)
    self:_after_any_event(event)
end

function simple_state_machine:_leave_state(event, transition)
    local specific = self:_leave_this_state(event, transition)
    local general = self:_leave_any_state(event, transition)
    if specific == false or general == false then
        return false
    elseif string.upper(tostring(specific)) == simple_state_machine.ASYNC
        or string.upper(tostring(general)) == simple_state_machine.ASYNC then
        return simple_state_machine.ASYNC
    end
end

function simple_state_machine:_enter_state(event)
    self:_enter_this_state(event)
    self:_enter_any_state(event)
end

function simple_state_machine:_on_error(event, error, message)
    print("%s [simple_state_machine] ERROR: error %s, event %s, from %s to %s", tostring(self._target), tostring(error), event.name, event.from, event.to)
    print(message)
end

return simple_state_machine