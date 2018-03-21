del *.proto /f /a /q
del *.pb /f /a /q
copy ..\*.proto .
rem rd js_out /s /q
rem md js_out               protoc-3.2.0rc2.exe


protoc --descriptor_set_out=./proto_msg.pb define_sys.proto

rem protoc-3.2.0rc2 --js_out=import_style=commonjs,binary:./js_out define_sys.proto msg_mahjong.proto msg_guandan.proto msg_niuniu.proto msg_runfast.proto msg_doudizhu.proto msg_doushisi.proto msg_penghu.proto msg_horse.proto msg_fish.proto define_attrib.proto mail_pro.proto friend_pro.proto define_pro.proto public_pro.proto msg_texaspoker.proto msg_slots.proto msg_blackjack.proto msg_runfast_qualifying.proto msg_data_center.proto
rem protoc-3.2.0rc2 --js_out=library=myprotos_lib.js,binary:./js_out define_sys.proto msg_mahjong.proto msg_guandan.proto msg_niuniu.proto msg_runfast.proto msg_doudizhu.proto msg_doushisi.proto msg_penghu.proto msg_horse.proto msg_fish.proto define_attrib.proto mail_pro.proto friend_pro.proto define_pro.proto public_pro.proto msg_texaspoker.proto msg_slots.proto msg_blackjack.proto msg_runfast_qualifying.proto msg_data_center.proto

del *.proto /f /a /q
pause

