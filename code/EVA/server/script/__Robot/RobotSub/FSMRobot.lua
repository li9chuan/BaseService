local FSMRobot = class("FSMRobot")

--[[
        »ù´¡Âß¼­×´Ì¬»ú¡£
--]]

function FSMRobot:ctor()
    self._GameFSM 			= StateMachine:new();
    self._StateEnterTime    = 0;
    self._CurrState         = "TIdle";
    self.RobotData          = nil;
end

function FSMRobot:Init( robot_data )
    self._GameFSM:setup_state({
        events = 
		{
            {name = "TIdle" 			        },
            {name = "TLogin" 			        },
			{name = "TCreatePrvRoom"		        },
			{name = "TJoinPrvRoom" 	            },

        },
        callbacks =
		{
            onTIdle   		        = handler(self, self.DoIdle),
			onTLogin   		        = handler(self, self.DoLogin),
			onTCreatePrvRoom  	    = handler(self, self.DoCreatePrvRoom),
            onTJoinPrvRoom          = handler(self, self.DoJoinPrvRoom),
            

		}
    })
    
    self.RobotData = robot_data;
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
    nlinfo("DoIdle");
end

function FSMRobot:DoLogin( event )
    
    nlinfo("DoLogin");
    if event.args[1] then
        --print( "FSMClass:DoLogin SwitchState" );
    else
        --print( "FSMClass:DoLogin TickUpdate" );
    end
    
end

function FSMRobot:DoCreatePrvRoom( event )
    nlinfo("DoCreatePrvRoom");
    --self:SwitchState("TDDZStateStartGame");
end

function FSMRobot:DoJoinPrvRoom( event )
    nlinfo("DoJoinPrvRoom");
    --self:SwitchState("TDDZStateStartGame");
end


return FSMRobot;

