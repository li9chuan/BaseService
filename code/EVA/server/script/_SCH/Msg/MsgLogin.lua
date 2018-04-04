--========================================================= 
-- SCH   负责分配到一个PLS
--=========================================================

local MsgLogin = class("MsgLogin")

-- 构造函数;
function MsgLogin:ctor( Data )

	self._EventRegister = EventRegister.new();
	
    --  服务器间消息
    self._EventRegister:RegisterEvent( "AuthOk",  self, self.DispatchPLS );
	
end

-- 分配PLS
function MsgLogin:DispatchPLS( fes_id, proto_buf )

	local MsgData = protobuf.decode("PB_MSG.MsgData", proto_buf)
    
	print(MsgData.ext640);
    
    local uid = MsgData.exe640;
    ClientMgr:RemoveClient(uid);
    
end

--释放函数
function MsgLogin:OnRelease()
    self._EventRegister:UnRegisterAllEvent();
end


return MsgLogin;
