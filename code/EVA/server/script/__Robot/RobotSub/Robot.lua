local Robot = class("Robot")

function Robot:ctor()
	
    
    self.Fsm                = FSMRobot:new();
    self.Fsm:Init(self);
    
end

function Robot:Update()
    self:Login();
    self.Fsm:TickUpdate();
end

function Robot:Login()
    
    local login_params = "Channel=REG&GameType=RM_DDZ&User=hello123&AppName=WX_5E8A"

    local http_res  = Net.HttpPost("http://127.0.0.1/www/login/login_test.php", login_params);


    local http_tb = Json2Table(http_res);

    PrintTable(http_tb)


    self.Net                = bin_types.LuaCallbackClient.NewInstance("tcp");
    self.Net:Connect("127.0.0.1:10240");

    
    local msg = CMessage("LOGIN");

    local proto_msg = {
        UID         = http_tb.UID,
        Channel     = "REG",
        RoomType    = "RM_DDZ",
        AppName     = "WX_5E8A",
        User        = "hello123",
        NonceStr    = http_tb.NonceStr,
        Timestamp   = http_tb.Timestamp,
        Token       = http_tb.Token,
    }

    local code      = protobuf.encode("PB.MsgLogin", proto_msg);
    msg:wstring(code);


    self.Net:Send( msg )

end


return Robot;
