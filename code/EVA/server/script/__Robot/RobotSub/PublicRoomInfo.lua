local PublicRoomInfo = class("PublicRoomInfo")

function PublicRoomInfo:ctor()
	
    self.RoomType       = "";
    self.RoomID         = -1;

    self.RoomRobots     = {};
end



return PublicRoomInfo;
