use d_mt_player ;

drop procedure if exists _t_mt_add_player;

/* t_playerinfo */
drop procedure if exists _t_select_playerinfo;
drop procedure if exists _t_mt_insert_playerinfo;
drop procedure if exists _t_mt_update_playerinfo;

DELIMITER ;;

create procedure _t_select_playerinfo(
	in af_uid bigint(20) unsigned )
begin
	select f_uid,
	       f_kpi
		   from t_playerinfo
		   where f_uid=af_uid;
end;;

create procedure _t_mt_insert_playerinfo(
	in af_uid bigint(20) unsigned,
	in af_kpi blob
	)
begin
	insert into t_playerinfo(
	            f_uid,
				f_kpi )

				values(
				af_uid,
				af_kpi);		

end;;

create procedure _t_mt_update_playerinfo(
	in af_uid bigint(20) unsigned,
	in af_kpi blob )
	
begin
	update t_playerinfo set
		   af_kpi = af_kpi
		   
    where  f_uid = af_uid;
end;;




create procedure _t_mt_add_player()
begin
	declare v int default 1;
	declare f_uid int default 1;

	while v < 10000
	do
		call _t_mt_insert_playerinfo( f_uid+1, 'mt\0jjj' );

		set v = v + 1;
		set f_uid = f_uid + 1;
		
	end while;
end	;;
	

DELIMITER ;











