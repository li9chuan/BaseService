FILE(GLOB SRC *.cpp *.h)
FILE(GLOB SRC_RECV_MGR    receiver_mgr/*.h receiver_mgr/*.cpp)
FILE(GLOB SRC_SEND_MGR    sender_mgr/*.h sender_mgr/*.cpp)
FILE(GLOB SRC_COC    	  coc/*.h coc/*.cpp)

SOURCE_GROUP("ReceiverMgr" FILES ${SRC_RECV_MGR})
SOURCE_GROUP("SenderMgr" FILES ${SRC_SEND_MGR})
SOURCE_GROUP("COC" FILES ${SRC_COC})

ADD_EXECUTABLE(persistant_data_service WIN32 ${SRC} ${SRC_RECV_MGR} ${SRC_SEND_MGR} ${SRC_COC})

INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR} ${MYSQL_INCLUDE_DIR})

IF(FREEBSD)
ADD_PLATFORM_FLAGS("-lz")
ENDIF(FREEBSD)

TARGET_LINK_LIBRARIES(persistant_data_service
						eva_adminmodules
						servershare
						gameshare
						${LIBXML2_LIBRARIES}
						${PROTOBUF_LIBRARIES}
						nelmisc
						nelnet)

NL_DEFAULT_PROPS(persistant_data_service "Base, Services: Persistent Data Service (PDS)")
NL_ADD_RUNTIME_FLAGS(persistant_data_service)

ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})

INSTALL(TARGETS persistant_data_service RUNTIME DESTINATION sbin COMPONENT services)
