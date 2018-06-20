local RoomDdz = class("RoomDdz", RoomBase)

DdzFSM              = require("Games/PokerDdz/DdzFSM")
DdzPlayerInfo       = require("Games/PokerDdz/DdzPlayerInfo")
DdzOutCardData      = require("Games/PokerDdz/DdzOutCardData")

require("Games/PokerDdz/DdzCardTypes")

--[[
        斗地主房间内游戏逻辑。
--]]

local tbinsert = table.insert

function RoomDdz:ctor()

    self.super:ctor();
    nlinfo("RoomDdz:ctor");

    self.Fsm                    = DdzFSM:new();
    self.Fsm:Init(self);
    
    
    self._LastOutCardData       = DdzOutCardData:new();


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

-- 每局清除数据
function RoomDdz:ResetGameData()
    
    self._CardsPool             = {};
	self._CardsBottom           = {};       -- 剩下的三张底牌
    self._ActionID              = 0;        -- 当前活动的玩家
    self._GameCount             = 1;        -- 当前是第几局
    self._Multiple              = 1;        -- 房间翻倍数
    self._QingDiZhuWik          = 0;        -- 抢地主权限
    self._QiangDiZhu            = true;     -- 是否是叫地主抢地主模式
    self._DiZhuID               = 0;        -- 地主 uid
    
    self._LastOutCardData:ClearData();
    
    -- 清空玩家临时数据
    for _,v in ipairs(self.SeatPlayers) do
        if v~=0 then
            local room_player = self.RoomPlayerData:Find(v);
            if room_player~=nil then
                room_player:ClearOneGameData();
            end
        end
    end
    
    -- 发牌洗牌
    for i,v in pairs(ConstCardsPoker) do
        self._CardsPool[i] = v;
    end

    shuffle(self._CardsPool);
end

-- 判断游戏是否可以开始
function RoomDdz:GameStartWait()
    --  人数是否足够
    if self.RoomPlayerData:Count()<self._RoomMin then
        return false;
    end
    
    -- 是否都点了准备
    for _,v in pairs(self.RoomPlayerData.map) do
        if not v:IsReady() then
            return false;
        end
    end
    
    return true;
end

-- 玩家加入房间
function RoomDdz:JoinRoom( player )
    local ddz_player = self.RoomPlayerData:Find(player.UID);

    if ddz_player~=nil then
        -- 返回房间
        ddz_player:SetState( enum.STATE_DDZ_NEWROLE );
    else
        
        -- 如果房间已满，跳出。
        if self.RoomPlayerData:Count() == self._RoomMin then
            return false
        end
        
        -- 第一次进入房间
        ddz_player = DdzPlayerInfo:new();
        ddz_player:SetState( enum.STATE_DDZ_NEWROLE );

        if player.UID == self.CreatorID then
            --  设置是房主
            ddz_player:SetState( enum.STATE_DDZ_ROOM_OWNER );
        end

        self.RoomPlayerData:Insert(player.UID, ddz_player);
    end

    self:BaseJoinRoom(player);
    self:BroadcastGameInfo();
    return true;
end

function RoomDdz:TickUpdate()
    self.Fsm:TickUpdate();
end

function RoomDdz:UserStartReady( uid )
    local room_player = self.RoomPlayerData:Find(uid);
    
    if room_player~=nil then
        room_player:SetReady();
        local msg_int = { value=uid };
        self:BroadcastMsg( "DDZ_SR", "PB.MsgInt", msg_int );
    end
end

function RoomDdz:UserCancelReady( uid )
    local room_player = self.RoomPlayerData:Find(uid);
    
    if room_player~=nil then
        room_player:CancleReady();
        local msg_int = { value=uid };
        self:BroadcastMsg( "DDZ_CR", "PB.MsgInt", msg_int );
    end
end

function RoomDdz:__SetQDZWiki( enum_idx )
    self._QiangDiZhuWiki = Misc.SetBit(self._QiangDiZhuWiki, enum_idx );
end

