local BasePath = Utility.GetBasePath() .. "/script/";
package.path = BasePath .. "_FES/?.lua;" .. BasePath .. "Framework/?.lua;";


require("InitFramework")


print("-=======DBSubStart==========-");


DBSubProc = require("DBSubProc");
sub_proc = DBSubProc:new();


--DBProc = require("DBProc");
--db_proc = DBProc:new();
--db_proc:LuaTest();


--[[MysqlConn = bin_types.MysqlConn.NewInstance();

local conn_info = { "localhost", "root", "", "d_mt_player", 3306 };
MysqlConn:Connect( conn_info );

local MysqlStmt = bin_types.MysqlStmt.NewInstance("CALL _t_select_playerinfo(?)");

local buff = "a\0000000ffff3e";

MysqlStmt:SetUint64(3);
--MysqlStmt:SetBlob(buff,#buff);

local MysqlResult = bin_types.MysqlResult.NewInstance();

MysqlConn:Query( MysqlStmt, MysqlResult );

MysqlResult:GetUint64();
local blob = MysqlResult:GetBlob();

print("====="..blob.."---"..#blob);--]]

