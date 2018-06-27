local RoomDdz = class("RoomDdz", RoomBase)

DdzFSM              = require("Games/PokerDdz/DdzFSM")
DdzPlayerInfo       = require("Games/PokerDdz/DdzPlayerInfo")
DdzOutCardData      = require("Games/PokerDdz/DdzOutCardData")

--[[
        斗地主房间内游戏逻辑。
--]]

local tbinsert = table.insert

function RoomDdz:ctor()

    self.super:ctor();
    
    self.CFG_HAND_COUNT         = 17;       -- 每个玩家的初始手牌个数
    self.CFG_TOTAL_CARD         = 54;       -- 牌池
    self.CFG_BASE_SCORE         = 1;        -- 底分
    self.CFG_DIPAI_COUNT        = 3;        -- 底牌个数
    self.CFG_HANDCOUNT_MAX      = 20;       -- 手牌最大个数
    self.CFG_TRUSTMAX           = 3;        -- 最大托管次数
    self.CFG_FENGDING_16        = 16;       -- 16封顶
    self.CFG_FENGDING_32        = 32;       -- 32封顶
    self.CFG_FENGDING_64        = 64;       -- 64封顶
    

    self.Fsm                    = DdzFSM:new();
    self.Fsm:Init(self);

    self._LastOutCardData       = DdzOutCardData:new();
    self._TempOutCardData       = DdzOutCardData:new();
    
    
    -- 战绩回放
    self._RecordNodeList        = {};       -- 战绩回放的节点
    self._RecordGames           = {};       -- 记录房间中的每局战绩  list[_RecordNodeList]
    
    self:ResetGameData();
end

-- 每局清除数据
function RoomDdz:ResetGameData()
    
    self._CardsPool             = {};
	self._CardsBottom           = {};       -- 剩下的三张底牌
    self._ActionID              = 0;        -- 当前活动的玩家
    self._ActionUserWik         = 0;        -- 当前活动玩家权限
    self._GameCount             = 1;        -- 当前是第几局
    self._Multiple              = 1;        -- 房间翻倍数
    
    self._QiangDiZhuWiki        = 0;        -- 抢地主权限
    self._QiangDiZhu            = true;     -- 是否是叫地主抢地主模式
    self._DiZhuID               = 0;        -- 地主 uid
    self._CanOutCard            = {};       -- 当前可以出的牌，托管时使用
    self.ShowDownEvent          = {};       -- 结算事件
    
    self._LastOutCardData:ClearData();
    self._TempOutCardData:ClearData();
    
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
    if self.RoomPlayerData:Count()<self.CFG_RM_MIN then
        return false;
    end
    
    -- 是否都点了准备
    for _,v in pairs(self.RoomPlayerData.map) do
        if not v:GetState( enum.STATE_DDZ_READY ) then
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
        --ddz_player:SetState( enum.STATE_DDZ_NEWROLE );
        self:BaseJoinRoom(player);
        self:RefreshRoomInfo(player.UID);
        return true;
    else
        
        -- 如果房间已满，跳出。
        if self.RoomPlayerData:Count() == self.CFG_RM_MIN then
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
        
        self:BaseJoinRoom(player);
        self:BroadcastGameInfo();
        return true;
    end
end

function RoomDdz:TickUpdate()
    self.Fsm:TickUpdate();
end

function RoomDdz:UserStartReady( uid )
    local room_player = self.RoomPlayerData:Find(uid);
    
    if room_player~=nil then
        room_player:SetState(enum.STATE_DDZ_READY);
        local msg_int = { value=uid };
        self:BroadcastMsg( "DDZ_SR", "PB.MsgInt", msg_int );
    end
end

function RoomDdz:UserCancelReady( uid )
    local room_player = self.RoomPlayerData:Find(uid);
    
    if room_player~=nil then
        room_player:ClearState( enum.STATE_DDZ_READY );
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

