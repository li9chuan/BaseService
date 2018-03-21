local CallbackService = class("CallbackService")

-- 初始化函数
function CallbackService:Init()
	self._Connect      = 0;
end

-- 监听端口
function CallbackService:Listen( Host , Port )


end


-- 发送网路消息
function CallbackService:SendMessage( Name , ProtoBuffer )
	-- 检查网络连接 --
	if self._Connect ~= 1 then
		return;
	end
	
--[[

	-- 填充发送缓存区 --
	local NetMessage Message = LuaFramework.NetMessage.New();
	Message:WriteName( Name );
	Message:WriteBytes( Session:SerializeToString() );
	if Buffer ~= nil then
	Message:WriteBytes( Buffer );
	else
	Message:WriteInt32( 0 );
	end
	
	local Res = NetWorkHelper:SendMessage( Message );
	if Res == 0 then
	self._BufferIndex = self._BufferIndex + 1;
	else
	self:ForceConnectServer();
	end
	
	]]
end

return CallbackService

