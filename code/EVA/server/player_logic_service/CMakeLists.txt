FILE(GLOB SRC 						*.cpp *.h)
FILE(GLOB SRC_LOGIN                 login/*.h login/*.cpp)
FILE(GLOB SRC_LUA                   lua/*.h lua/*.cpp)
FILE(GLOB SRC_PLAYER                player/*.h player/*.cpp)
FILE(GLOB SRC_PROC_MSG              process_msg/*.h process_msg/*.cpp)
FILE(GLOB SRC_CHAT                  chat/*.h chat/*.cpp)


SOURCE_GROUP("Login"		FILES ${SRC_LOGIN})
SOURCE_GROUP("Lua"			FILES ${SRC_LUA})
SOURCE_GROUP("Player"		FILES ${SRC_PLAYER})
SOURCE_GROUP("ProcessMsg"	FILES ${SRC_PROC_MSG})
SOURCE_GROUP("Chat"			FILES ${SRC_CHAT})

ADD_EXECUTABLE(player_logic_service WIN32 ${SRC} ${SRC_LOGIN} ${SRC_LUA}
										  ${SRC_PLAYER} ${SRC_PROC_MSG} ${SRC_CHAT} )

INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${TOLUA_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(  player_logic_service
						${LIBXML2_LIBRARIES}
						${LUA_LIBRARIES}
						${TOLUA_LIBRARIES}
						${PROTOBUF_LIBRARIES}
						nelmisc
						nelnet
						gameshare
						servershare
						eva_adminmodules)

NL_DEFAULT_PROPS(player_logic_service "Base, Services: Player Logic Service (PLS)")
NL_ADD_RUNTIME_FLAGS(player_logic_service)

#ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})

IF(WITH_PCH AND NOT MINGW) # FIXME: PCH too large (> 130MB), crashes cc1plus under MinGW
  ADD_NATIVE_PRECOMPILED_HEADER(player_logic_service ${CMAKE_CURRENT_SOURCE_DIR}/stdpch.h ${CMAKE_CURRENT_SOURCE_DIR}/stdpch.cpp)
ENDIF(WITH_PCH AND NOT MINGW)

INSTALL(TARGETS player_logic_service RUNTIME DESTINATION sbin COMPONENT services)
