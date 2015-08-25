FILE(GLOB SRC *.cpp *.h)

ADD_EXECUTABLE(luafun_gen ${SRC})

TARGET_LINK_LIBRARIES(luafun_gen nelmisc)

NL_DEFAULT_PROPS(luafun_gen "MT, Tools: LuaFunctionGen")
NL_ADD_RUNTIME_FLAGS(luafun_gen)

#INSTALL(TARGETS luafun_gen RUNTIME DESTINATION ${NL_BIN_PREFIX} COMPONENT tools)
#INSTALL(FILES luafun_gen.cfg DESTINATION ${NL_SHARE_PREFIX}/luafun_gen COMPONENT tools)
