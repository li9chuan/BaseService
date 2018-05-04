local RoomDdz = class("RoomDdz",RoomBase)

DdzFSM              = require("Games/PokerDdz/DdzFSM")
DdzPlayerInfo       = require("Games/PokerDdz/DdzPlayerInfo")
DDZOutCardData      = require("Games/PokerDdz/DDZOutCardData")

-- 构造函数;
function RoomDdz:ctor()

    self.super:ctor();
    print("RoomDdz:ctor");

    self.Fsm                    = DdzFSM:new();
    self.Fsm:Init(self);
    
    
    self._LastOutCardData       = DDZOutCardData:new();


    self.CFG_HAND_COUNT         = 17;       -- 每个玩家的初始手牌个数
    self.CFG_TOTAL_CARD         = 54;       -- 牌池
    self.CFG_BASE_SCORE         = 1;        -- 底分
    self.CFG_DIPAI_COUNT        = 3;        -- 底牌个数
    self.CFG_HANDCOUNT_MAX      = 20;       -- 手牌最大个数
    self.CFG_TRUSTMAX           = 3;        -- 最大托管次数
    self.CFG_FENGDING_16        = 16;       -- 16封顶
    self.CFG_FENGDING_32        = 32;       -- 32封顶
    self.CFG_FENGDING_64        = 64;       -- 64封顶
    


    self:ResetGameData();
end

function RoomDdz:ResetGameData()
    
    self._CardsPool             = {};
	self._CardsBottom           = {}; 		-- 剩下的三张底牌
    self._ActionID              = 0;        -- 当前活动的玩家
    self._GameCount             = 1;        -- 当前是第几局
    self._Multiple              = 1;        -- 房间翻倍数
    self._QingDiZhuWik          = 0;        -- 抢地主权限
    self._QiangDiZhu            = true;     
    
    self._LastOutCardData:ClearData();
    
    -- 清空玩家临时数据
    for _,v in ipairs(self.SeatPlayers) do
        if v~=0 then
            local room_player = self.RoomPlayerData[v];
            if room_player~=nil then
                room_player:ClearOneGameData();
            end
        end
    end
    
    -- 洗牌
    for i,v in pairs(ConstCardsPool) do
        self._CardsPool[i] = v;
    end

    shuffle(self._CardsPool);
end

function RoomDdz:GameStartWait()
    if self:GetRoomPlayerNum()>=self._RoomMin then
        return true;
    end
    return false;
end

-- 玩家加入房间
function RoomDdz:JoinRoom( player )

    if #self.RoomPlayerData < self._RoomMin then
    
        local ddz_player = DdzPlayerInfo:new();
        ddz_player:SetState( "PB.TDDZPlayerState", "STATE_DDZ_NEWROLE" );
        self.RoomPlayerData[player.UID] = ddz_player;
        
        self:BaseJoinRoom(player);
        self:BroadcastGameInfo();
    end
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


