FILE(GLOB SRC *.cpp *.h)
FILE(GLOB SRC_M_LOGIN    module_login/*.h module_login/*.cpp)

SOURCE_GROUP("ModuleLogin" FILES ${SRC_M_LOGIN})

ADD_EXECUTABLE(login_service WIN32 ${SRC} ${SRC_M_LOGIN})

INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR} ${MYSQL_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})

IF(FREEBSD)
ADD_PLATFORM_FLAGS("-lz")
ENDIF(FREEBSD)

TARGET_LINK_LIBRARIES(login_service
						${LIBXML2_LIBRARIES}
						${PROTOBUF_LIBRARIES}
						${MYSQL_LIBRARIES}
						${ZLIB_LIBRARIES}
						nelmisc
						nelnet
						gameshare
						servershare
						eva_adminmodules)

NL_DEFAULT_PROPS(login_service "Base, Services: Login Service (LS)")
NL_ADD_RUNTIME_FLAGS(login_service)

ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})

INSTALL(TARGETS login_service RUNTIME DESTINATION sbin COMPONENT ls)
#INSTALL(FILES login_service.cfg common.cfg DESTINATION etc/EVA COMPONENT ls)
