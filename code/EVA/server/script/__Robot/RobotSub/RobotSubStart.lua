local BasePath = Misc.GetBasePath() .. "/script/";
package.path = BasePath .. "__Robot/RobotSub/?.lua;" .. BasePath .. "Framework/?.lua;";


require("InitFramework")

require("RobotTest")

print("-=======DBSubStart==========-");


G_ThreadHandle = 999;

function ThreadInit( thread_handle, params )

    print("Lua ThreadInit:".. thread_handle);

    print(params);
    
    G_ThreadHandle = thread_handle;


    RobotTest:Init();

end

