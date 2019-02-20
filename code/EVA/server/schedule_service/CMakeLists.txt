FILE(GLOB SRC *.cpp *.h)

ADD_EXECUTABLE(schedule_service WIN32 ${SRC})

INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(	schedule_service
						#eva_adminmodules
						servershare)

NL_DEFAULT_PROPS(schedule_service "Base, Services: Schedule Service (SCH)")
NL_ADD_RUNTIME_FLAGS(schedule_service)

INSTALL(TARGETS schedule_service RUNTIME DESTINATION sbin COMPONENT services)
