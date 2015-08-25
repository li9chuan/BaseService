FILE(GLOB SRC *.cpp *.h)

ADD_EXECUTABLE(service_daemon ${SRC})

TARGET_LINK_LIBRARIES(service_daemon nelmisc)

NL_DEFAULT_PROPS(service_daemon "MT, Tools: ServiceDaemon")
NL_ADD_RUNTIME_FLAGS(service_daemon)

#INSTALL(TARGETS service_daemon RUNTIME DESTINATION ${NL_BIN_PREFIX} COMPONENT tools)
#INSTALL(FILES service_daemon.cfg DESTINATION ${NL_SHARE_PREFIX}/service_daemon COMPONENT tools)
