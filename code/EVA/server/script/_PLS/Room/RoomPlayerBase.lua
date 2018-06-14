local RoomPlayerBase = class("RoomPlayerBase")

function RoomPlayerBase:ctor()

    self._State         = 0;
    self._HandCards     = {};
    self._Score         = 0;            -- »ý·Ö

end

function RoomPlayerBase:GetHandCount()
    return #self._HandCards;
end

function RoomPlayerBase:ClearAllState()
    self._State = 0;
end

function RoomPlayerBase:GetState( enum_val )
    
    if enum_val==nil then
        return self._State;
    end
    
    return Misc.GetBit(self._State, enum_val);
end

function RoomPlayerBase:SetState( enum_val )
    if enum_val~=nil then
        self._State = Misc.SetBit(self._State, enum_val);
    end
end

function RoomPlayerBase:ClearState( enum_val )
    if enum_val~=nil then
        self._State = Misc.ClearBit(self._State, enum_val);
    end
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