-- 刷新加倍的选择
function RoomDdz:RefreshSelectJiaBei( uid, msg_jbr )
    if self.Fsm:IsState("TDDZStateSelectAddTimes") then
        local room_player = self.RoomPlayerData:Find(uid);

        if room_player~=nil then
            room_player:SetState( enum.STATE_DDZ_SELECT_JIABEI );

            if msg_jbr.result == enum.DDZ_AT_JIABIE then
                room_player:SetState( enum.STATE_DDZ_JIABEI );
            end
            
            msg_jbr.playid  = uid;
            msg_jbr.state   = room_player:GetState();
            
            self:BroadcastMsg("DDZ_JB", "PB.MsgJiaBeiResult", msg_jbr);

            for _,v in pairs(self.RoomPlayerData.map) do
                if not v:GetState(enum.STATE_DDZ_SELECT_JIABEI) then
                    return;
                end
            end

            -- 回放开始记录
            self:__RecordGameStart();
            
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
                    self:__AddShowDownEvent(enum.EVENT_QIANGDIZHU, 1);
                        
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
            
        self._QiangDiZhuWiki = WIK;
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

-- 设置地主和农民,抢地主完成
function RoomDdz:SetDiZhuState( uid )
    
    local ply_dz = self.RoomPlayerData:Find(uid);
    
    if ply_dz~=nil then
    
        ply_dz:SetState( enum.STATE_DDZ_DIZHU );
        ply_dz:AddHandCards( self._CardsBottom );
        SortPokerLogicValue( ply_dz.HandCards );
        
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
            
            tbinsert(MsgBRQiangDiZhuResult.player_list, MsgQiangDiZhuResult);
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

--  广播房间活动玩家
function RoomDdz:BroadGameActionPlayer( )
    self._ActionUserWik = self:__GetActionWik();
    
    local MsgDDZActon = {
        new_actionid    = self._ActionID,
        old_actionid    = self._LastOutCardData.UID,
        last_out_type   = self._LastOutCardData.Type,
        last_out_cards  = self._LastOutCardData.Cards,
        wik             = self._ActionUserWik,
        player_list     = {}
    }
    
    for _,uid in ipairs(self.SeatPlayers) do
        local ddz_player = self.RoomPlayerData:Find(uid);
        if ddz_player~=nil then
            local MsgDDZPlayer = {
                state       = ddz_player:GetState(),
                hand_count  = ddz_player:GetCardCount(),
                score       = ddz_player.Score,
                player_base = { pid = uid }
            }
            
            tbinsert( MsgDDZActon.player_list, MsgDDZPlayer );
        end
    end
    
    local player = PlayerMgr:GetPlayer(self._ActionID);
    
    if player~=nil then
        BaseService:SendToClient(player, "DDZ_RA", "PB.MsgDDZActon", MsgDDZActon);
    end
    
    MsgDDZActon.wik     = enum.ASK_DDZ_NULL;
    self:BroadcastMsg( "DDZ_RA", "PB.MsgDDZActon", MsgDDZActon, self._ActionID );
end


function RoomDdz:UserOutCard( uid, msg_oc )

    if uid~=self._ActionID or not self:__GetFsmState(enum.TDDZStateAction) then
        return false;
    end

    local room_player = self.RoomPlayerData:Find(uid);
    
    if room_player==nil then
        return false;
    end
    
    
    -- 检查用户是否可以出牌
    if not self:__UserOutCardLimit( room_player, msg_oc.out_cards ) then
        return false;
    end
    
    self._ActionUserWik = enum.ASK_DDZ_NULL;
    
    -- 检查手中是否还有牌。
    if room_player:GetHandCount()>0 then
        self.Fsm:SwitchState("TDDZStateOutCard");
        return true;
    end
    
    -- 手牌数为零，已出完牌。
    self._ActionID = uid;
    self:__CheckIsChunTian();       -- 检查春天
    
    self.Fsm:SwitchState("TDDZStateShowDown");  -- 跳结算
end

function RoomDdz:BroadcastGameInfo( )
    for _,v in ipairs(self.SeatPlayers) do
        if v~=0 then
            local msg_gameinfo = {};
            self:SendGameInfo( v, "DDZ_GI", msg_gameinfo );
        end
    end
end

function RoomDdz:RefreshRoomInfo( uid )
    local msg_gameinfo = {};
    self:SendGameInfo( uid, "DDZ_BLC", msg_gameinfo );
end

