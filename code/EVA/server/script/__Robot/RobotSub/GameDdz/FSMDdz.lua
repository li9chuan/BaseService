local FSMDdz = class("FSMDdz")

--[[
        ÓÎÏ·Âß¼­×´Ì¬»ú¡£
--]]

function FSMDdz:ctor()
    self._GameFSM 			= StateMachine:new();
    self._StateEnterTime    = 0;
    self._CurrState         = "TIdle";
    self.Robot              = nil;
end

function FSMDdz:Init( robot )
    self._GameFSM:setup_state({
        events = 
		{
            {name = "TIdle" 			        },
			{name = "TCreatePrvRoom"		    },
			{name = "TJoinPrvRoom" 	            },

        },
        callbacks =
		{
            onTIdle   		        = handler(self, self.DoIdle),
			onTCreatePrvRoom  	    = handler(self, self.DoCreatePrvRoom),
            onTJoinPrvRoom          = handler(self, self.DoJoinPrvRoom),
		}
    })
    
    self.Robot      = robot;
    self.GameDdz    = robot.GameLoigc;
    self:SwitchState( self._CurrState );


end


function FSMDdz:__GetRunStateTime()
    return TimerMgr:GetTime() - self._StateEnterTime;
end

function FSMDdz:TickUpdate()
    self._GameFSM:do_event( self._CurrState, false );
end

function FSMDdz:SwitchState( event, ... )
    self._CurrState = event;
    self._StateEnterTime = TimerMgr:GetTime();
    self._GameFSM:do_event( event, true, ... );
end

function FSMDdz:GetState()
	return self._CurrState;
end

function FSMDdz:IsState( state )
    if self._CurrState == state then
        return true;
    end
	return false;
end

function FSMDdz:DoIdle( event )
    nlinfo("FSMDdz DoIdle");
end

function FSMDdz:DoCreatePrvRoom( event )
    nlinfo("DoCreatePrvRoom");
    --self:SwitchState("TDDZStateStartGame");
end

function FSMDdz:DoJoinPrvRoom( event )
    nlinfo("DoJoinPrvRoom");
    --self:SwitchState("TDDZStateStartGame");
end


return FSMDdz;

