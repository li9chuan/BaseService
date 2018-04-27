RoomFactory = {}

function RoomFactory:Init()
	
    self.RoomTypes                      = {};
    self.RoomTypes["RM_DDZ"]            = require("Games/PokerDdz/RoomDdz");

end

function RoomFactory:CreateRoom( room_type )
    local room_type = self.RoomTypes[room_type];
    if room_type~=nil then
        return room_type:new();
    end
    return nil;
end


return RoomFactory;