function RoomDdz:SendGameInfo( uid, msg_name, msg_ddz_room )
    msg_ddz_room.room_id = self.PrvRoomID;
    self:__FillRoomInfoMsg(msg_ddz_room, uid);
    
    local cur_state = self:__GetFsmState();
    if  uid==self._ActionID and 
        ( cur_state==enum.TDDZStateAction or 
          cur_state==enum.TDDZStateOutCard or 
          cur_state==enum.TDDZStateRelieveRoom ) 
    then
        msg_ddz_room.wik = self._ActionUserWik;
    else
        msg_ddz_room.wik = enum.ASK_DDZ_NULL;
    end

	local player = PlayerMgr:GetPlayer(uid);
    BaseService:SendToClient( player, msg_name, "PB.MsgDDZRoom", msg_ddz_room )
end

-- 游戏结算
function RoomDdz:GameShowDown()
    self._GameCount     = self._GameCount + 1;
    self:__AddShowDownEvent(enum.EVENT_DDZDIFEN, 1);
    
    local room_player = self.RoomPlayerData:Find( self._DiZhuID );
    if room_player==nil then
        return;
    end

    -- 计算玩家的翻倍数,设置地主翻倍数.
    local dizhu_jb = room_player:GetState( enum.STATE_DDZ_JIABEI );
    local total_multi, jiabei_cnt = self:__CalcJiaBei( dizhu_jb );
    room_player.Mulit = total_multi;
    
    -- 封顶加减分
    if self:CheckRoomSpecialKind(enum.TSK_DDZ_BFD) then
        self:__NormalCalcScore(total_multi);
    elseif self:CheckRoomSpecialKind(enum.TSK_DDZ_16) then
        self:__CalcScore( total_multi, 16, jiabei_cnt );
    elseif self:CheckRoomSpecialKind(enum.TSK_DDZ_32) then
        self:__CalcScore( total_multi, 32, jiabei_cnt );
    elseif self:CheckRoomSpecialKind(enum.TSK_DDZ_64) then
        self:__CalcScore( total_multi, 64, jiabei_cnt );
    else
        self:__NormalCalcScore(total_multi);
    end
end

function RoomDdz:AfterShowDown()
    local COST_CFG = StaticTableMgr:GetCreateCost( self.CreateInfo.consume_id );
    
    if COST_CFG~=nil then
        if self._GameCount >= COST_CFG.game_cnt then
        
            self:__GameOverRecordLog();
            for uid,room_player in pairs(self.RoomPlayerData.map) do
                self:ReleaseRoomPlayer(uid);
            end
            
            self.RoomPlayerData:Clear();
        end
    end
end

function RoomDdz:BroadcastShowDownInfo()
    local COST_CFG = StaticTableMgr:GetCreateCost( self.CreateInfo.consume_id );
    if COST_CFG==nil then   return;     end

    local MsgDDZRoomShowDown = {
        room_id     = self.PrvRoomID,
        room_state  = self:__GetFsmState(),
        state_time  = 0,
        game_count  = self._GameCount,
        time        = os.time(),
        game_over   = self._GameCount>=COST_CFG.game_cnt,
        event_count = {},
        player_list = {}
    };
    
    for eid,cnt in pairs(self.ShowDownEvent) do
        local MsgDDZShowDownEvent = { event_id=eid, count=cnt };
        tbinsert( MsgDDZRoomShowDown, MsgDDZShowDownEvent );
    end
    
    for uid,room_player in pairs(self.RoomPlayerData.map) do
    
        local MsgDDZPlayer = {
            player_base     = { UID = uid },
            
        };
        
        self:__FillPlayerRoomInfoMsg( uid, MsgDDZPlayer, uid );
        tbinsert( MsgDDZRoomShowDown.player_list, MsgDDZPlayer );
    end
    
    if MsgDDZRoomShowDown.game_over then
        -- self:__AddPlayerIntegral
    end
    
    self:BroadcastMsg( "DDZ_SD", "PB.MsgDDZRoomShowDown", MsgDDZRoomShowDown );
    self:__RecordGameShowDown( MsgDDZRoomShowDown );
end


function RoomDdz:RelieveRequestRoom( uid, is_relieve )
    
end

function RoomDdz:RelieveAutoRoom( )
    self._GameCount = 9999;
    --self.Fsm:SwitchState("TDDZStateShowDown");  -- 跳结算
    self:BroadcastShowDownInfo();
    self:AfterShowDown();
end

function RoomDdz:RelieveForceRoom( uid )
end

