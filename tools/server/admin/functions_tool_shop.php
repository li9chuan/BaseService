<?php

	function tool_shop_get_check_items()
	{
		global $NELTOOL;
		$check_items = "";

		reset($NELTOOL['POST_VARS']);
		foreach($NELTOOL['POST_VARS'] as $post_key => $post_val)
		{
			$val = 'checkshoplist_'. $post_val;
			if ($post_key == $val)
			{
				$check_items = $check_items ."`". $post_val;
			}
		}

		return $check_items;
	}
	
	function tool_sp_parse_shop_list($data)
	{
		$entity_data = array();

		reset($data);
		foreach($data as $egs_data)
		{
			$service_name = 'n/a';

			reset($egs_data);
			foreach($egs_data as $egs_line)
			{
				$egs_line = trim($egs_line);
				if (ereg("^===\[ Service ([^\ ]+) returned \]===$", $egs_line, $eregs))
				{
					$service_name = $eregs[1];
				}
				elseif (ereg("^SID: ([^\ ]+) IDX: ([^\ ]+) TID: ([^\ ]+) Name: ([^\ ]+) NUM: ([^\ ]+) PRICE: ([^\ ]+)$", $egs_line, $eregs))
				{
					$entity_data[] = array(	'service'		=>	$service_name,
											'shardid'		=>	$eregs[1],
											'idx'			=>	$eregs[2],
											'tid'			=>	$eregs[3],
											'name'			=>	$eregs[4],
											'num'			=>	$eregs[5],
											'price'			=>	$eregs[6],
											);
				}
			}
		}

		return $entity_data;
	}

?>