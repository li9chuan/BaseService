copy ..\*.proto .
protogen -i:.\define_sys.proto -i:.\define_attrib.proto -i:.\define_pro.proto -i:.\msg_player.proto -i:.\msg_c2s.proto -i:.\msg_s2c.proto -o:.\proto\csharp\unity\MessageDefine.cs -p:detectMissing -ns:ZhiWa


cd .\proto\csharp\unity
call todll_release.bat
