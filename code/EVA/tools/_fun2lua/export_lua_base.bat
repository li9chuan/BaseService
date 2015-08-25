copy  ..\..\server\server_share\lua_base_function.h .
tolua.exe -n lua_base_function -o ..\..\server\server_share\auto_lua_base_callback.cpp  export_lua_base.pkg
del lua_base_function.h

pause

