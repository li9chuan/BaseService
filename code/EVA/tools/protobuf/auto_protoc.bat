set DEF_OUT=../../common/src/game_share
set PROTO_OUT=../../common/src/game_share/proto_msg

protoc.exe --cpp_out=%DEF_OUT% ./define_sys.proto
protoc.exe --cpp_out=%DEF_OUT% ./define_attrib.proto
protoc.exe --cpp_out=%DEF_OUT% ./define_pro.proto

protoc.exe --cpp_out=%PROTO_OUT% ./msg_player.proto
protoc.exe --cpp_out=%PROTO_OUT% ./msg_c2s.proto
protoc.exe --cpp_out=%PROTO_OUT% ./msg_s2c.proto


cd ../luafun_gen
luafun_gen.exe genlua_base.cfg
luafun_gen.exe genlua_pls.cfg

pause
