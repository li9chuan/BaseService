StaticTableMgr = {}

function StaticTableMgr:Init()
	
    local ConfigPath = Misc.GetBasePath() .. "/script/DataTable/";
    
    local json_text     = JsonUtil.file_load(ConfigPath.."RoomConfig.json")
    self._RoomConfig    = Json.decode(json_text)

    json_text           = JsonUtil.file_load(ConfigPath.."RoomCreateCost.json")
    self._CreateCost    = Json.decode(json_text)
    
    json_text           = JsonUtil.file_load(ConfigPath.."SpecialConfig.json")
    self._SpecialCfg    = Json.decode(json_text)
    
    
end


function StaticTableMgr:GetRoomConfig( room_type )
    return self._RoomConfig[tostring(room_type)];
end

function StaticTableMgr:GetCreateCost( cost_id )
    return self._CreateCost[tostring(cost_id)];
end

function StaticTableMgr:GetSpecialCfg( room_type )
    return self._SpecialCfg[tostring(room_type)];
end

return StaticTableMgr;

