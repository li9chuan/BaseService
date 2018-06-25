local FSMDouDiZhu = class("FSMDouDiZhu")

--[[
        斗地主房间逻辑状态机。
--]]

function FSMDouDiZhu:ctor()
    self._GameFSM 			= StateMachine:new();
    self._StateEnterTime    = 0;
    self._CurrState         = "TDDZStateWait";
    self.RoomDdz            = nil;
end

function FSMDouDiZhu:Init( room_ddz )
    self._GameFSM:setup_state({
        events = 
		{
            {name = "TDDZStateWait" 			        },  -- 等待开始
			{name = "TDDZStateCheckStartGame"		    },  -- 检查是否可以开始游戏
			{name = "TDDZStateSelectMingCardStart" 	    },  -- 选择明牌开始阶段
			{name = "TDDZStateStartGame" 			    },  -- 开始游戏
            {name = "TDDZStateSendCard" 		    	},  -- 发送手牌
            {name = "TDDZStateQiangDiZhu" 		    	},  -- 抢地主阶段
            {name = "TDDZStateSelectAddTimes" 		    },  -- 选择加倍阶段
            {name = "TDDZStateAction" 		    	    },  -- 玩家自由活动
            {name = "TDDZStateOutCard" 		    	    },  -- 出牌状态
            {name = "TDDZStateShowDown" 		    	},  -- 游戏结算
            {name = "TDDZStateRelieveRoom" 		    	},  -- 解散房间
        },
        callbacks =
		{
			onTDDZStateWait   		        = handler(self, self.DoWait),
			onTDDZStateCheckStartGame  	    = handler(self, self.DoCheckStartGame),
            onTDDZStateSelectMingCardStart  = handler(self, self.DoSelectMingCardStart),
            onTDDZStateStartGame  	        = handler(self, self.DoStartGame),
            onTDDZStateSendCard  	        = handler(self, self.DoSendCard),
            onTDDZStateQiangDiZhu  	        = handler(self, self.DoQiangDiZhu),
            onTDDZStateSelectAddTimes  	    = handler(self, self.DoSelectAddTimes),
            onTDDZStateAction  	            = handler(self, self.DoAction),
            onTDDZStateOutCard  	        = handler(self, self.DoOutCard),
            onTDDZStateShowDown  	        = handler(self, self.DoShowDown),
            onTDDZStateRelieveRoom  	    = handler(self, self.DoRelieveRoom),
		}
    })
    
    self.RoomDdz = room_ddz;
    self:SwitchState( self._CurrState );
end


function FSMDouDiZhu:__GetRunStateTime()
    return TimerMgr:GetTime() - self._StateEnterTime;
end

function FSMDouDiZhu:TickUpdate()
    self._GameFSM:do_event( self._CurrState, false );
end

function FSMDouDiZhu:SwitchState( event, ... )
    self._CurrState = event;
    self._StateEnterTime = TimerMgr:GetTime();
    self._GameFSM:do_event( event, true, ... );
end

function FSMDouDiZhu:GetState()
	return self._CurrState;
end

function FSMDouDiZhu:IsState( state )
    if self._CurrState == state then
        return true;
    end
	return false;
end

function FSMDouDiZhu:DoWait( event )
    if not event.args[1] then
        -- 如果都点了普通开始，跳转检查距离等条件的状态。
        if self.RoomDdz:GameStartWait() then
            self:SwitchState("TDDZStateCheckStartGame");
        end
    end
end

-- 检查是否可以开始,暂不检查，直接跳下一状态
function FSMDouDiZhu:DoCheckStartGame( event )
    self:SwitchState("TDDZStateStartGame");
end

-- 有玩家选择了明牌开始
function FSMDouDiZhu:DoSelectMingCardStart( event )
    if event.args[1] then
        self.RoomDdz:ResetGameData();
        self.RoomDdz:BroadcastGameInfo();
        self.RoomDdz:SendQiangDiZhuWik();
    end
end

function FSMDouDiZhu:DoStartGame( event )
    if event.args[1] then
        self.RoomDdz.IsGameStart    = true;
        self.RoomDdz:ResetGameData();
        self.RoomDdz:BroadcastGameInfo();
    else
        
        if self:__GetRunStateTime()<3000 then
            return;
        end
        
        -- self:SwitchState("TDDZStateSelectMingCardStart");
        self:SwitchState("TDDZStateSendCard");
    end
end

function FSMDouDiZhu:DoSendCard( event )
    if event.args[1] then
        self.RoomDdz:SendHandCard();
        self.RoomDdz:BroadcastGameInfo();
    else
        if self:__GetRunStateTime()<3000 then
            return;
        end
        
        self:SwitchState("TDDZStateQiangDiZhu");
    end
end

function FSMDouDiZhu:DoQiangDiZhu( event )
    if event.args[1] then
        self.RoomDdz:SendQiangDiZhuWik();
    end
end

function FSMDouDiZhu:DoSelectAddTimes( event )
    --if self:__GetRunStateTime()<10000 then
    --    return;
    --end
end

function FSMDouDiZhu:DoAction( event )
    if event.args[1] then
        --self.RoomDdz:SendQiangDiZhuWik();
    end
end

function FSMDouDiZhu:DoOutCard( event )
    if self:__GetRunStateTime()>3000 then
        self:SwitchState("TDDZStateAction");
    end
end

function FSMDouDiZhu:DoShowDown( event )
    nlinfo( "FSMClass:DoShowDown" );
end

function FSMDouDiZhu:DoRelieveRoom( event )
    nlinfo( "FSMClass:DoRelieveRoom" );
end

return FSMDouDiZhu;

