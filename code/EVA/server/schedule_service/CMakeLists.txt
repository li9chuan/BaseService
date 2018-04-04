FILE(GLOB SRC *.cpp *.h)

ADD_EXECUTABLE(schedule_service WIN32 ${SRC})

INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${PBC_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(	schedule_service
						eva_adminmodules
						${LIBXML2_LIBRARIES}
						${LUA_LIBRARIES}
						${PROTOBUF_LIBRARIES}
						${PBC_LIBRARY}
						${LIBEVENT_LIBRARY}
						nelmisc
						nelnet
						gameshare
						servershare)

NL_DEFAULT_PROPS(schedule_service "Base, Services: Schedule Service (SCH)")
NL_ADD_RUNTIME_FLAGS(schedule_service)

INSTALL(TARGETS schedule_service RUNTIME DESTINATION sbin COMPONENT services)
