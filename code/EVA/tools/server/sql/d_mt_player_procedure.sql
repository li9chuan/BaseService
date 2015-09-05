use d_mt_player ;

drop procedure if exists _t_mt_add_player;

/* t_playerinfo */
drop procedure if exists _t_mt_pagination_playerinfo;

drop procedure if exists _t_mt_insert_playerinfo;
drop procedure if exists _t_mt_update_playerinfo;
drop procedure if exists _t_mt_delete_playerinfo;

drop procedure if exists _t_mt_query_pid_max;


create procedure _t_mt_pagination_playerinfo(
	in a_greater_id bigint(20) unsigned )
begin
	select f_player_id,
	       f_account_id,
		   f_nickname,
		   f_level,
		   f_sex,
		   f_exp,
		   f_money
		   from t_playerinfo
		   where f_player_id>a_greater_id
		   order by f_player_id asc limit 1500;
end;

create procedure _t_mt_insert_playerinfo(
	in af_player_id bigint(20) unsigned,
	in af_account_id bigint(20) unsigned,
	in af_nickname char(24) ,
	in af_sex tinyint(3) unsigned,
	in af_level smallint(5) unsigned,
	in af_exp int(10) unsigned,
	in af_money bigint(20) unsigned
	)
begin
	insert into t_playerinfo(
	            f_player_id,
				f_account_id,
				f_nickname,
				f_sex,
				f_level,
				f_exp,
				f_money )
				
				values(
				af_player_id,
				af_account_id,
				af_nickname,
				af_sex,
				af_level,
				af_exp,
				af_money);		

end;

create procedure _t_mt_update_playerinfo(
	in af_player_id bigint(20) unsigned ,
	in af_nickname char(24) ,
	in af_level smallint(5) unsigned,
	in af_exp int(10) unsigned ,
	in af_money bigint(20) unsigned )
	
begin
	update t_playerinfo set
		   f_nickname = af_nickname ,
	       f_level = af_level ,
	       f_exp   = af_exp ,
		   f_money = af_money
		   
    where  f_player_id = af_player_id;
end;


--  6位服务器ID 36位ID  10位区ID
create procedure _t_mt_query_pid_max()
	select max( f_player_id >>10 ) from t_playerinfo;
	

create procedure _t_mt_add_player()
begin
	declare v int default 1;
	declare player_id int default 1;
    declare account_id int default 1;

	while v < 10000
	do

		call _t_mt_insert_playerinfo( player_id + 1, account_id + 1, 'mt' , 1 , 0, 10000000 );

		set v = v + 1;
		set player_id = player_id + 1;
		set account_id = account_id+ 1;
		
	end while;
end	;
	













