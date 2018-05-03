local RoomDdz = class("RoomDdz",RoomBase)

DdzFSM              = require("Games/PokerDdz/DdzFSM")
DdzPlayerInfo       = require("Games/PokerDdz/DdzPlayerInfo")
DDZOutCardData      = require("Games/PokerDdz/DDZOutCardData")

-- 构造函数;
function RoomDdz:ctor()

    self.super:ctor();
    print("RoomDdz:ctor");

    self.Fsm                    = DdzFSM:new();
    self.RoomPlayerData         = {};
    
	
	self._CardsBottom           = {}; 		-- 剩下的三张底牌
    self._ActionID              = 0;        -- 当前活动的玩家
    self._GameCount             = 1;        -- 当前是第几局
    self._Multiple              = 1;        -- 房间翻倍数
    
    self._LastOutCardData       = DDZOutCardData:new();
    
    self.CardsPool              = {  };

    self:__InitData();
end



-- 玩家加入房间
function RoomDdz:JoinRoom( player )


    local ddz_player = DdzPlayerInfo:new();

    ddz_player:SetState( "PB.TDDZPlayerState", "STATE_DDZ_NEWROLE" );
    
    self.RoomPlayerData[player.UID] = ddz_player;
    
    self:BaseJoinRoom(player);
    
    self:BroadcastGameInfo();

end




function RoomDdz:TickUpdate()

    --print("RoomDdz:TickUpdate");

    self.Fsm:TickUpdate();
    
    self._TimerHandle = TimerMgr:AddTimer(self._TimerTick, self, self.TickUpdate);
end


function RoomDdz:UserStartReady( uid )
    local room_player = self.RoomPlayerData[uid];
    
    if room_player~=nil then
        room_player:SetReady();
        local msg_int = { value=uid };
        self:BroadcastMsg( "DDZ_SR", "PB.MsgInt", msg_int );
    end
end

function RoomDdz:UserCancelReady( uid )
    local room_player = self.RoomPlayerData[uid];
    
    if room_player~=nil then
        room_player:CancleReady();
        local msg_int = { value=uid };
        self:BroadcastMsg( "DDZ_CR", "PB.MsgInt", msg_int );
    end
end

function RoomDdz:BroadcastGameInfo( )

    for k,v in pairs(self.SeatPlayers) do
        if v~=0 then
            local msg_gameinfo = {};
            self:SendGameInfo( v, "DDZ_GI", msg_gameinfo );
        end
    end

end

function RoomDdz:SendGameInfo( uid, msg_name, msg_ddz_room )

    msg_ddz_room.room_id = self.PrvRoomID;
    self:__FillRoomInfoMsg(msg_ddz_room, uid);

	local player = PlayerMgr:GetPlayer(uid);
    BaseService:SendToClient( player, msg_name, "PB.MsgDDZRoom", msg_ddz_room )
end

function RoomDdz:__FillRoomInfoMsg( msg_ddz_room, current_uid )
    
    
    msg_ddz_room.room_id        = self.PrvRoomID;

	msg_ddz_room.room_state     = enum("PB.TDDZState", self.Fsm:GetState());
    msg_ddz_room.action_id      = self._ActionID;
    msg_ddz_room.game_count     = self._GameCount;
    msg_ddz_room.multiple       = self._Multiple;


    msg_ddz_room.bottom_card    = self._CardsBottom;


    if self._LastOutCardData:IsEmpty()==false then
        
        msg_ddz_room.last_outcard = {
            old_actionid = self._LastOutCardData.UID,
            out_type     = self._LastOutCardData.Kind,
            out_card     = self._LastOutCardData.Cards
        };
        
        
    end
    
    msg_ddz_room.private_room = { room_type=self.RoomType };


    for _,v in pairs(self.SeatPlayers) do
        if v>0 then
            self:__FillPlayerBaseInfoMsg( v, msg_ddz_room, current_uid );
        end
    end
end


function RoomDdz:__FillPlayerBaseInfoMsg( uid, msg_ddz_room, current_uid )
    
    
    local room_player = {};
    
    local player = PlayerMgr:GetPlayer(uid);
    
    if player~=nil then
    
        room_player.player_base = {
            UID         = uid,
            Nickname    = player.PlayerDataHelper.f_nickname;
            Portrait    = player.PlayerDataHelper.f_portrait;
        }
        
    end
    
    
    
    self:__FillPlayerRoomInfoMsg(uid, room_player, current_uid )
    
    table.insert( msg_ddz_room.player_list, room_player );
  
end

function RoomDdz:__FillPlayerRoomInfoMsg( uid, msg_room_player, current_uid )
    
    local room_player = self.RoomPlayerData[uid];
    
    if room_player~=nil then
    
        msg_room_player.state       = room_player:GetState();
        msg_room_player.hand_count  = room_player:GetHandCount();
        msg_room_player.seats       = self:GetPlayerSeatIdx(uid);
        msg_room_player.score       = room_player:GetScore();
        
    
    end
  
end

function RoomDdz:__InitData()
    
    self.CardsPool              = {  };
        
    for i,v in pairs(ConstCardsPool) do
        self.CardsPool[i] = v;
    end

    shuffle(self.CardsPool);
end

--释放函数
function RoomDdz:Release()
    
    self:BaseRelease();
end


return RoomDdz;
