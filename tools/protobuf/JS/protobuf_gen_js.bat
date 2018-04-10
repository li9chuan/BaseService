del .\protobuf\protofile\*.proto /f /a /q
copy ..\*.proto .\protobuf\protofile\

call pb-egret generate

del .\protobuf\protofile\*.proto /f /a /q
pause

