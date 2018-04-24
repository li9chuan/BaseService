local RoomDdz = class("RoomDdz",RoomBase)

-- 构造函数;
function RoomDdz:ctor( Data )

    RoomDdz.super.ctor(self );
    print("RoomDdz:ctor");

end

-- 玩家加入房间
function RoomDdz:JoinRoom( player )



end

-- 玩家离开房间
function RoomDdz:LeaveRoom( uid )
    
	
    
end


-- 初始化，子类可重写
function RoomDdz:OnInit()
    print("RoomDdz:OnInit");
end


--释放函数
function RoomDdz:Release()
    
end


return RoomDdz;
