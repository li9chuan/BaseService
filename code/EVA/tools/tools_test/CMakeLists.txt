FILE(GLOB SRC *.cpp *.h)
FILE(GLOB SRC_THREAD thread/*.h thread/*.cpp)

SOURCE_GROUP("SrcThread" FILES ${SRC_THREAD})

INCLUDE_DIRECTORIES(${SERVER_INCLUDE})

ADD_EXECUTABLE(tools_test ${SRC} ${SRC_THREAD} )

TARGET_LINK_LIBRARIES(tools_test ${PROTOBUF_LIBRARIES} ${LIBXML2_LIBRARIES}
							gameshare servershare nelmisc nelnet)
							
NL_DEFAULT_PROPS(tools_test "MT, Tools: ToolsTest")
NL_ADD_RUNTIME_FLAGS(tools_test)

