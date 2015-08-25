set DEF_OUT=..\..\..\..\..\..\..\..\RetailProjectU3D\Assets\Plugins
..\precompile MessageDefine.dll -o:ProtobufSerializer.dll -t:ZhiWa.ProtobufSerializer

copy MessageDefine.dll %DEF_OUT%\MessageDefine.dll

copy ProtobufSerializer.dll %DEF_OUT%\ProtobufSerializer.dll

copy protobuf-net.dll %DEF_OUT%\protobuf-net.dll
 
pause