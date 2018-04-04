PlayerInfoMgr = {}

-- 初始化函数
function PlayerInfoMgr:Init()
	self.PlayerInfoMap      = {};
end

function PlayerInfoMgr:GetPlayerInfo( _uid )
    if self.PlayerInfoMap[_uid] ~= nil then
        return self.PlayerInfoMap[_uid];
    end
    return nil;
end

function PlayerInfoMgr:SetPlayerInfo( _uid, _playerinfo )
    if _uid ~= _playerinfo.UID then  assert()  end;
    self.PlayerInfoMap[_uid] = _playerinfo;
end

function PlayerInfoMgr:RemovePlayerInfo( _uid )
    self.ClientMap[_uid] = nil;
end

function PlayerInfoMgr:RemovePLS( pls_sid )

    local newT = {} 
    
	for _,v in pairs(self.PlayerInfoMap) do
        if v.PLSID~=pls_sid then
            newT[v.UID] = v;
        end
    end 
    
    self.PlayerInfoMap = newT;
    
end

return PlayerInfoMgr
