FILE(GLOB SRC 						*.cpp *.h)

ADD_EXECUTABLE(player_logic_service WIN32 ${SRC} )

INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${PBC_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(  player_logic_service
						${LIBXML2_LIBRARIES}
						${LUA_LIBRARIES}
						${PROTOBUF_LIBRARIES}
						${PBC_LIBRARY}
						${LIBEVENT_LIBRARY}
						nelmisc
						nelnet
						servershare
						eva_adminmodules)

NL_DEFAULT_PROPS(player_logic_service "Base, Services: Player Logic Service (PLS)")
NL_ADD_RUNTIME_FLAGS(player_logic_service)

#ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})

IF(WITH_PCH AND NOT MINGW) # FIXME: PCH too large (> 130MB), crashes cc1plus under MinGW
  ADD_NATIVE_PRECOMPILED_HEADER(player_logic_service ${CMAKE_CURRENT_SOURCE_DIR}/stdpch.h ${CMAKE_CURRENT_SOURCE_DIR}/stdpch.cpp)
ENDIF(WITH_PCH AND NOT MINGW)

INSTALL(TARGETS player_logic_service RUNTIME DESTINATION sbin COMPONENT services)
