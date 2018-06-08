local FSMDdz = class("FSMDdz")

--[[
        游戏逻辑状态机。
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
    
    self._OldCreateRoomTime     = TimerMgr:GetTime();

    
    self.Robot      = robot;
    self.GameDdz    = robot.Game;
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
    -- 不是第一帧，下一帧执行。
    if not event.args[1] then

        if self.Robot.Game.RoomInfo == nil then
            local open_room = PublicRoomInfoMgr:GetOpenRoom("RM_DDZ");

            if open_room ~= nil  then
                -- 如果有其它robot创建的房间，加入。
                self:SwitchState("TJoinPrvRoom", open_room);
            else
                -- 没有公共的房间，创建一个。
                local create_time = math.random(5000,20000);
                if TimerMgr:GetTime() - self._OldCreateRoomTime > create_time then
                    self._OldCreateRoomTime = TimerMgr:GetTime();
                    self:SwitchState("TCreatePrvRoom");
                end
            end
        end
    end
end

function FSMDdz:DoCreatePrvRoom( event, open_room )
    
    if event.args[1] then
        self.GameDdz:DoCreatePrvRoom()
    else
        if self:__GetRunStateTime() > 15*1000 then
            self:SwitchState("TIdle");
        end
    end
end

function FSMDdz:DoJoinPrvRoom( event )
    
    -- 调用的这帧执行
    if event.args[1] then

        nlinfo("DoJoinPrvRoom");
        local open_room = event.args[2];

        PrintTable(open_room);
        self.GameDdz:DoJoinPrvRoom(open_room)

    else
        if self:__GetRunStateTime() > 15*1000 then
            self:SwitchState("TIdle");
        end
    end
end


return FSMDdz;

