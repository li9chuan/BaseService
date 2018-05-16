local BasePath = Misc.GetBasePath() .. "/script/";
package.path = BasePath .. "_PLS/?.lua;" .. BasePath .. "Framework/?.lua;";


require("InitFramework")


print("-=======DBSubStart==========-");


--DBSubProc = require("DBSubProc");
--sub_proc = DBSubProc:new();


--DBProc = require("DBProc");
--db_proc = DBProc:new();
--db_proc:LuaTest();


function ThreadInit( thread_handle, params )

    print("Lua ThreadInit:".. thread_handle);

    PrintTable(params);

end

function ThreadUpdate()


end