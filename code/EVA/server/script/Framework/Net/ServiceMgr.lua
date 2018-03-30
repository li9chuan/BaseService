local ServiceMgr = singleton("ServiceMgr")

-- ³õÊ¼»¯º¯Êý
function ServiceMgr:Init()
	self._Session      = 0;
	self._Seed         = 0;
	self._Connect      = 0;
	self._BufferIndex  = 0;
end


return ServiceMgr
