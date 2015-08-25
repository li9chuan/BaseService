<?php

	require_once('common.php');
	require_once('functions_tool_main.php');
	require_once('functions_tool_shop.php');

	if (!tool_admin_applications_check('tool_shop'))	nt_common_redirect('index.php');

	nt_common_add_debug('-- Starting on \'tool_shop.php\'');

	$tpl->assign('tool_title', "商城管理");

	$view_domain_id = nt_auth_get_session_var('view_domain_id');
	$view_shard_id 	= nt_auth_get_session_var('view_shard_id');

	if (!$view_domain_id)
	{
		$view_domain_id	= $nel_user['group_default_domain_id'];
		$view_shard_id	= $nel_user['group_default_shard_id'];
		nt_auth_set_session_var('view_domain_id', $view_domain_id);
		nt_auth_set_session_var('view_shard_id', $view_shard_id);
	}

	if (isset($NELTOOL['GET_VARS']['domain']))
	{
		if ($view_domain_id != $NELTOOL['GET_VARS']['domain'])
		{
			$view_domain_id = $NELTOOL['GET_VARS']['domain'];
			nt_auth_set_session_var('view_domain_id', $view_domain_id);

			$view_shard_id = null;
			nt_auth_unset_session_var('view_shard_id');
		}
	}

	if (isset($NELTOOL['GET_VARS']['shard']))
	{
		$view_shard_id = $NELTOOL['GET_VARS']['shard'];
		nt_auth_set_session_var('view_shard_id', $view_shard_id);
	}

	if (isset($NELTOOL['GET_VARS']['refdata']))
	{
		$tmp_data = unserialize(base64_decode($NELTOOL['GET_VARS']['refdata']));
		if (is_array($tmp_data))
		{
			$NELTOOL['POST_VARS'] = $tmp_data;
		}
	}

	$current_refresh_rate = nt_auth_get_session_var('current_refresh_rate');

	if (isset($_POST['services_refresh']))
	{
		if ($current_refresh_rate != $_POST['services_refresh'])
		{
			$current_refresh_rate = $_POST['services_refresh'];
			nt_auth_set_session_var('current_refresh_rate',$current_refresh_rate);
		}
	}

	if ($current_refresh_rate == null)
	{
		$current_refresh_rate = 0;
	}
	elseif ($current_refresh_rate > 0)
	{
		$tpl->assign('nel_tool_refresh',	'<meta http-equiv=refresh content="'. $current_refresh_rate .'">');
	}

	$tpl->assign('tool_refresh_list',		$refresh_rates);
	$tpl->assign('tool_refresh_rate',		$current_refresh_rate);

	$tpl->assign('tool_domain_list',		$nel_user['access']['domains']);
	$tpl->assign('tool_domain_selected',	$view_domain_id);

	$tpl->assign('tool_shard_list',			$nel_user['access']['shards']);
	$tpl->assign('tool_shard_selected',		$view_shard_id);

	$tool_shard_filters	= tool_main_get_shard_ids($view_shard_id);
	$tpl->assign('tool_shard_filters',		$tool_shard_filters);

	if (tool_admin_applications_check('tool_shop'))		$tpl->assign('restriction_tool_shop',	true);

	if ($view_domain_id)
	{
		$tool_as_error = null;

		$AS_Name 		= tool_main_get_domain_name($view_domain_id);
		$AS_Host 		= tool_main_get_domain_host($view_domain_id);
		$AS_Port 		= tool_main_get_domain_port($view_domain_id);
		$AS_ShardName	= tool_main_get_shard_name($view_shard_id);
		$AS_Application	= tool_main_get_domain_data($view_domain_id, 'domain_application');
		$AS_RingSQL		= tool_main_get_domain_data($view_domain_id, 'domain_sql_string');

		$tpl->assign('tool_page_title', 'Player Locator - '. $AS_Name . ($AS_ShardName != '' ? ' / '. $AS_ShardName : ''));

		$tool_as_error = null;

		if ($AS_Host && $AS_Port)
		{
			$adminService = new MyAdminService;
			if (@$adminService->connect($AS_Host, $AS_Port, $res) === false)
			{
				nt_common_add_debug($res);
				$tpl->assign('tool_domain_error', $res );
			}
			else
			{
				if (isset($NELTOOL['POST_VARS']['services_pl']))
				{
					$tool_services_pl = $NELTOOL['POST_VARS']['services_pl'];
					$tpl->assign('tool_post_data',	base64_encode(serialize($NELTOOL['POST_VARS'])));

					switch ($tool_services_pl)
					{
						case '刷新物品列表':

							if (tool_admin_applications_check('tool_shop'))
							{
								$service_list = tool_main_get_checked_services();
								if (sizeof($service_list))
								{
									$service_command = 'shoplist';

									nt_log("Domain '$AS_Name' : '$service_command' on ". implode(', ',array_values($service_list)));

									$tpl->assign('tool_service_select_list', array_combine($service_list, $service_list));
									$tpl->assign('tool_execute_result', '');
									$command_return_data = array();

									reset($service_list);
									foreach($service_list as $service)
									{
										//nt_common_add_debug("about to run 'displayPlayers' on '$service' ...");

										$adminService->serviceCmd($service, $service_command);
										if (!$adminService->waitCallback())
										{
											nt_common_add_debug('Error while waiting for callback on service \''. $service .'\' for command : '. $service_command);
										}
										else
										{
											// the locator displays a nice output, no need for the raw one
											//$tpl->assign('tool_execute_command', 	$service_command);
										}
									}

									if (sizeof($command_return_data))
									{
										$shop_list_data = tool_sp_parse_shop_list($command_return_data);
										$tpl->assign('tool_shop_list_data',	$shop_list_data);
									}
								}
							}

							break;

						case '添加物品':

							if (tool_admin_applications_check('tool_shop'))
							{
								$tool_template_id	= trim($NELTOOL['POST_VARS']['tpl_item_template_id']);
								$tpl->assign('tool_template_id_value', $tool_template_id);
								
								$tool_item_num	= trim($NELTOOL['POST_VARS']['tpl_item_num']);
								$tpl->assign('tool_item_num_value', $tool_item_num);
								
								$tool_item_price	= trim($NELTOOL['POST_VARS']['tpl_item_price']);
								$tpl->assign('tool_item_price_value', $tool_item_price);
								
								nt_common_add_debug('----- \''. $tool_template_id .'\' for command : '. $tool_item_num);

								$service_list = tool_main_get_checked_services();
								if (sizeof($service_list) && ($tool_template_id != '') && ($tool_item_num != '') && ($tool_item_price != ''))
								{
									$service_command = 'addshopitem '. $tool_template_id. ' '. $tool_item_num. ' '. $tool_item_price;
									nt_log("Domain '$AS_Name' : '$service_command' on ". implode(', ',array_values($service_list)));

									$tpl->assign('tool_service_select_list', array_combine($service_list, $service_list));
									$command_return_data = array();

									reset($service_list);
									foreach($service_list as $service)
									{
										nt_common_add_debug("about to run 'addshopitem' on '$service' ...");

										$adminService->serviceCmd($service, $service_command);
										if (!$adminService->waitCallback())
										{
											nt_common_add_debug('Error while waiting for callback on service \''. $service .'\' for command : '. $service_command);
										}
									}
									
									if (sizeof($command_return_data))
									{
										$shop_list_data = tool_sp_parse_shop_list($command_return_data);
										$tpl->assign('tool_shop_list_data',	$shop_list_data);
									}
								}
							}

							break;
						case '删除物品':
							
							if (tool_admin_applications_check('tool_shop'))
							{
								$check_item_list = tool_shop_get_check_items();
								$service_list = tool_main_get_checked_services();

								if (sizeof($service_list) && sizeof($check_item_list))
								{
									$service_command = 'removeshopitem '. $check_item_list;
									nt_log("Domain '$AS_Name' : '$service_command' on ". implode(', ',array_values($service_list)));

									$tpl->assign('tool_service_select_list', array_combine($service_list, $service_list));
									$command_return_data = array();

									reset($service_list);
									foreach($service_list as $service)
									{
										nt_common_add_debug("about to run 'removeshopitem' on '$service' ...");

										$adminService->serviceCmd($service, $service_command);
										if (!$adminService->waitCallback())
										{
											nt_common_add_debug('Error while waiting for callback on service \''. $service .'\' for command : '. $service_command);
										}
									}
									
									if (sizeof($command_return_data))
									{
										$shop_list_data = tool_sp_parse_shop_list($command_return_data);
										$tpl->assign('tool_shop_list_data',	$shop_list_data);
									}
								}
							}
							break;
					}
				}

				if (isset($NELTOOL['GET_VARS']['eid']))
				{
					$locate_eid = $NELTOOL['GET_VARS']['eid'];

					// someday i'll do something here :)
				}

				$status = $adminService->getStates();
				nt_common_add_debug($status);

				$domainServices		= tool_main_parse_status($status);

				$filteredServices	= array();
				reset($domainServices);
				foreach($domainServices as $aKey => $aService)
				{
					// we are only interested in EGS
					if ($aService['ShortName'] == 'EGS')
					{
						$filteredServices[] = $aService;
					}
				}

				$tpl->assign('tool_services_list',	$filteredServices);
				$tpl->assign('shard_su_name',	tool_main_get_su_from_status($domainServices));
			}
		}
	}

	$tpl->display('tool_shop.tpl');

?>