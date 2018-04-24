use d_mt_player ;

drop procedure if exists _t_mt_add_player;

/* t_playerinfo */
drop procedure if exists _t_mt_select_playerinfo;
drop procedure if exists _t_mt_insert_playerinfo;
drop procedure if exists _t_mt_update_playerinfo_low;
drop procedure if exists _t_mt_update_playerinfo;

DELIMITER ;;

create procedure _t_mt_select_playerinfo(
	in af_uid bigint(20) unsigned )
begin
	select  f_uid,
            f_nickname,
            f_portrait,
            f_money,
            f_rmb,
            f_main,
            f_flag_bit
		   from t_playerinfo
		   where f_uid=af_uid;
end;;

create procedure _t_mt_insert_playerinfo(
	in af_uid bigint(20) unsigned
	)
begin
	insert into t_playerinfo( f_uid ) values( af_uid );

end;;

create procedure _t_mt_update_playerinfo_low(
	in af_uid bigint(20) unsigned,
	in af_nickname varchar(64),
    in af_portrait int(10)  unsigned 
    )
	
begin
	update t_playerinfo set
		   f_nickname = af_nickname,
           f_portrait = af_portrait
		   
    where  f_uid = af_uid;
end;;

create procedure _t_mt_update_playerinfo(
	in af_uid bigint(20) unsigned,
	in af_money bigint(20) unsigned,
    in af_rmb bigint(20) unsigned,
    in af_main int(10) unsigned,
    in af_flag_bit bigint(20) unsigned
    )
	
begin
	update t_playerinfo set
		   f_money = af_money,
           f_rmb = af_rmb,
           f_main = af_main,
           f_flag_bit = af_flag_bit
		   
    where  f_uid = af_uid;
end;;




create procedure _t_mt_add_player()
begin
	declare v int default 1;
	declare f_uid int default 1;

	while v < 10000
	do
		call _t_mt_insert_playerinfo( f_uid+1 );

		set v = v + 1;
		set f_uid = f_uid + 1;
		
	end while;
end	;;
	

DELIMITER ;











