

local BasePath = "E:/BaseService/code/EVA/server/script/";
package.path = BasePath .. "Framework/Test/?.lua;" .. BasePath .. "Framework/?.lua;";

require("Class")
require("functions")

StateMachine = require("SimpleStateMachine");



local FSMClass = require("FSMClass");
fsm_class = FSMClass:new();

fsm_class:SwitchState("TChessStateWait","=============");







