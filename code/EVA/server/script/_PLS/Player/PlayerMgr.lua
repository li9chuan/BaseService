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
    return self.playerMap[_uid];
end

function PlayerMgr:LoadDBPlayer( _uid )
    
    local tb_playerinfo = DBMgr:GetPlayerInfo(_uid);
    
    if tb_playerinfo==nil then
        if DBMgr:CreatePlayer(_uid)==true then
            tb_playerinfo = DBMgr:GetPlayerInfo(_uid);
        end
    end
    
    if tb_playerinfo~=nil then
        local player_helper    = PlayerHelper:new();
        
        player_helper.UID               = _uid;
        player_helper.PlayerDataHelper  = tb_playerinfo;
        
        
        self.playerMap[_uid] = player_helper;
    end
    
    return self.playerMap[_uid];
end

function PlayerMgr:RemovePlayer( _uid )
    self.playerMap[_uid] = nil;
end

return PlayerMgr