-- 普通算分
function RoomDdz:__NormalCalcScore( total_multi )
    for uid,room_player in pairs(self.RoomPlayerData.map) do
        if uid==self._DiZhuID then
            room_player:ChangeScore( total_multi, self._ActionID==self._DiZhuID )
        else
            room_player:ChangeScore( room_player.Mulit, self._ActionID~=self._DiZhuID )
        end
    end
end

-- 计算玩家的积分
function RoomDdz:__CalcScore( total_multi, limit_score, jiabei_cnt )

    if total_multi>limit_score then
        for uid,room_player in pairs(self.RoomPlayerData.map) do
        
            --  两个都没有加倍或者两个都加倍，在封顶时输赢的分相同
            if jiabei_cnt==0 or jiabei_cnt==2 then
                if uid==self._DiZhuID then
                    room_player:ChangeScore( limit_score, self._ActionID==self._DiZhuID );
                else
                    room_player:ChangeScore( limit_score/2, self._ActionID~=self._DiZhuID );
                end
            else
                -- 农民中一个加倍一个不加倍，封顶时要按照比例进行加减分
                if uid~=self._DiZhuID then
                    if room_player:GetState(enum.STATE_DDZ_JIABEI) then
                        if limit_score==16 then
                            room_player:ChangeScore( 11, self._ActionID~=self._DiZhuID );
                        elseif limit_score==32 then
                            room_player:ChangeScore( 22, self._ActionID~=self._DiZhuID );
                        elseif limit_score==64 then
                            room_player:ChangeScore( 43, self._ActionID~=self._DiZhuID );
                        end
                    else
                        if limit_score==16 then
                            room_player:ChangeScore( 5, self._ActionID~=self._DiZhuID );
                        elseif limit_score==32 then
                            room_player:ChangeScore( 10, self._ActionID~=self._DiZhuID );
                        elseif limit_score==64 then
                            room_player:ChangeScore( 21, self._ActionID~=self._DiZhuID );
                        end
                    end
                else
                    room_player:ChangeScore( limit_score, self._ActionID==self._DiZhuID );
                end
            end
        end
    else
        self:__NormalCalcScore( total_multi );
    end
end

-- 计算玩家加倍
function RoomDdz:__CalcJiaBei( is_dizhu )
    local total_multi   = 0;
    local jiabei_cnt    = 0;
    local add_times     = self:__GetSpecialCfg("add_times");
    
    for uid,room_player in pairs(self.RoomPlayerData.map) do
        
        if uid~=self._DiZhuID then
        
            -- 如果没有加倍玩法，直接设置房间的加倍数
            if not self:CheckRoomSpecialKind(enum.TSK_DDZ_JIABEI) then
                room_player.Mulit = self._Multiple;
                total_multi = total_multi + room_player.Mulit;
            else
                if room_player:GetState(enum.STATE_DDZ_JIABEI) then
                    if is_dizhu then
                        room_player.Mulit = self._Multiple * add_times * 2;
                    else
                        room_player.Mulit = self._Multiple * add_times;
                    end
                    
                    jiabei_cnt = jiabei_cnt + 1;
                else
                    if is_dizhu then
                        room_player.Mulit = self._Multiple * add_times;
                    else
                        room_player.Mulit = self._Multiple;
                    end
                end
                
                total_multi = total_multi + room_player.Mulit;
            end
        end
    end
    
    return total_multi, jiabei_cnt;
end

-- 战绩保存
function RoomDdz:__GameOverRecordLog()
    
end

