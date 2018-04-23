# BaseService
微信小游戏服务器，房间及卡牌类游戏服务器。支持websocket及tcp连接，lua编写游戏逻辑，运行于C++服务器，跨平台。C++底层已支持多款线上产品。

使用lua编写游戏逻辑，不需要编写C++代码。



### 服务器分类：
```
naming_service              名字服务器，用于服务器间互相注册，不对外提供服务。

player_logic_service        用来均摊逻辑的服务器，游戏逻辑写在这里。

schedule_service            调度服务器，用来统一协调服务器资源，比如分配房间。

frontend_service            前端服务器，主要用于处理网络连接，消息转发，也可以缓存一部分热点数据。
```

### 配置：
每种服务器有与自己名字相同的配置文件，如frontend_service.cfg

```
AESAliasName = "fes_0";                         // 服务器别名，在运营工具中查看服务器状态时显示这个名字。
SId = 66;                                       // 服务器ID，唯一。

StartLuaScript = "_FESMain.lua";                // 脚本执行入口

Paths =
{
	"./script/_FES/",							// 脚本文件查找路径
};

```


###脚本工程：
```
server\script\DataTable                     各种配置文件的目录
server\script\Framework                     框架提供的基础工具。
server\script\SharedLib                     游戏共享库，与游戏逻辑相关的共享代码在这里。
server\script\_FES                          前端服务器代码。
server\script\_PLS                          逻辑服务器代码。
server\script\_SCH                          调度服务器代码。

```

### 脚本调用顺序

```
1.服务器启动时读取.cfg配置文件，运行对应的StartLuaScript。
2.调用其中的        function ServiceInit()
3.每帧调用          function ServiceUpdate()
4.程序退出调用      function ServiceRelease()

```


### 简单通信

server\script\_FES\_FESMain.lua

https://github.com/li9chuan/EgretWebSocket

```
function MsgLogin:ctor( Data )

    self._EventRegister = EventRegister.new();

    --  客户端消息
	self._EventRegister:RegisterEvent( "LOGIN",     self, self.CBLogin );

end

function MsgLogin:CBLogin( sock_id, msg_login )

    nlinfo( msg_login:rint32() );
    nlinfo( msg_login:rdouble() );
    nlinfo( msg_login:rstring() );
    nlinfo( msg_login:rint64() );
    nlinfo( msg_login:rint64() );

	--  将收到的 LOGIN 消息原样发回给客户端。
    msg_login:invert();
    ClientService:Send( sock_id, msg_login );

end

```

