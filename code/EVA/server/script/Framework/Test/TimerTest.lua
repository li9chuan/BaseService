
local TimerTest = class("TimerTest")

function TimerTest:ctor()
	self.val = 0; 
    
    TimerMgr:AddTimer(1000, self, self.CallBack);
end

function TimerTest:CallBack()

    
    
    
    local timerid = TimerMgr:AddTimer(1000, self, self.CallBack);
    
    self.val = self.val+1;
    print("======"..self.val.."   id:"..timerid);

end

--释放函数
function TimerTest:OnRelease()
    
end


return TimerTest;