function RoomDdz:__FillRoomInfoMsg( msg_ddz_room, current_uid )

    msg_ddz_room.room_id        = self.PrvRoomID;

	msg_ddz_room.room_state     = self:__GetFsmState();
    msg_ddz_room.state_time     = 0;                    -- 当前房间状态的运行时间
    msg_ddz_room.action_id      = self._ActionID;
    msg_ddz_room.game_count     = self._GameCount;
    msg_ddz_room.multiple       = self._Multiple;       -- 房间的翻倍数


    msg_ddz_room.bottom_cards   = self._CardsBottom;    -- 底牌


    -- 上把牌信息
    if self._LastOutCardData.UID>0 then
        
        msg_ddz_room.last_outcard = {
            old_actionid    = self._LastOutCardData.UID,
            out_type        = self._LastOutCardData.Type,
            out_cards       = self._LastOutCardData.Cards
        };
    end
    
    -- 创建房间的信息
    msg_ddz_room.private_room = self.CreateInfo;

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
    
        msg_room_player.state           = room_player:GetState();           -- 玩家的当前状态
        msg_room_player.hand_count      = room_player:GetHandCount();       -- 玩家手中的牌数
        msg_room_player.seats           = self:GetPlayerSeatIdx(uid);       -- 玩家的座位
        msg_room_player.score           = room_player:GetScore();           -- 玩家当前的积分
        msg_room_player.show_down_score = room_player:GetScore() - room_player.StartScore;  -- 填充玩家的输赢积分
        msg_room_player.qingdizhu_value = room_player.QiangDiZhu;

        if self:__GetFsmState(enum.TDDZStateQiangDiZhu) and room_player:GetState(enum.STATE_DDZ_QIANGDIZHU) then
            msg_room_player.qingdizhu_wiki = self._QiangDiZhuWiki;
        end
        
        if uid==current_uid or room_player:GetState(enum.STATE_DDZ_MINGPAI) then
            msg_room_player.card_list   = room_player.HandCards;
        end
    end
end

function RoomDdz:__CheckCanOutCards( room_player )
    --self._CanOutCard    = {};

    -- 上次出牌的玩家也是自己，或刚开局。
    if  self._ActionID==self._LastOutCardData.UID or self._LastOutCardData:GetCardCount()==0  then
        return true;
    end

    return DdzCardtypes:CheckCanOutCards( self._LastOutCardData, room_player.HandCards );
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

function RoomDdz:__RefreshPlayerQDZState( uid )
    self.RoomPlayerData:ForEach( function(k,v)
        if k==uid then
            v:SetState( enum.STATE_DDZ_QIANGDIZHU );
        else
            v:ClearState( enum.STATE_DDZ_QIANGDIZHU );
        end
    end )
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

-- 获取当前活动玩家操作权限
function RoomDdz:__GetActionWik()
    self._CanOutCard        = {};
    
    local WIK               = enum.ASK_DDZ_NULL;
    local room_player       = self.RoomPlayerData:Find(self._ActionID);
    
    if room_player==nil then
        return WIK;
    end
    
    -- 地主第一次出牌，没有选择明牌时可以有选择明牌的选项
    if  self:CheckRoomSpecialKind(enum.TSK_DDZ_MINGPAI) and
        self._ActionID==self._DiZhuID and
        room_player:GetState(enum.STATE_DDZ_MINGPAI) and
        room_player:GetHandCount()==self.CFG_HANDCOUNT_MAX
    then
        WIK = Misc.SetBit( WIK, enum.ASK_DDZ_DIZHU_MINGPAI );
    end
    
    local is_can_out = self:__CheckCanOutCards(room_player);
    
    if is_can_out then
        if self._ActionID==self._LastOutCardData.UID or self._LastOutCardData:GetCardCount()==0 then
            WIK = Misc.SetBit( WIK, enum.ASK_DDZ_TISHI );
            WIK = Misc.SetBit( WIK, enum.ASK_DDZ_CHUPAI );
        else
            WIK = Misc.SetBit( WIK, enum.ASK_DDZ_TISHI );
            WIK = Misc.SetBit( WIK, enum.ASK_DDZ_CHUPAI );
            WIK = Misc.SetBit( WIK, enum.ASK_DDZ_BUCHU );
        end
    else
        WIK = Misc.SetBit( WIK, enum.ASK_DDZ_BUCHU );
    end
    
    return WIK;
end

