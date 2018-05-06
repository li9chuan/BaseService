PlayerMgr = {}

-- 初始化函数
function PlayerMgr:Init()
	self.playerMap      = Map:new();

    self.maxPlayer      = 3000;

end

function PlayerMgr:MaxPlayer()
    return self.maxPlayer;
end

function PlayerMgr:Count()
    return self.playerMap:Count();
end

function PlayerMgr:GetPlayer( _uid )
    return self.playerMap:Find(_uid);
end

function PlayerMgr:LoadDBPlayer( _uid )
    
    local tb_playerinfo = DBMgr:LoadPlayerInfo(_uid);
    
    if tb_playerinfo==nil then
        if DBMgr:CreatePlayer(_uid)==true then
            tb_playerinfo = DBMgr:LoadPlayerInfo(_uid);
        end
    end
    
    if tb_playerinfo~=nil then
        local player_helper    = PlayerHelper:new();
        
        player_helper.UID               = _uid;
        player_helper.PlayerDataHelper  = tb_playerinfo;
        
        self.playerMap:Insert(_uid, player_helper);
        return player_helper;
    end
    
    return nil;
end

function PlayerMgr:RemovePlayer( _uid )
    self.playerMap:Remove(_uid);
end

return PlayerMgr
