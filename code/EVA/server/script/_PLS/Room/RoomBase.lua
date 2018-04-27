local RoomBase = class("RoomBase")

-- 构造函数;
function RoomBase:ctor( Data )

    self.RoomID                 = RoomMgr:GenerateRoomID();
    self.PrvRoomID              = 0;
    self.RoomType               = "";
    
    self.SeatPlayers            = {};
    self.ViewPlayers            = {};
end

-- 玩家加入房间
function RoomBase:JoinRoom( player )



end

-- 玩家离开房间
function RoomBase:LeaveRoom( uid )
    
	
    
end


function RoomBase:Init()
    print("RoomBase:Init");
	return self:OnInit();
end

-- 初始化，子类可重写
function RoomBase:OnInit()
    print("RoomBase:OnInit");
end

--释放函数
function RoomBase:Release()
    
end


--  广播消息给房间内桌上所有玩家  如有except_pid，那么广播给除except_pid的其它玩家。
function RoomBase:__BroadcastMsg( msg_name, msg_stru, except_pid )
    
    
    
end     
  
--  广播消息给房间内观战所有玩家  如有except_pid，那么广播给除except_pid的其它玩家。
function RoomBase:__BroadcastViewer( msg_name, msg_stru, except_pid )
    
    
    
end   

      
function RoomBase:__GetRoomPlayerNum()
    return #self.RoomPlayers;
end

function RoomBase:__GetViewPlayerNum()
    return #self.ViewPlayers;
end

function RoomBase:__IsRoomPlayer( playerid )
    if self.RoomPlayers[playerid]~=nil then
        return true;
    end
    return false;
end

function RoomBase:__IsViewPlayer( playerid )
    if self.ViewPlayers[playerid]~=nil then
        return true;
    end
    return false;
end

return RoomBase;
