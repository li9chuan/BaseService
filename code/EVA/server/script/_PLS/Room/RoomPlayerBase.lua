local RoomPlayerBase = class("RoomPlayerBase")

function RoomPlayerBase:ctor()

    self._State         = 0;
    self._HandCards     = {};
    self._Score         = 0;            -- »ý·Ö

end

function RoomPlayerBase:SetStateEnum( enum_type, enum_name )
    local enum_val = protobuf.enum_id( enum_type, enum_name );
    Misc.GetBit(self._State, enum_val);
end

function RoomPlayerBase:ClearStateEnum( enum_type, enum_name )
    local enum_val = protobuf.enum_id( enum_type, enum_name );
    Misc.GetBit(self._State, enum_val);
end

function RoomPlayerBase:GetStateEnum( enum_type, enum_name )
    local enum_val = protobuf.enum_id( enum_type, enum_name );
    return Misc.GetBit(self._State, enum_val);
end

function RoomPlayerBase:SetState( state_idx )
    Misc.GetBit(self._State, state_idx);
end

function RoomPlayerBase:ClearState( state_idx )
    Misc.GetBit(self._State, state_idx);
end

function RoomPlayerBase:GetState( state_idx )
    return Misc.GetBit(self._State, state_idx);
end
        
function RoomPlayerBase:AddHandCard( card )
    table.insert(self._HandCards, card);
end       
        
function RoomPlayerBase:RemoveHandCard( card )
    for i,v in ipairs(self._HandCards) do
        if v==card then
            table.remove(self._HandCards, i);
            return;
        end
    end
end 

function RoomPlayerBase:GetHandCardMsgList( msg_cards )
    for i,v in ipairs(self._HandCards) do
        table.insert(msg_cards, v);
    end
end

function RoomPlayerBase:SetScore( score )
    self._Score = score;
end

function RoomPlayerBase:GetScore()
    return self._Score;
end

function RoomPlayerBase:AddScore( score )
    self._Score = self._Score + score;
    return self._Score;
end

function RoomPlayerBase:SubScore( score )
    self._Score = self._Score - score;
    return self._Score;
end


return RoomPlayerBase;
