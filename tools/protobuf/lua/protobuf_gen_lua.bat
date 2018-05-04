del *.proto /f /a /q
del *.pb /f /a /q
copy ..\*.proto .

del ..\..\..\code\EVA\server\script\DataTable\*.proto /f /a /q
copy ..\*.proto ..\..\..\code\EVA\server\script\DataTable\proto\ /y

protoc-3.2.0rc2 --descriptor_set_out=./ProtoMsg.pb define_pro.proto define_attrib.proto msg_client.proto msg_service.proto msg_doudizhu.proto

copy ProtoMsg.pb ..\..\..\code\EVA\server\script\DataTable\  /y
del *.proto /f /a /q
del *.pb /f /a /q
pause

