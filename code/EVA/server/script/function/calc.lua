calc = {}		--  公式

-- 重置科技分配点数消耗金钱
calc.ResetTechCostRMB = function( level, count )

	if( level<25 ) then
		return 0;
	end
	
	local cost_rmb = 50+count*10;
	
	if( cost_rmb>200 ) then
		cost_rmb = 200;
	end

	return cost_rmb;
end

-- 匹配分数
calc.MatchingScore = function( count , score )

	local PvPScore = 200
	
	PvPScore = count * 50
	
	if ( PvPScore > 200 ) then
		 PvPScore = 200
	end
	
	if ( PvPScore < -200 ) then
		 PvPScore = -200
	end
	
	return PvPScore
end

-- 匹配难度
-- 1 普通
-- 2 困难
-- 3 噩梦
calc.MatchingNpc = function( count , IsFirst )

	local diff = 1
	
	-- 首次匹配
	if IsFirst == 1 then
		return diff
	end
	
	local value = math.random(10000)
	
	-- 连胜6次以上;
	if count >= 6  then
		if value >= 5000 then
			return 1
		elseif value >= 3000 then
			return 2
		else
			return 3
		end
	end

	-- 连胜3次以上;
	if count >= 3 then
		if value >= 6000 then
			return 1
		else
			return 2
		end
	end
	
	return diff
end

calc.GuardLevelItem = function( cur_level , need_num , need_level )
	
	local NeedDebrisNum = 1;
	local TestNeedNum   = 0;

	TestNeedNum   = ( cur_level - 1 ) / need_level;
	need_num      = need_num + TestNeedNum * need_num
	
	return need_num
end

calc.GuardLevelMoney = function( init_money , next_money , level_need , level_cur )

	local upgrade_time = 0;
	local money        = 0;
	
	if level_need > 0 then
		upgrade_time =(level_cur-1)/level_need ;
	end
	
	money = init_money+next_money* math.floor(upgrade_time);
	return money;
end

--castle_monster_num 部署怪物数量点;
--killed_num         杀死怪物数量点;
--target_pvp_score   目标积分;
--self_pvp_score     自己积分;
calc.CastleScore = function( castle_monster_num , killed_num , target_pvp_score , self_pvp_score )

	local level_score = 0;
	local lose_score  = 0;
	local win_score   = 0;
	
	-- 错误处理;
	if killed_num > castle_monster_num then
	   killed_num = 0
	end

	if castle_monster_num > 0 and killed_num > 0 then
		level_score = 100/castle_monster_num*killed_num/33.3;
	else
		level_score = 3;
	end
	
	win_score  =(level_score*5*((target_pvp_score+1)/(self_pvp_score+1)));
	lose_score =(win_score*0.9);
	
	return win_score,lose_score;
end

calc.CastleRobotScore = function( target_pvp_score , self_pvp_score )

	local level_score = 1;
	local lose_score  = 0;
	local win_score   = 0;
	
	win_score  = (level_score*5*((target_pvp_score+1)/(self_pvp_score+1)));
	lose_score = (win_score*0.9);
	
	return win_score,lose_score;
end

calc.CastleMoney = function( target_money , self_money , target_score , self_score , score_level , castle_monster_num , killed_num )

	-- PVP理论抢夺金钱;
	local level_money = 0;
	local win_money   = 0;
	local lose_money  = 0;
	local money_coe   = 0;
	
	-- 错误处理;
	if killed_num > castle_monster_num then
	   killed_num = 0
	end
	
	level_money = target_money*((100-(self_score-target_score))/100+1)/10
	
	-- 胜利金钱;
	if( castle_monster_num > 0 ) then
		win_money = level_money / 100 * ( 28 + 72 / castle_monster_num * killed_num );
	else
		win_money = level_money / 100 * ( 28 + 72 / 3 * 3 );
	end
	
	-- >=100;
	if( target_score - self_score >= 100 ) then
		money_coe = ( target_score - self_score ) / 10*0.05;
		if( money_coe >= 0.5 ) then
		money_coe = 0.5;
		end
		win_money  = win_money * ( 1 + money_coe );					--计算胜利方金钱;
		lose_money = level_money * ( 1 + money_coe ); 				--计算失败方金钱;
	elseif( self_score - target_score >= 100 ) then
		money_coe = ( self_score - target_score ) /10*0.05;
		if( money_coe >= 0.5 ) then
		money_coe = 0.5
		end
		win_money = win_money*( 1 - money_coe );					--计算胜利方金钱;
		lose_money= level_money * (1 - money_coe);					--计算失败方金钱;
	else
		lose_money = level_money;
	end
	
	return win_money,lose_money;									--计算失败方金钱;
