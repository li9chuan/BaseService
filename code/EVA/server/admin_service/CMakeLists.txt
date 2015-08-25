FILE(GLOB SRC *.cpp *.h)

ADD_EXECUTABLE(admin_service WIN32 ${SRC})

INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR} ${MYSQL_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(  admin_service
						gameshare
						eva_adminmodules
						${LIBXML2_LIBRARIES}
						nelmisc 
						nelnet)

NL_DEFAULT_PROPS(admin_service "MT, Services: Admin Service (AS)")
NL_ADD_RUNTIME_FLAGS(admin_service)

ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})

INSTALL(TARGETS admin_service RUNTIME DESTINATION sbin COMPONENT services)