-- 检查用户可否出牌
function RoomDdz:__UserOutCardLimit( room_player, out_cards )
    
    if room_player.UID ~= self._ActionID or not self:__GetFsmState(enum.TGuanDanStateAction) then
        return false;
    end
    
    -- 排序要出的牌,分析用。
    SortPokerLogicValue(out_cards);

    -- 获取牌型
    local card_type, comp_val = DdzCardtypes:GetCardType( out_cards, self._LastOutCardData.Type );
    if card_type == enum.CT_DDZ_ERROR then
        return false;
    end
    
    -- 
    local temp_data     = self._TempOutCardData;
    temp_data:SetCards(out_cards);
    temp_data.Type      = card_type;
    temp_data.UID       = self._ActionID;
    temp_data.CompVal   = comp_val;
    
    -- 比较大小
    if not DdzCardtypes:CompareCards( temp_data, self._LastOutCardData ) then
        return false;
    end

    -- 可以出牌，从手牌中删除玩家需要出的牌
    if not room_player:RemoveCards(temp_data.Cards) then
        self:RefreshRoomInfo(self._ActionID);
        return false;
    end
    
    self._ActionID  = self:GetNextUID( self._ActionID );
    self._LastOutCardData:Copy( temp_data );
    
    local MsgDDZUserOutCard = {
        old_actionid    = temp_data.UID,
        out_type        = card_type,
        hand_count      = room_player:GetCardCount(),
        hand_card       = room_player.HandCards
    }
    
    -- 出炸弹时需要瞬间翻倍，同步到客户端
    if card_type==enum.CT_DDZ_ZHADAN_SIZHANG or card_type==enum.CT_DDZ_HUOJIAN then
        local multi_boom = self:__GetSpecialCfg("multi_boom");
        if multi_boom~=nil then
            self._Multiple = self._Multiple * multi_boom;
            self:__AddShowDownEvent( enum.EVENT_ZHADAN, 1 );
            MsgDDZUserOutCard.multiple = self._Multiple;
        end
    end
    
    -- 广播出牌信息
    self:BroadcastMsg( "DDZ_OC", "PB.MsgDDZUserOutCard", MsgDDZUserOutCard );
    
    -- 添加玩家出牌次数，地主被春天时使用
    room_player:AddOutCount();
    
    -- 记录出的牌
    self:__RecordGameOutCard( MsgDDZUserOutCard );
    
    -- 清除上把的过牌状态
    self:__ClearPlayerState(enum.STATE_DDZ_GUOPAI);
    return true;
end

-- 添加玩家状态
function RoomDdz:__AddPlayerState( state, uid )
    for id,v in pairs(self.RoomPlayerData.map) do
        if not (uid~=nil and id~=uid) then
            v:SetState( state );
        end
    end
end

-- DelRoleState
function RoomDdz:__ClearPlayerState( state, uid )
    for id,v in pairs(self.RoomPlayerData.map) do
        if not (uid~=nil and id~=uid) then
            v:ClearState( state );
        end
    end
end

function RoomDdz:__RecordGameOutCard( out_msg )
    
    
    
end


function RoomDdz:__GetFsmState( state )
    if state~=nil then
        if enum[self.Fsm:GetState()]==state then
            return true;
        end
        return false;
    else
        return enum[self.Fsm:GetState()];
    end
end

function RoomDdz:__CheckIsChunTian()
    local is_ct = false;
    
    for uid,v in pairs(self.RoomPlayerData.map) do
    
        -- 检查地主有没有被春天
        if uid==self._DiZhuID and self._DiZhuID~=self._ActionID and v:GetOutCount()==1 then
            is_ct = true;
            break;
        end
        
        -- 地主获胜需要检查两个农民有没有被春天
        if self._ActionID==self._DiZhuID and uid~=self._ActionID then
        
            if v:GetCardCount()==self.CFG_HAND_COUNT then
                is_ct = true;
            else
                is_ct = false;
                break;
            end
        end
    end
    
    if is_ct then
        local multi_ct = self:__GetSpecialCfg("multi_ct");
        if multi_ct~=nil then
            self._Multiple = self._Multiple * multi_ct;
            self:__AddShowDownEvent(enum.EVENT_CHUNTIAN, 1);
        end
    end
end


-- --------------------------  战绩回放  Start
-- 记录游戏开始
function RoomDdz:__RecordGameStart()
    
    self._RecordNodeList    = {};
    
    

end

function RoomDdz:__RecordGameOutCard()

end

function RoomDdz:__RecordGameActionState()

end

function RoomDdz:__RecordGameShowCardPass()


end

function RoomDdz:__RecordGameShowDown( MsgDDZRoomShowDown )

end

function RoomDdz:__RecordGameShowDownEvent()

end

-- --------------------------  战绩回放  End

--释放函数
function RoomDdz:Release()
    self:BaseRelease();
end


return RoomDdz;
