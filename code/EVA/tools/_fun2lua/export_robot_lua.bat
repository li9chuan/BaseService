copy  ..\robot\lua\lua_function.h lua_function.h
tolua.exe -n lua_function -o ..\robot\lua\auto_lua_callback.cpp  export_lua_robot.pkg
del lua_function.h

pause