end


calc.CastleSoul = function( target_soul , self_soul , target_score , self_score , score_level , castle_monster_num , killed_num )

	-- PVP理论抢夺魂晶;
	local level_soul = 0;
	local win_soul   = 0;
	local lose_soul  = 0;
	local soul_coe   = 0;
	
	-- 错误处理;
	if killed_num > castle_monster_num then
	   killed_num = 0
	end
	
	level_soul = target_soul*((100-(self_score-target_score))/100+1)/10;
	
	-- 胜利魂晶;
	if( castle_monster_num > 0 ) then
		win_soul = level_soul / 100 * ( 28 + 72 / castle_monster_num * killed_num );
	else
		win_soul = level_soul / 100 * ( 28 + 72 / 3 * 3 );
	end
	
	-- >=100;
	if( target_score - self_score >= 100 ) then
		soul_coe = ( target_score - self_score ) / 10*0.05;
		if(soul_coe) then
		soul_coe = 0.5;
		end
		win_soul  = win_soul * ( 1 + soul_coe );				--计算胜利方魂晶;
		lose_soul = level_soul * ( 1 + soul_coe ); 				--计算失败方魂晶;
	elseif( self_score - target_score >= 100 ) then
		soul_coe = ( self_score - target_score ) /10*0.05;
		if( soul_coe >= 0.5 ) then
		soul_coe = 0.5
		end
		win_soul = win_soul*( 1 - soul_coe );					--计算胜利方魂晶;
		lose_soul= level_soul * (1 - soul_coe );				--计算失败方魂晶;
	else
		lose_soul = level_soul;
	end
	
	return win_soul,lose_soul;									--计算失败方魂晶;
end

calc.Exp2Item = function( _exp )
	local item_num = math.ceil(_exp/200);
	if(item_num<1) then
		item_num = 1;
	end
	return 12002001,item_num;
end

calc.CastleOutPutMoney = function( fight_power , elapse_time ) 
	
	local output_money   	= 0;
	local output_hour    	= 0;
	local output_percent 	= 0;
	local output_max     	= 0;
	local output_hour_money = 0;
	
	output_money 	  = fight_power / 50 + 20;
	output_hour 	  = math.floor( elapse_time / 60 );
	output_hour_money = fight_power / 2000 + 60 * 20;
	output_max        = output_hour_money * 24;
	
	-- 禁止循环次数多过;
	if output_hour > 48 then
	   output_hour = 48
	end
	
	-- 按照百分比减少金钱;
	for i = 1 , output_hour , 1 do
		output_money = output_money - output_hour_money * ( output_percent / 100 );
		if output_percent < 50 then
		   output_percent = output_percent + 5
		end
	end
	
	-- 最小值;
	output_money = math.min( output_money , output_max )
	output_money = math.abs(output_money);
	return output_money;
end

calc.CastleOutPutSoul = function( fight_power , elapse_time )

	local output_soul      = 0;
	local output_hour      = 0;
	local output_percent   = 0;
	local output_max       = 0;
	local output_hour_soul = 0;
	
	output_soul 		= fight_power / 30 + 20;
	output_hour 		= math.floor(elapse_time / 60);
	output_hour_soul	= fight_power / 1500 + 60 * 20;
	output_max          = output_hour_soul * 24;
	
	-- 禁止循环次数多过;
	if output_hour > 48 then
	   output_hour = 48
	end
	
	-- 按照百分比减少魂晶;
	for i = 1 , output_hour , 1 do
		output_soul = output_soul - output_hour_soul * ( output_percent / 100 );
		if output_percent < 50 then
		   output_percent = output_percent + 5
		end
	end
	
	-- 最小值;
	output_soul = math.min( output_soul , output_max )
	output_soul = math.abs( output_soul );
	return output_soul;
end

