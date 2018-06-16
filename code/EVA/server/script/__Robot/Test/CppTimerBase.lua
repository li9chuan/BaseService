
local CppTimerBase = class("CppTimerBase")

function CppTimerBase:ctor()
	self.val = 0; 
    self._TimerHandle           = TimerMgr:AddTimer( 5000, self, self.TickUpdate);
    
end

function CppTimerBase:TickUpdate()
    --nlinfo("RoomBase:TickUpdate");
    self:BaseTickUpdate();
end

function CppTimerBase:BaseTickUpdate()
    --nlinfo("RoomBase:BaseTickUpdate");
    self._TimerHandle = TimerMgr:AddTimer( 5000, self, self.TickUpdate);
    
    self.val = self.val+1;
    print("======"..self.val.."   _TimerHandle:"..self._TimerHandle);
    
    
end

--释放函数
function CppTimerBase:Release()
    self.Timer =  nil;
end


return CppTimerBase;
