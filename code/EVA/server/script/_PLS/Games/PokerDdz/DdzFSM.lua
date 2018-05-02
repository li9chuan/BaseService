local FSMDouDiZhu = class("FSMDouDiZhu")

-- 构造函数;
function FSMDouDiZhu:ctor()
    
    self._GameFSM 			= StateMachine:new();

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
end

function FSMDouDiZhu:TickUpdate()
    self._GameFSM:do_event( self._GameFSM:get_state() );
end

function FSMDouDiZhu:SwitchState( event, ... )
    self._GameFSM:do_event( event, ... );
end

function FSMDouDiZhu:DoWait( event )
    print( "FSMClass:DoWait" .. event.args[1] );
end

function FSMDouDiZhu:DoCheckStartGame( event )
    print( "FSMClass:DoCheckStartGame".. event.args[1] );
end

function FSMDouDiZhu:DoSelectMingCardStart( event )
    print( "FSMClass:DoSelectMingCardStart".. event.args[1] );
end

function FSMDouDiZhu:DoStartGame( event )
    print( "FSMClass:DoStartGame".. event.args[1] );
end

function FSMDouDiZhu:DoSendCard( event )
    print( "FSMClass:DoSendCard".. event.args[1] );
end

function FSMDouDiZhu:DoQiangDiZhu( event )
    print( "FSMClass:DoQiangDiZhu".. event.args[1] );
end

function FSMDouDiZhu:DoSelectAddTimes( event )
    print( "FSMClass:DoSelectAddTimes".. event.args[1] );
end

function FSMDouDiZhu:DoAction( event )
    print( "FSMClass:DoAction".. event.args[1] );
end

function FSMDouDiZhu:DoOutCard( event )
    print( "FSMClass:DoOutCard".. event.args[1] );
end

function FSMDouDiZhu:DoShowDown( event )
    print( "FSMClass:DoShowDown".. event.args[1] );
end

function FSMDouDiZhu:DoRelieveRoom( event )
    print( "FSMClass:DoRelieveRoom".. event.args[1] );
end

return FSMDouDiZhu;

