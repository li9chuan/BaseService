MsgRoom = {}

-- 构造函数;
function MsgRoom:Init()
	self._EventRegister = EventRegister.new();

    self._EventRegister:RegisterEvent( "ER",                self, self.cbEnterRoom );
    self._EventRegister:RegisterEvent( "LR",                self, self.cbLeaveRoom );
    self._EventRegister:RegisterEvent( "CPRM=>PLS",         self, self.cbCreatePrivateRoom );
    self._EventRegister:RegisterEvent( "EPRM=>PLS",         self, self.cbEnterPrivateRoom );

end

function MsgRoom:cbEnterRoom( fes_sid, msg_enter_room )


    nlinfo("MsgRoom:cbEnterRoom");
	
end

function MsgRoom:cbLeaveRoom( fes_sid, msgin )

    local uid       = msgin:rint64();
    local player    = PlayerMgr:GetPlayer(uid);
    
    if player~=nil then
        local room = RoomMgr:GetRoom(player.RoomID);
        
        if room~=nil then
            room:LeaveRoom(uid);
        end
    end
    nlinfo("MsgRoom:cbLeaveRoom");
	
end

function MsgRoom:cbCreatePrivateRoom( sch_sid, msgin )
    local uid           = msgin:rint();
    local prv_room_id   = msgin:rint();
    local pls_sid       = msgin:rint();
    local room_type     = msgin:rstring();

    nlinfo("MsgRoom:cbCreatePrivateRoom");
    
    nlinfo(uid);
    nlinfo(prv_room_id);
    nlinfo(pls_sid);
    nlinfo(room_type);
	
    RoomMgr:CreatePrivateRoom(uid, prv_room_id, room_type);
end

function MsgRoom:cbEnterPrivateRoom( sch_sid, msgin )
    local uid           = msgin:rint();
    local pls_sid       = msgin:rint();
    local prv_room_id   = msgin:rint();
    local room_type     = msgin:rstring();
    
    nlinfo("MsgRoom:cbEnterPrivateRoom");
    
    nlinfo(uid);
    nlinfo(prv_room_id);
    nlinfo(pls_sid);
    nlinfo(room_type);
    
    if GetServiceID()~=pls_sid then
        nlwarning("GetServiceID()~=pls_sid");
        
    else
        RoomMgr:EnterPrivateRoom(uid, prv_room_id, room_type);
    end

end

--释放函数
function MsgRoom:Release()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgRoom;
