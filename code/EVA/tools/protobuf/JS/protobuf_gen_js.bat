del .\protobuf\protofile\*.proto /f /a /q
copy ..\*.proto .\protobuf\protofile\
rd js_out /s /q
md js_out

pb-egret generate

rem protoc-3.2.0rc2 --js_out=import_style=commonjs,binary:./js_out define_pro.proto define_attrib.proto msg_client.proto


del .\protobuf\protofile\*.proto /f /a /q

pause

