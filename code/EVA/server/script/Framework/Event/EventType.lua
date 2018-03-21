--========================================================= 
-- 消息类型
--=========================================================

EventType = 
{
	-- 游戏内部事件消息 格式:[EVENT_DO_XXXXX]
	EVENT_DO_LOGIN		                 = "EVENT_DO_LOGIN";			          -- 执行登录状态切换


	-- 游戏大厅网络消息 格式:[NET_TO_XXXX]
	NET_DO_LOGIN		        		 = "LOGIN";								  -- 游戏登录消息;
	NET_DO_SYNCDATA		        		 = "SyncData";							  -- 游戏同步角色信息;
	NET_DO_TOKEN_ERROR					 = "LTERROR";							  -- TOKEN错误重新验证;
	NET_DO_S_CREATEROLE         		 = "PLY_NULL";							  -- 服务器回执需创建角色;
	NET_DO_C_CREATEROLE         		 = "CT_PLY";							  -- 游戏创建角色消息;
	NET_DO_S_SYNCLOGINPLAYER    		 = "LPLY";				     			  -- 服务器返回登陆玩家基础信息


}



