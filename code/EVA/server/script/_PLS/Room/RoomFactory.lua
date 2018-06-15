RoomFactory = {}

function RoomFactory:Init()
	
    self.RoomTypes                      = {};
    self.RoomTypes["RM_DDZ"]            = require("Games/PokerDdz/RoomDdz");

end

function RoomFactory:CreateRoom( room_type )
    local room_class = self.RoomTypes[room_type];
    if room_class~=nil then
        local room_ins = room_class:new(room_type);
        room_ins:Init(room_type, 1000);
        return room_ins;
    end
    return nil;
end