function RoomDdz:SendQiangDiZhuWik()
    
    -- 随机一个玩家选择抢地主
    local seat_idx  = math.random(1, #self.SeatPlayers);
    self._ActionID  = self.SeatPlayers[seat_idx];
    
    self._QiangDiZhuWiki = 0;
        
    if _QiangDiZhu then
        self:__SetQDZWiki( enum.DDZ_QDZ_BUJIAO );
        self:__SetQDZWiki( enum.DDZ_QDZ_JIAODIZHU );
    else
        self:__SetQDZWiki( enum.DDZ_QDZ_BUJIAO );
        self:__SetQDZWiki( enum.DDZ_JF_JIAO_ONE );
        self:__SetQDZWiki( enum.DDZ_JF_JIAO_TWO );
        self:__SetQDZWiki( enum.DDZ_JF_JIAO_THREE );
    end

    self:__RefreshPlayerQDZState(self._ActionID);
        
    local msg_qdz = {
        playid = self._ActionID;
        qingdizhu_wiki = self._QiangDiZhuWiki;
    }

    local player    = PlayerMgr:GetPlayer(self._ActionID);
    BaseService:SendToClient( player, "DDZ_QDZ_QX", "PB.MsgQiangDiZhu", msg_qdz );
        
    msg_qdz.qingdizhu_wiki = 0;
    self:BroadcastMsg( "DDZ_QDZ_QX", "PB.MsgQiangDiZhu", msg_qdz, self._ActionID );
end

function RoomDdz:__RefreshPlayerQDZState( uid )
    self.RoomPlayerData:ForEach( function(k,v)
        if k==uid then
            v:SetState( enum.STATE_DDZ_QIANGDIZHU );
        else
            v:ClearState( enum.STATE_DDZ_QIANGDIZHU );
        end
    end )
end

-- 刷新加倍的选择
function RoomDdz:RefreshSelectJiaBei( uid, msg_jbr )

    if self.Fsm:IsState("TDDZStateSelectAddTimes") then
        local room_player = self.RoomPlayerData:Find(uid);
        
        if room_player~=nil then
            --room_player:IsSelectJiaBei()
            
            room_player:SetState( enum.STATE_DDZ_SELECT_JIABEI );
            
            if msg_jbr.result == enum.DDZ_AT_JIABIE then
                room_player:SetState( enum.STATE_DDZ_JIABEI );
            end
            
            msg_jbr.playid  = uid;
            msg_jbr.state   = room_player:GetState();
            
            self:BroadcastMsg("DDZ_JB", "PB.MsgJiaBeiResult", msg_jbr);
            
            
            for _,v in pairs(self.RoomPlayerData.map) do
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

    if not self.Fsm:IsState("TDDZStateQiangDiZhu") then
        return
    end

    local room_player = self.RoomPlayerData:Find(uid);
        
    if room_player~=nil then
        --room_player:IsSelectJiaBei()
            
        local qdz_select = msg_qdz.result;
        room_player.QiangDiZhu = qdz_select;
            
        if self._QiangDiZhu then
                
            if qdz_select==enum.DDZ_QDZ_QIANGDIZHU then
                
                self._Multiple = self._Multiple*3;
                
            end

        end
            
        msg_qdz.playid      = uid;
        msg_qdz.multiple    = self._Multiple;
            
        self:BroadcastMsg("DDZ_QDZ", "PB.MsgQiangDiZhuResult", msg_qdz)


        --  设置下一个玩家抢地主的操作
        local next_uid = self:GetNextUID( uid );
            
        -- 如果都不叫，并且默认的地主叫了
        if self._QiangDiZhu and qdz_select~=enum.DDZ_QDZ_BUJIAO then
            while true do
                local next_player = self:GetRoomPlayer(next_uid);
                if next_player==nil then    return;  end
                    
                if next_uid==uid then
                    self._ActionID = uid;
                    self:SetDiZhuState(uid)
                    return;
                end
                    
                if next_player.QiangDiZhu == enum.DDZ_QDZ_BUJIAO then
                    next_uid = self:GetNextUID( next_uid );
                else
                    break;
                end
            end
        end
            
        local WIK   = 0;
            
        if self._QiangDiZhu then
            if qdz_select==enum.DDZ_QDZ_BUJIAO then
                    
                if next_uid == self._ActionID then
                    -- 没有人叫地主则系统随机的第一个人作为地主
                    self:SetDiZhuState(next_uid);
                    return;
                end
                    
                -- 设置客户端可选择权限
                WIK = Misc.SetBit(WIK, enum.DDZ_QDZ_JIAODIZHU);
                WIK = Misc.SetBit(WIK, enum.DDZ_QDZ_BUJIAO);
            else
                if qdz_select == enum.DDZ_QDZ_JIAODIZHU then
                    self._ActionID = uid;
                elseif qdz_select == enum.DDZ_QDZ_QIANGDIZHU then
                    -- 叫地主的玩家选择抢地主则完成抢地主
                    -- AddShowDownEvent(EVENT_QIANGDIZHU, 1);
                        
                    if uid==self._ActionID then
                        self:SetDiZhuState(next_uid);
                        return;
                    end
                elseif qdz_select == enum.DDZ_QDZ_BUQIANG then
                        
                    if next_uid==self._ActionID then
                            
                            
                        if not self:__CheckQiangDiDiZhu( next_uid ) then
                            -- 如果下家叫地主，且都不抢，设置next_uid地主
                            self:SetDiZhuState(next_uid);
                            return;
                        elseif uid==self._ActionID then
                            -- 叫地主的玩家选择不抢地主后，选择最后一次抢地主的玩家做地主
                            self._ActionID = self:__SelectDZ(next_uid);
                            self:SetDiZhuState(self._ActionID);
                            return;
                        end
                    end
                end
                    
                -- 设置客户端可选择权限
                WIK = Misc.SetBit(WIK, enum.DDZ_QDZ_QIANGDIZHU);
                WIK = Misc.SetBit(WIK, enum.DDZ_QDZ_BUQIANG);
            end
        end
            
        self._QingDiZhuWik = WIK;
        self:__RefreshPlayerQDZState(next_uid);
            
        local msg_qdz = {
            playid          = next_uid,
        };
        self:BroadcastMsg("DDZ_QDZ_QX", "PB.MsgQiangDiZhu", msg_qdz, next_uid)
            
        msg_qdz.qingdizhu_wiki = WIK;
        local player = PlayerMgr:GetPlayer(next_uid);
        BaseService:SendToClient( player, "DDZ_QDZ_QX", "PB.MsgQiangDiZhu", msg_qdz );
    end
end

-- 检查除了uid外，有没有玩家抢过地主
function RoomDdz:__CheckQiangDiDiZhu(uid)
    for k,v in pairs(self.RoomPlayerData.map) do
        if k~=uid and v.QiangDiZhu==enum.DDZ_QDZ_QIANGDIZHU then
            return true;
        end
    end
    return false;
end

-- 叫地主的玩家选择不抢地主后，选择最后一次抢地主的玩家做地主
function RoomDdz:__SelectDZ(uid)
    for i=1,i<4 do
        local next_uid = self:GetNextUID(uid);
        
        if next_uid~=self._ActionID then
            local room_player = self:GetRoomPlayer(next_uid);
            if room_player==nil then  return 0;  end
            
            if room_player.QiangDiZhu == enum.DDZ_QDZ_QIANGDIZHU then
                return next_uid;
            end
        end
        
        uid = next_uid;
    end
    
    return 0;
end

-- 设置地主和农民,抢地主完成
function RoomDdz:SetDiZhuState( uid )
    
    local ply_dz = self.RoomPlayerData:Find(uid);
    
    if ply_dz~=nil then
    
        ply_dz:SetState( enum.STATE_DDZ_DIZHU );
        ply_dz:AddHandCards( self._CardsBottom );
        
        
        self._DiZhuID   = uid;
        self._ActionID  = uid;
        
        
        --- 底牌翻倍
        
    
        -- 抢完地主清除玩家身上的抢地主状态
        self:__RefreshPlayerQDZState(0);
        
        -- 填充抢地主最终结果(服务器广播的)
        local MsgBRQiangDiZhuResult = {
            multiple    = self._Multiple;
            player_list = {};
        }
        
        -- 暂时不加底牌翻倍
        for u,v in pairs(self.RoomPlayerData.map) do
            if u~=self._DiZhuID then
                v:SetState( enum.STATE_DDZ_NONGMING )
            
            end
            
            local MsgQiangDiZhuResult = {
                playid      = u;
                state       = v:GetState();
                cardcount   = v:GetCardCount();
            }

            -- 如果地主明牌了，增加地主手牌
            if u==self._DiZhuID and ply_dz:GetState(enum.STATE_DDZ_MINGPAI) then
                MsgQiangDiZhuResult["dizhu_cards"] = ply_dz.HandCards;
            end
            
            table.insert(MsgBRQiangDiZhuResult.player_list, MsgQiangDiZhuResult);
        end
        
        self:BroadcastMsg( "DDZ_QDZ_F", "PB.MsgBRQiangDiZhuResult", MsgBRQiangDiZhuResult, self._DiZhuID );
        
        -- 填充地主手牌，发给地主。
        for _,v in pairs(MsgBRQiangDiZhuResult.player_list) do
            if v.playid==self._DiZhuID then
                if v.dizhu_cards==nil then
                    v.dizhu_cards = ply_dz.HandCards;
                end
                break;
            end
        end
        
        local player = PlayerMgr:GetPlayer(uid);
        BaseService:SendToClient(player, "DDZ_QDZ_F", "PB.MsgBRQiangDiZhuResult", MsgBRQiangDiZhuResult);
        
        -- 抢地主结束，切状态。
        
        if self:CheckRoomSpecialKind(enum.TSK_DDZ_JIABEI) then
            self.Fsm:SwitchState("TDDZStateSelectAddTimes");
        else
            self.Fsm:SwitchState("TDDZStateSelectAddTimes");
        end
    end
        
end

-- 发牌
function RoomDdz:SendHandCard()
    
    nlwarning("======================>   SendHandCard");

    local start_send    = 1;
    local end_send      = self.CFG_HAND_COUNT;
    
    for _,v in ipairs(self.SeatPlayers) do
        local room_player = self.RoomPlayerData:Find(v);
        
        if room_player~=nil then
            
            room_player:AddHandCards( self._CardsPool, start_send, end_send );
            
            start_send  = start_send + self.CFG_HAND_COUNT;
            end_send    = end_send + self.CFG_HAND_COUNT;
        end
    end

    -- 发底牌
    self._CardsBottom           = {};

    for idx=start_send, self.CFG_TOTAL_CARD do
        tbinsert( self._CardsBottom, self._CardsPool[idx] );
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

	msg_ddz_room.room_state     = enum[self.Fsm:GetState()];
    msg_ddz_room.state_time     = 0;                    -- 当前房间状态的运行时间
    msg_ddz_room.action_id      = self._ActionID;
    msg_ddz_room.game_count     = self._GameCount;
    msg_ddz_room.multiple       = self._Multiple;       -- 房间的翻倍数


    msg_ddz_room.bottom_cards   = self._CardsBottom;    -- 底牌


    -- 上把牌信息
    if self._LastOutCardData:IsEmpty()==false then
        
        msg_ddz_room.last_outcard = {
            old_actionid    = self._LastOutCardData.UID,
            out_type        = self._LastOutCardData.Type,
            out_cards       = self._LastOutCardData.Cards
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
    
    tbinsert( msg_ddz_room.player_list, room_player );
  
end

function RoomDdz:__FillPlayerRoomInfoMsg( uid, msg_room_player, current_uid )
    
    local room_player = self.RoomPlayerData:Find(uid);
    
    if room_player~=nil then
    
        msg_room_player.state       = room_player:GetState();
        msg_room_player.hand_count  = room_player:GetHandCount();
        msg_room_player.seats       = self:GetPlayerSeatIdx(uid);
        msg_room_player.score       = room_player:GetScore();
        
    
        --TDDZStateQiangDiZhu
        
        if uid==current_uid or room_player:GetState(enum.STATE_DDZ_MINGPAI) then
            msg_room_player.card_list   = room_player.HandCards;
        end
    end
  
end



--释放函数
function RoomDdz:Release()
    self:BaseRelease();
end


return RoomDdz;
