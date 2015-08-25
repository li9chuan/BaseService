copy  ..\..\server\player_logic_service\lua\lua_function.h lua_function.h
tolua.exe -n lua_function -o ..\..\server\player_logic_service\lua\auto_lua_callback.cpp  export_lua.pkg
del lua_function.h

pause

