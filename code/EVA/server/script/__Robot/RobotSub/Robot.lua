local Robot = class("Robot")

function Robot:ctor()
	
    self.Data               = RobotData:new();
    self.Fsm                = FSMRobot:new();
    self.Fsm:Init(self);
    
    self.Net        = bin_types.LuaCallbackClient.NewInstance("tcp");

end

function Robot:Init( net_handle )
    self.Net:SetHandle(net_handle);
end

function Robot:Update()
    self.Net:Update();

    self.Fsm:TickUpdate();

    if self.GameFsm~=nil then
        self.GameFsm:TickUpdate();
        self.Game:Update();

    end
end

function Robot:GetHandle()
    return self.Net:GetHandle();
end

function Robot:Connected()
    return self.Net:Connected();
end

-- ¿ªÊ¼ÓÎÏ·Âß¼­²âÊÔ
function Robot:StartGameTest()
    if self.Data.Game == "RM_DDZ"	then
        self.Game                   = RobotGameDdz:new();
        self.Game.Robot             = self;
        self.GameFsm                = FSMDdz:new();
        self.GameFsm:Init(self);
    end
end

function Robot:Login()
    
    local login_url     = "http://127.0.0.1/www/login/login_test.php";
    local login_params  = "?Channel=REG&GameType="..self.Data.Game.."&User="..self.Data.User.."&AppName=WX_5E8A";
    local http_res      = Net.HttpGet(login_url..login_params);
    
    if http_res==nil or #http_res<20 then
        nlwarning("http login error")
        return
    end
    
    local http_tb       = Json2Table(http_res);

    PrintTable(http_tb)

    self.Net:Connect("127.0.0.1:9999");

    if self.Net:Connected() then

        local proto_msg = {
            UID         = http_tb.UID,
            Channel     = "REG",
            RoomType    = "RM_DDZ",
            AppName     = "WX_5E8A",
            User        = self.Data.User,
            NonceStr    = http_tb.NonceStr,
            Timestamp   = http_tb.Timestamp,
            Token       = http_tb.Token,
        }

        self:Send( "LOGIN", "PB.MsgLogin", proto_msg )

        nlinfo("Login :"..self.Data.User);
        return true;
    else
        nlwarning("Connect Error :"..self.Data.User);
    end

    return false;
end

function Robot:Send( msgname, proto_type, proto_msg )
    local code  = protobuf.encode(proto_type, proto_msg);
    local msg   = CMessage(msgname);
    msg:wstring(code);
    self.Net:Send( msg );
end

function Robot:cbSyncPlayerInfo( msgin )
    local player_info = msgin:rpb("PB.MsgPlayerInfo");

    nlinfo("Robot:cbSyncPlayerInfo");
    PrintTable(player_info);
end


return Robot;
