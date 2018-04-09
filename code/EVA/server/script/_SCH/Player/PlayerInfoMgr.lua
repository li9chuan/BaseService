PlayerInfoMgr = {}

-- 初始化函数
function PlayerInfoMgr:Init()
	self.PlayerInfoMap      = {};
    
    print("PlayerInfoMgr:Init");
end

function PlayerInfoMgr:GetPlayerInfo( _uid )
    if self.PlayerInfoMap[_uid] ~= nil then
        return self.PlayerInfoMap[_uid];
    end
    return nil;
end

function PlayerInfoMgr:CreatePlayerInfo( _uid )

    if self.PlayerInfoMap[_uid] ~= nil then
        return self.PlayerInfoMap[_uid];
    end
    
    local player = PlayerInfo:new();
    player.UID = _uid;
    self.PlayerInfoMap[_uid] = player;
    
    return self.PlayerInfoMap[_uid];
end

function PlayerInfoMgr:RemovePlayerInfo( _uid )
    
    if self.PlayerInfoMap[_uid] ~= nil then
        
        self.PlayerInfoMap[_uid]:OnRelease();
        self.PlayerInfoMap[_uid] = nil;

    end

end

function PlayerInfoMgr:RemovePLS( pls_sid )

    local newT = {} 
    
	for _,v in pairs(self.PlayerInfoMap) do
        if v.ConPLS~=pls_sid then
            newT[v.UID] = v;
        end
    end 
    
    self.PlayerInfoMap = newT;
   
    print("PlayerInfoMgr.RemovePLS:"..pls_sid);
end

function PlayerInfoMgr:OnRelease()
    
	for _,v in pairs(self.PlayerInfoMap) do
        v:OnRelease();
    end 
    
    self.PlayerInfoMap = nil;
end

return PlayerInfoMgr
