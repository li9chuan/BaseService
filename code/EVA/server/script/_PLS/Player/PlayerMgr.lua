PlayerMgr = {}

-- 初始化函数
function PlayerMgr:Init()
	self.playerMap      = {};

    self.maxPlayer      = 3000;

end

function PlayerMgr:MaxPlayer()
    return self.maxPlayer;
end

function PlayerMgr:Count()
    return #self.playerMap;
end

function PlayerMgr:GetPlayer( _uid )
    if self.playerMap[_uid] ~= nil then
        return self.playerMap[_uid];
    end
    return nil;
end

function PlayerMgr:SetPlayer( _uid, _player )
    if _uid ~= _player.UID then  assert()  end;
    self.playerMap[_uid] = _player;
end

function PlayerMgr:RemovePlayer( _uid )
    self.playerMap[_uid] = nil;
end

return PlayerMgr
