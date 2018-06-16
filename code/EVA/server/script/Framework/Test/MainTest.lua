

local BasePath = "E:/BaseService/code/EVA/server/script/";
package.path = BasePath .. "Framework/Test/?.lua;" .. BasePath .. "Framework/?.lua;";

require("Class")
require("functions")
require("TimerMgr")


StateMachine                = require("SimpleStateMachine");


local FSMClass  = require("FSMClass");
local TimerTest = require("TimerTest");



TimerMgr:Init( os.time()*1000 );


local test_timer = TimerTest:new();



while true do

    TimerMgr:Update( os.time()*1000 );
end








