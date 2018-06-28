local RobotGameDdz = class("RobotGameDdz", RobotGameBase)

function RobotGameDdz:ctor()
    self.super:ctor();
    self.Robot = nil;
    self.RoomInfo   = nil;
    self.SelfData   = nil;          -- 服务器上 player_list中自己的房间内数据
    self.HandCards  = {};
    self.WIK        = 0;
end

function RobotGameDdz:GetFsmState()
    return self.Robot.GameFsm:GetState();
end

function RobotGameDdz:GetRobotState( enum_idx )
    return Misc.GetBit( self.SelfData.state, enum_idx );
end

function RobotGameDdz:__UID()
    return self.Robot.Data.UID;
end

function RobotGameDdz:DoCreatePrvRoom()
    local create_prvroom = { room_type="RM_DDZ", consume_id=1001, special_kind=0x1c5 };
    self.Robot:Send( "CPRM", "PB.MsgCreatePrivateRoom", create_prvroom );
end

function RobotGameDdz:DoJoinPrvRoom(open_room)
    local join_prvroom = { room_id=open_room.RoomID, room_type=open_room.RoomType };
    self.Robot:Send( "EPRM", "PB.MsgEnterPrivateRoom", join_prvroom );
end

function RobotGameDdz:DoReady()
    self.Robot:Send( "DDZ_SR" );
end

-- 轮到玩家出牌
function RobotGameDdz:DoAction()

    if self.WIK ~= enum.ASK_DDZ_NULL then
        local flg_chupai    = 1 << enum.ASK_DDZ_CHUPAI;
        local flg_buchu     = 1 << enum.ASK_DDZ_BUCHU;
        local select_rnd    = math.random( 1, 15 );
        
        if (self.WIK&flg_buchu and select_rnd==1) or #self.HandCards==0 then
            self.Robot:Send( "DDZ_PS" );
        else
            local MsgDDZUserOutCard = {  out_cards = {}  };
            local rnd_oc = math.random( 1, #self.HandCards );
            table.insert( MsgDDZUserOutCard.out_cards, self.HandCards[rnd_oc] );
            self.Robot:Send( "DDZ_OC", "PB.MsgDDZUserOutCard", MsgDDZUserOutCard );
            self.Robot:Print( "out card:" .. self.HandCards[rnd_oc] );
        end
    end
end

function RobotGameDdz:cbDdzGameInfo( msgin )
    
    local ddz_gi = msgin:rpb("PB.MsgDDZRoom");
    
    if ddz_gi==nil then
        nlwarning("ddz_gi==nil !!!!!!!!!!!!");
        return
    end

    -- 刷新房间内数据
    self.RoomInfo = ddz_gi;
    
    -- 刷新自己的数据
    for _,v in ipairs(ddz_gi.player_list) do
        if v.player_base.UID == self.Robot.Data.UID then
            self.SelfData   = v;
            self.HandCards  = v.card_list;
        end
    end
    
    
    self.Robot:PrintTable(ddz_gi);
    self.Robot:Print("=======>  RobotGameDdz:cbDdzGameInfo  UID:"..self.Robot.Data.UID);
    
    if self:GetFsmState()=="TCreatePrvRoom" then
        
        --if self:GetRobotState(enum.STATE_DDZ_ROOM_OWNER) then
            -- 房间是自己创建的，返回的是创建成功
        --    self.IsCreate = true;
        --end
        
        
        self.Robot:Print("=========>Create private room.   UID:"..self.Robot.Data.UID);
        -- 把room_id加入到开放列表中，供其它机器人加入。
        
        local pb_room_info = PublicRoomInfo:new();
        pb_room_info.RoomType   = ddz_gi.private_room.room_type;
        pb_room_info.RoomID     = ddz_gi.room_id;
        pb_room_info.RoomRobots = ddz_gi.player_list;
        
        PublicRoomInfoMgr:PushOpenRoom(pb_room_info);
        
        self.Robot.GameFsm:SwitchState("TInRoomIdlem");
    elseif self:GetFsmState()=="TJoinPrvRoom" then
        self.Robot:Print("=========>Join private room.   UID:"..self.Robot.Data.UID);
        self.Robot.GameFsm:SwitchState("TInRoomIdlem");
    else
        
        self.Robot:Print("=========>Refresh private room.   UID:"..self.Robot.Data.UID);
    end
end

-- 有玩家点击了准备，如果是自己就更新状态
function RobotGameDdz:cbDdzUserStartReady( msgin )
    
    local ready_uid = msgin:rpb("PB.MsgInt");
    
    if self.Robot.Data.UID==ready_uid then
        
        self.SelfData.state = Misc.SetBit( self.SelfData.state, enum.STATE_DDZ_READY );
        self.Robot:Print("cbDdzUserStartReady");
    end
end

function RobotGameDdz:cbDDZ_QDZ_QX( msg_qdz )
    self.Robot:Print("SELF:"..self.Robot.Data.UID.."    cbDDZ_QDZ_QX  UID:"..msg_qdz.playid.."  WIK:"..msg_qdz.qingdizhu_wiki);
    
    if self.Robot.Data.UID==msg_qdz.playid then
        
        if msg_qdz.qingdizhu_wiki > 0 then
            
            local select_wki_list = {};
            
            for i=1,10 do
                if Misc.GetBit( msg_qdz.qingdizhu_wiki, i ) then
                    table.insert(select_wki_list, i);
                end
            end
            
            if #select_wki_list==0 then
                nlwarning("#select_wki_list==0");
                return;
            end
            
            local rnd           = math.random(#select_wki_list);
            local select_wki    = select_wki_list[rnd];
            
            self.Robot:Print("Select WKI:"..select_wki);
            local MsgQiangDiZhuResult = { result=select_wki };
            self.Robot:Send( "DDZ_QDZ", "PB.MsgQiangDiZhuResult", MsgQiangDiZhuResult );
        end
    end
end

function RobotGameDdz:cbDDZ_QDZ_F( msg_qdz_res )

    -- 如果是地主，刷新手牌
    for _,v in ipairs(msg_qdz_res.player_list) do
        if v.playid==self.Robot.Data.UID and #v.dizhu_cards>0 then
            self.HandCards = v.dizhu_cards;
            self.Robot:Print("Refresh DiZhu HandCards");
            break;
        end
    end
    
    local MsgJiaBeiResult = {
        result = math.random(2)
    };

    self.Robot:Send( "DDZ_JB", "PB.MsgJiaBeiResult", MsgJiaBeiResult );
end

-- 轮到玩家出牌
function RobotGameDdz:cbDDZ_RA( msg_ddz_act )
    
    self.Robot:PrintTable( msg_ddz_act );
    
    if msg_ddz_act.new_actionid == self:__UID() then
        self.WIK      = msg_ddz_act.wik;
    elseif msg_ddz_act.old_actionid == self:__UID() then

        
        
        -- 清除已出的牌
        for i,v in ipairs(self.HandCards) do
            if v==msg_ddz_act.last_out_cards[1] then
                table.remove( self.HandCards, i );
                break;
            end
        end
    end
    
    if self:GetFsmState()~="TAction" then
        self.Robot.GameFsm:SwitchState("TAction");
    end
end



return RobotGameDdz;
