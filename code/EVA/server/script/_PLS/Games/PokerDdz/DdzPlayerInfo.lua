local DdzPlayerInfo = class("DdzPlayerInfo", RoomPlayerBase)

-- 构造函数;
function DdzPlayerInfo:ctor()

    self.super:ctor();
    
    self.HandCards = {};
    
    self.QiangDiZhu = 0;    -- 抢地主权限     TDDZQiangDiZhu  TDDZJiaoFen
end

function DdzPlayerInfo:AddHandCards( tbl, start_idx, end_idx )
    if start_idx ~=nil then
        for idx=start_idx, end_idx do
            table.insert( self.HandCards, tbl[idx] );
        end
    else
        for _,v in ipairs(tbl) do
            table.insert( self.HandCards, v );
        end
    end
end



-- 清除每局临时数据
function DdzPlayerInfo:ClearOneGameData()
    
    self.HandCards = {};
    self:ClearAllState();
end

function DdzPlayerInfo:SetState( enum_name )
    local enum_val = protobuf.enum_id( "PB.TDDZPlayerState", enum_name );
    Misc.SetBit(self._State, enum_val);
end

function DdzPlayerInfo:GetState( enum_name )
    local enum_val = protobuf.enum_id( "PB.TDDZPlayerState", enum_name );
    return Misc.GetBit(self._State, enum_val);
end

function RoomPlayerBase:ClearState( enum_name )
    local enum_val = protobuf.enum_id( "PB.TDDZPlayerState", enum_name );
    Misc.ClearBit(self._State, enum_val);
end

function DdzPlayerInfo:IsSelectJiaBei()
    return self:GetStateEnum("PB.TDDZPlayerState", "STATE_DDZ_SELECT_JIABEI");
end


function DdzPlayerInfo:IsQiangDiZhu()
    return self:GetStateEnum("PB.TDDZPlayerState", "STATE_DDZ_QIANGDIZHU");
end

function DdzPlayerInfo:IsReady()
    return self:GetStateEnum("PB.TDDZPlayerState", "STATE_DDZ_READY");
end

function DdzPlayerInfo:SetReady()
    return self:SetStateEnum("PB.TDDZPlayerState", "STATE_DDZ_READY");
end

function DdzPlayerInfo:CancleReady()
    return self:ClearStateEnum("PB.TDDZPlayerState", "STATE_DDZ_READY");
end

return DdzPlayerInfo;
