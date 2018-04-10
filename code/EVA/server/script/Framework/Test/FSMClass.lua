local FSMClass = class("FSMClass")

-- 构造函数;
function FSMClass:ctor()
    
    self._GameFSM 			= StateMachine:new();

    self._GameFSM:setup_state({
        events = 
		{
            {name = "TChessStateWait" 			        }, -- 等待游戏开始
			{name = "TChessStateStartGame"		 		}, -- 开始游戏
			{name = "TChessStateAction" 			    }, -- 玩家自由出牌状态
			{name = "TChessStateShowDown" 			    }, -- 玩家结算状态
            {name = "TChessStateLeaveRoom" 		    	}, -- 离开
        },
        callbacks =
		{
			onTChessStateWait   		= handler(self, self.DoGameFSMWait),
			onTChessStateStartGame  	= handler(self, self.DoGameFSMStartGame),
		}
    })
end
function FSMClass:SwitchState( event, ... )
	if (self._GameFSM ~= nil) then
		self._GameFSM:do_event( event, ... );
	end
end

function FSMClass:DoGameFSMWait( event )
    print( "FSMClass:DoGameFSMWait" .. event.args[1] );
end

function FSMClass:DoGameFSMStartGame( event )
    print( "FSMClass:DoGameFSMStartGame".. event.args[1] );
end

return FSMClass;