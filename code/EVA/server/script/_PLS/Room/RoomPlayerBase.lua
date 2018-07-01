local RoomPlayerBase = class("RoomPlayerBase")

function RoomPlayerBase:ctor()

    self._State         = 0;
    self._Score         = 0;            -- »ý·Ö
    
    self.HandCards      = {};
end

function RoomPlayerBase:GetHandCount()
    return #self.HandCards;
end

function RoomPlayerBase:ClearOneGameState()

    local clear_states = { enum.STATE_DDZ_NEWROLE, enum.STATE_DDZ_RELIEVE, enum.STATE_DDZ_GUOPAI, enum.STATE_DDZ_NONGMING, enum.STATE_DDZ_DIZHU,
    enum.STATE_DDZ_JIABEI, enum.STATE_DDZ_SELECT_JIABEI, enum.STATE_DDZ_MINGPAI,enum.STATE_DDZ_SELECT_MINGPAISTART, enum.STATE_DDZ_CONTINUE_GAME };
    self._State = ClearBits( self._State, clear_states );
    
    self.HandCards = {};
end

function RoomPlayerBase:GetState( enum_val )
    
    if enum_val==nil then
        return self._State;
    end
    
    return GetBit(self._State, enum_val);
end

function RoomPlayerBase:SetState( enum_val )
    self._State = SetBit( self._State, enum_val );
end

function RoomPlayerBase:ClearState( enum_val )
    self._State = ClearBit(self._State, enum_val);
end

function RoomPlayerBase:AddHandCard( card )
    table.insert(self.HandCards, card);
end       
        
function RoomPlayerBase:RemoveHandCard( card )
    for i,v in ipairs(self.HandCards) do
        if v==card then
            table.remove(self.HandCards, i);
            return;
        end
    end
end 

function RoomPlayerBase:GetHandCardMsgList( msg_cards )
    for i,v in ipairs(self.HandCards) do
        table.insert(msg_cards, v);
    end
end

function RoomPlayerBase:SetScore( score )
    self._Score = score;
end

function RoomPlayerBase:GetScore()
    return self._Score;
end

function RoomPlayerBase:ChangeScore( score, is_add )
    if is_add then
        self._Score = self._Score + score;
    else
        self._Score = self._Score - score;
    end
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
