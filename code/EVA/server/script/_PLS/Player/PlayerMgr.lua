PlayerMgr = {}

-- ³õÊ¼»¯º¯Êý
function PlayerMgr:Init()
	self.PlayerMap      = {};
end

function PlayerMgr:GetPlayer( _uid )
    if self.PlayerMap[_uid] ~= nil then
        return self.PlayerMap[_uid];
    end
    return nil;
end

function PlayerMgr:SetPlayer( _uid, _player )
    if _uid ~= _player.UID then  assert()  end;
    self.PlayerMap[_uid] = _player;
end

function PlayerMgr:RemovePlayer( _uid )
    self.PlayerMap[_uid] = nil;
end

return PlayerMgr
