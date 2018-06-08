local RobotData = class("RobotData")

function RobotData:ctor()
	
    self.User           = "";
    self._RoomState     = 0;
    


    -- robot ≈‰÷√ ˝æ›
    self.Game           = "";
end

function RobotData:ClearRoomState()
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
        Misc.SetBit(self._RoomState, enum_val);
    end
end

return RobotData;
