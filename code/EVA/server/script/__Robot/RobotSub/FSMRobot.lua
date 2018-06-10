local FSMRobot = class("FSMRobot")

--[[
        基础逻辑状态机。
--]]

function FSMRobot:ctor()
    self._GameFSM 			= StateMachine:new();
    self._StateEnterTime    = 0;
    self._CurrState         = "TIdle";
    self.Robot              = nil;
end

function FSMRobot:Init( robot )
    self._GameFSM:setup_state({
        events = 
		{
            {name = "TIdle" 			        },
            {name = "TLogin" 			        },
        },
        callbacks =
		{
            onTIdle   		        = handler(self, self.DoIdle),
			onTLogin   		        = handler(self, self.DoLogin),
		}
    })
    
    self.Robot  = robot;
    self:SwitchState( self._CurrState );
end


function FSMRobot:__GetRunStateTime()
    return TimerMgr:GetTime() - self._StateEnterTime;
end

function FSMRobot:TickUpdate()
    self._GameFSM:do_event( self._CurrState, false );
end

function FSMRobot:SwitchState( event, ... )
    self._CurrState = event;
    self._StateEnterTime = TimerMgr:GetTime();
    self._GameFSM:do_event( event, true, ... );
end

function FSMRobot:GetState()
	return self._CurrState;
end

function FSMRobot:IsState( state )
    if self._CurrState == state then
        return true;
    end
	return false;
end

function FSMRobot:DoIdle( event )
    -- 不是第一帧，下一帧执行。
    if not event.args[1] then
        if not self.Robot:Connected() then
            self:SwitchState("TLogin");
        end
    end
end

function FSMRobot:DoLogin( event )
    
    --if event.args[1] then
        --nlinfo( "FSMClass:DoLogin SwitchState" );
    --else
        --nlinfo( "FSMClass:DoLogin TickUpdate" );
    --end
    if not event.args[1] then
        if self.Robot:Login() then
            self.Robot:StartGameTest();
            self:SwitchState("TIdle");
        end
    end
end



return FSMRobot;