function RoomDdz:SendQiangDiZhuWik()
    
    -- 随机一个玩家选择抢地主
    
    local seat_idx  = math.random(1, #self.SeatPlayers);
    self._ActionID  = self.SeatPlayers[seat_idx];
    local player    = self.RoomPlayerData[self._ActionID]
    
    if player~=nil then
    
        if _QiangDiZhu then
            self._QingDiZhuWiki = SetBit( self._QingDiZhuWiki, "PB.TDDZQiangDiZhu", "DDZ_QDZ_BUJIAO" );
            self._QingDiZhuWiki = SetBit( self._QingDiZhuWiki, "PB.TDDZQiangDiZhu", "DDZ_QDZ_JIAODIZHU" );
        else
            self._QingDiZhuWiki = SetBit( self._QingDiZhuWiki, "PB.TDDZQiangDiZhu", "DDZ_QDZ_BUJIAO" );
            self._QingDiZhuWiki = SetBit( self._QingDiZhuWiki, "PB.TDDZQiangDiZhu", "DDZ_JF_JIAO_ONE" );
            self._QingDiZhuWiki = SetBit( self._QingDiZhuWiki, "PB.TDDZQiangDiZhu", "DDZ_JF_JIAO_TWO" );
            self._QingDiZhuWiki = SetBit( self._QingDiZhuWiki, "PB.TDDZQiangDiZhu", "DDZ_JF_JIAO_THREE" );
        end

        self:RefreshPlayerQiangDiZhuState(self._ActionID);
        
        local msg_qdz = {
            playid = self._ActionID;
            qingdizhu_wiki = self._QingDiZhuWiki;
        }
        
        BaseService:SendToClient( player, "DDZ_QDZ_QX", "PB.MsgQiangDiZhu", msg_qdz );
        
        msg_qdz.qingdizhu_wiki = 0;
        self:BroadcastMsg( "DDZ_QDZ_QX", "PB.MsgQiangDiZhu", msg_qdz, self._ActionID );
    
    end
end

function RoomDdz:RefreshPlayerQiangDiZhuState( uid )

    for k,v in pairs(self.RoomPlayerData) do
        if k==uid then
            v:SetStateEnum("PB.TDDZPlayerState", "STATE_DDZ_QIANGDIZHU");
        else
            v:ClearStateEnum("PB.TDDZPlayerState", "STATE_DDZ_QIANGDIZHU");
        end
    end
end

-- 刷新加倍的选择
function RoomDdz:RefreshSelectJiaBei( uid, msg_jbr )

    if self.Fsm:IsState("TDDZStateQiangDiZhu") then
        local room_player = self.RoomPlayerData[uid];
        
        if room_player~=nil then
            --room_player:IsSelectJiaBei()
            
            room_player:SetStateEnum("PB.TDDZPlayerState", "STATE_DDZ_SELECT_JIABEI");
            
            if msg_jbr.result == enum("PB.TDDZAddTimes","DDZ_AT_JIABIE") then
                room_player:SetStateEnum("PB.TDDZPlayerState", "STATE_DDZ_JIABEI");
            end
            
            msg_jbr.playid  = uid;
            msg_jbr.state   = room_player:GetState();
            
            self:BroadcastMsg("DDZ_JB", "PB.MsgJiaBeiResult", msg_jbr);
            
            
            for k,v in pairs(self.RoomPlayerData) do
                if v:IsSelectJiaBei()==false then
                    return;
                end
            end
               
            -- 如果所有玩家都选择过了，进入下一状态
            self.Fsm:SwitchState("TDDZStateAction");
        end
    end
end

-- 刷新抢地主
function RoomDdz:RefrshRoleQiangDiZhu( uid, msg_qdz )

    if self.Fsm:IsState("TDDZStateSelectAddTimes") then
        local room_player = self.RoomPlayerData[uid];
        
        if room_player~=nil then
            --room_player:IsSelectJiaBei()
            
            room_player.QiangDiZhu = msg_qdz.result;
            
            if self._QiangDiZhu then
                
                if msq_qdz.result==enum("PB.TDDZQiangDiZhu","DDZ_QDZ_QIANGDIZHU") then
                
                    self._Multiple = self._Multiple*3;
                
                end

            end
            
            msg_qdz.playid      = uid;
            msg_qdz.multiple    = self._Multiple;
            
            self:BroadcastMsg("DDZ_QDZ", "PB.MsgQiangDiZhuResult", msg_qdz)
            
            
        end
    end
end



function RoomDdz:SetDiZhuState( uid )
    
        
end


function RoomDdz:SendHandCard()

    local start_send    = 1;
    local end_send      = self.CFG_HAND_COUNT;
    
    for _,v in ipairs(self.SeatPlayers) do
        local room_player = self.RoomPlayerData[v];
        
        if room_player~=nil then
            
            room_player:AddHandCards( self._CardsPool, start_send, end_send );
            
            start_send  = start_send + self.CFG_HAND_COUNT;
            end_send    = end_send + self.CFG_HAND_COUNT;
        end
    end

    -- 发底牌
    self._CardsBottom           = {};
    local i=1;
    for idx=start_send, self.CFG_TOTAL_CARD do
        self._CardsBottom[i] = self._CardsPool[idx];
        i = i+1;
    end
    
end


function RoomDdz:BroadcastGameInfo( )

    for k,v in ipairs(self.SeatPlayers) do
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
    
    
    PrintTable(msg_ddz_room);
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


    for _,v in ipairs(self.SeatPlayers) do
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
    
    if msg_ddz_room.player_list==nil then
        msg_ddz_room.player_list = {};
    end
    
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



--释放函数
function RoomDdz:Release()
    
    self:BaseRelease();
end


return RoomDdz;
