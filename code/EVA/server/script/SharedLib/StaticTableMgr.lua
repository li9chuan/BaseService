StaticTableMgr = {}

function StaticTableMgr:Init()
	
    local ConfigPath = Misc.GetBasePath() .. "/script/DataTable/";
    
    local json_text     = JsonUtil.file_load(ConfigPath.."RoomConfig.json")
    self._RoomConfig    = Json.decode(json_text)
    
    
end


function StaticTableMgr:GetRoomConfigXml( room_type )
    return self._RoomConfig[tostring(room_type)];
end


return StaticTableMgr;