calc.ArenaReward  = function( ranking )
	
	local reward_money = 0;
	local reward_rmb   = 0;
	local reward_score = 0;
	
	if ( ranking == 1 ) then					         -- 第1名;
	   reward_money = 200000
	   reward_rmb   = 200
	   reward_score = 500
	elseif ( ranking >= 2 and ranking <= 10 ) then       -- 第2-10名; 
	   reward_money = 200000 / ranking + 50000
	   reward_rmb   = 200 / ranking + 70
	   reward_score = 500 / ranking + 200
	elseif ( ranking >= 11 and ranking <= 50 ) then      -- 第11-50名;
	   reward_money = 200000 / ranking + 50000
	   reward_rmb   = 200 / ranking + 60
	   reward_score = 500 / ranking + 190
	elseif ( ranking >= 51 and ranking <= 200 ) then     -- 第51-200名;
	   reward_money = 200000 / ranking + 40000
	   reward_rmb   = 200 / ranking + 50 
	   reward_score = 500 / ranking + 180
	elseif ( ranking >= 201 and ranking <= 1000 ) then   -- 第201-1000名;
	   reward_money = 200000 / ranking + 30000
	   reward_rmb   = 200 / ranking + 40
	   reward_score = 500 / ranking + 160
	elseif ( ranking >= 1001 and ranking <= 10000 ) then -- 第1001-10000名;
	   reward_money = 20000
	   reward_rmb   = 30
	   reward_score = 150
	else
	   reward_money = 0
	   reward_rmb   = 0
	   reward_score = 0
	end
	
	return reward_money , reward_rmb , reward_score
end

calc.CastleWinReward = function( IsFirst , IsWin , SelfMoney , SelfScore , SelfSoul , TarGetMoney , TarGetScore , ScoreLevel , SelfPvPScore , TarGetPvPScore )
	
	if ( IsFirst == 1 ) then
		SelfMoney = SelfMoney + SelfMoney / 2
	end
	
	if ( IsWin == 1 ) then
		SelfMoney = SelfMoney + 4900 + ScoreLevel * ScoreLevel * 100
		SelfSoul  = SelfSoul  + 4900 + ScoreLevel * ScoreLevel * 100
		if ( TarGetPvPScore < 100 ) then	-- 目标身上积分;
			 TarGetMoney = 0
			 SelfScore   = 0
		end
	else
		if ( SelfPvPScore < 100 ) then		-- 自己身上积分;
			 SelfScore   = 0
			 TarGetScore = 0
		end
	end
	
	return SelfMoney , SelfScore , SelfSoul , TarGetMoney , TarGetScore
end

calc.ArenaMatchingRanking = function( SelfRanking )
	
	-- defaul
	local MaxRanking = 1;
	local MinRanking = 1;
	
	if ( SelfRanking <=800 ) then
		MaxRanking = 200
		MinRanking = 200
	elseif ( SelfRanking > 800 and SelfRanking <= 1500 ) then
		MaxRanking = 200;
		MinRanking = 300;
	elseif ( SelfRanking > 1500 and SelfRanking <= 5000 ) then
		MaxRanking = 200;
		MinRanking = 400;
	else
		MaxRanking = 200;
		MinRanking = 1000;
	end
	
	return MaxRanking , MinRanking;
end

calc.PvPScore = function( SelfScore , TarGetScore )

	local WinScore = 5;

	if  TarGetScore <= SelfScore then
		return WinScore
	end
	
	WinScore = (TarGetScore-SelfScore)/20+5
	
	return WinScore;
end

calc.RevoverHeroVitality = function( SurTime , NeedGold , OneTime )

	if OneTime <= 0 then
		return -1;
	end

	local MeanGold   = NeedGold / OneTime
	local TotalGold  = MeanGold * SurTime
	
	return TotalGold;
end

calc.PvERefreshTime = function( MaxCount )

	local deftime = 3600
	
	if MaxCount <= 3 then
		return 5*60
	elseif MaxCount <= 6 then
		return 10*60
	else
	    return 20*60
	end
	
	return deftime
end

calc.PvPRefreshTime = function( MaxCount )

	local deftime = 7200
	
	if MaxCount <= 3 then
		return 6*60
	elseif MaxCount <= 6 then
		return 12*60
	else
		return 60*24
	end
	
	return deftime
end