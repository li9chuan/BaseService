RoomMgr = {}

function RoomMgr:Init()
    
    self._RoomIDPool            = {};
    self._RoomsInfo             = MapMap:new();  -- { room_type, {prv_room_id, room_ins} }
    
    self:__InitRoomID();

end


function RoomMgr:CreatePrivateRoom( uid, msg_cpr )
    
    local player = PlayerInfoMgr:GetPlayerInfo(uid);
    
    if player~=nil then
        if player.PrvRoomID==0 then
            
            local room_type     = msg_cpr.room_type;
            local new_room_id   = self:__NewRoomID(room_type);
            
            if new_room_id>0 then
                
                local room_info = RoomInfo:new();
                room_info.PlayerID      = uid;
                room_info.PrvRoomID     = new_room_id;
                room_info.RoomType      = room_type;

                if PLSInfoMgr:IsInPLS(room_type, player.ConPLS) then
                    room_info.PLSID = player.ConPLS;
                else
                    room_info.PLSID = PLSInfoMgr:AllocPLS(room_type);
                end
                
                player.PrvRoomID    = new_room_id;
                player.RoomType     = room_type;
                
                self._RoomsInfo:Insert(room_type, new_room_id, room_info);
        
                local msgout = CMessage("CPRM=>PLS");
                msgout:wint(uid);
                msgout:wint(new_room_id);
                msgout:wint(room_info.PLSID);
                msgout:wtable(msg_cpr);
                
                BaseService:Send( player.ConPLS, msgout );
            end
        else
            local msgout = CMessage("EPRM=>PLS");
            msgout:wint(uid);
            msgout:wint(player.ConPLS);
            msgout:wint(player.PrvRoomID);
            msgout:wstring(player.RoomType);

            BaseService:Send( player.ConPLS, msgout );
        end
    end
end

function RoomMgr:EnterPrivateRoom( uid, msg_epr )
    
    local player = PlayerInfoMgr:GetPlayerInfo(uid);
    
    if player~=nil then
        if player.PrvRoomID==0 then
            
            local room_type     = msg_epr.room_type;
            local room_id       = msg_epr.room_id;
            local room = self._RoomsInfo:Find(room_type, room_id);
            if room~=nil then
                
                player.PrvRoomID    = room_id;
                player.RoomType     = room_type;
                
                local msgout = CMessage("EPRM=>PLS");
                msgout:wint(uid);
                msgout:wint(room.PLSID);
                msgout:wint(room_id);
                msgout:wstring(room_type);

                BaseService:Send( player.ConPLS, msgout );
            end
        else
            local msgout = CMessage("EPRM=>PLS");
            msgout:wint(uid);
            msgout:wint(player.ConPLS);
            msgout:wint(player.PrvRoomID);
            msgout:wstring(player.RoomType);

            BaseService:Send( player.ConPLS, msgout );
        end
    end
end

function RoomMgr:ResetPrivateRoom( uid, prv_rmtp, prv_rmid )
    local player = PlayerInfoMgr:GetPlayerInfo(uid);
    
    if player~=nil then
        if player.PrvRoomID~=prv_rmid or player.RoomType~=prv_rmtp then
            nlwarning(" player.PrvRoomID~=prv_rmid or player.RoomType~=prv_rmtp ");
            return;
        end
           
        player.PrvRoomID    = 0;
        player.RoomType     = "";
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


