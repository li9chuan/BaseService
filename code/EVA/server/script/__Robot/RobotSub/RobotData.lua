local RobotData = class("RobotData")

function RobotData:ctor()
	
    self.UID            = 0;
    self.User           = "";
    self._RoomState     = 0;
    


    -- robot ≈‰÷√ ˝æ›
    self.Game           = "";
end

function RobotData:ClearRoomState( enum_val )
    if enum_val~=nil then
        self._RoomState = Misc.ClearBit(self._RoomState, enum_val);
    end
end

function RobotData:ClearRoomAllState()
    self._RoomState = 0;
end

function RobotData:GetRoomState( enum_val )
    
    if enum_val==nil then
        return self._RoomState;
    end
    
    return Misc.GetBit(self._RoomState, enum_val);
end

function RobotData:SetRoomState( enum_val )
    if enum_val~=nil then
        self._RoomState = Misc.SetBit(self._RoomState, enum_val);
    end
end

return RobotData;
