RoomMgr = {}

function RoomMgr:Init()
    
    self._RoomIDPool            = {};
    self._RoomsInfo             = {{}};
    
    self:__InitRoomID();

end


function RoomMgr:CreatePrivateRoom( uid, room_type )
    
    local player = PlayerInfoMgr:GetPlayerInfo(uid);
    
    if player~=nil then
        if player.RoomID==0 then
            
            local new_room_id = self:__NewRoomID(room_type);
            if new_room_id>0 then
                
                local room_info = RoomInfo:new();
                room_info.PlayerID  = uid;
                room_info.RoomID    = new_room_id;
                room_info.RoomType  = room_type;

                if PLSInfoMgr:IsInPLS(room_type, player.ConPLS) then
                    room_info.PLSID = player.ConPLS;
                else
                    room_info.PLSID = PLSInfoMgr:AllocPLS(room_type);
                end
                
                self._RoomsInfo[room_type] = { [new_room_id] = room_info };
        
                local msgout = CMessage("CPRM=>PLS");
                msgout:wint(uid);
                msgout:wint(new_room_id);
                msgout:wint(room_info.PLSID);
                msgout:wstring(room_type);
                
                BaseService:Send( player.ConPLS, msgout );
            end
        else
            
        end
    end
end

function RoomMgr:EnterPrivateRoom( uid, room_type, room_id )
    
    local player = PlayerInfoMgr:GetPlayerInfo(uid);
    
    if player~=nil then
        if player.RoomID==0 then
            
            local rooms = self._RoomsInfo[room_type];
            
            if rooms~=nil then
                local room = rooms[room_id];
                if room~=nil then
                    
                    local msgout = CMessage("EPRM=>PLS");
                    msgout:wint(uid);
                    msgout:wint(room.PLSID);
                    msgout:wint(room.RoomID);
                    msgout:wstring(room.RoomType);

                    BaseService:Send( player.ConPLS, msgout );
                end
            end
        else
            
        end
    end
end

function RoomMgr:__NewRoomID( room_type )

    local idpool = self._RoomIDPool[room_type];
    local new_id = 0;
    
    if idpool~=nil then
        new_id = idpool:AllocID();
    end
    
    return new_id;
end

function RoomMgr:__InitRoomID()

    for k, v in pairs(StaticTableMgr._RoomConfig) do  
        if v.match == "private" then
            self._RoomIDPool[k] = RoomIDAlloter.new();
        end
    end 

end

return RoomMgr


