PlayerInfoMgr = {}

-- 初始化函数
function PlayerInfoMgr:Init()
	self.PlayerInfoMap      = Map:new();
end

function PlayerInfoMgr:GetPlayerInfo( _uid )
    return self.PlayerInfoMap:Find(_uid);
end

function PlayerInfoMgr:SetPlayerInfo( _uid, _playerinfo )
    if _uid ~= _playerinfo.UID then  assert()  end;
    
    self.PlayerInfoMap:Insert(_uid, _playerinfo);
end

function PlayerInfoMgr:RemovePlayerInfo( _uid )
    self.PlayerInfoMap:Remove(_uid);
end

function PlayerInfoMgr:RemovePLS( pls_sid )
    self.PlayerInfoMap:ForEachRemove("PLSID", pls_sid);
end

return PlayerInfoMgr
