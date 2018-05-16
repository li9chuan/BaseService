FILE(GLOB SRC *.cpp *.h)

ADD_EXECUTABLE(client_robot WIN32 ${SRC})

INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(	client_robot
						servershare)

NL_DEFAULT_PROPS(client_robot "Base, Client: ClientRobot")
NL_ADD_RUNTIME_FLAGS(client_robot)





