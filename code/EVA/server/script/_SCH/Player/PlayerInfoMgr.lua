PlayerInfoMgr = {}

-- 初始化函数
function PlayerInfoMgr:Init()
	self.PlayerInfoMap      = Map:new();
    
    nlinfo("PlayerInfoMgr:Init");
end

function PlayerInfoMgr:GetPlayerInfo( _uid )
    return self.PlayerInfoMap:Find(_uid);
end

function PlayerInfoMgr:CreatePlayerInfo( _uid )

    local player = self.PlayerInfoMap:Find(_uid);
    if player~= nil then
        return player;
    else
        player = PlayerInfo:new();
        player.UID = _uid;
        self.PlayerInfoMap:Insert(_uid, player);
    end

    return player;
end

function PlayerInfoMgr:RemovePlayerInfo( _uid )
    
    local player = self.PlayerInfoMap:Find(_uid);
    
    if player ~= nil then
        player:Release();
        self.PlayerInfoMap:Remove(_uid);
    end
end

function PlayerInfoMgr:RemovePLS( pls_sid )
    self.PlayerInfoMap:ForEachRemove("ConPLS", pls_sid);
    nlinfo("PlayerInfoMgr.RemovePLS:"..pls_sid);
end

function PlayerInfoMgr:Release()
    self.PlayerInfoMap:ForEach( function(_,v) v:Release(); end );
    self.PlayerInfoMap = nil;
end

return PlayerInfoMgr
