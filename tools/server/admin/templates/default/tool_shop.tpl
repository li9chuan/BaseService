
{include file="page_header.tpl"}

{literal}
<script language="Javascript" type="text/javascript">
<!--
	function CheckAll()
	{
		for (var i=0; i<document.qlist.elements.length; i++)
		{
			var e = document.qlist.elements[i];
			//if (e.type == 'checkbox' && e.name != 'allbox')
			if (e.type == 'checkbox' && e.name.substring(0,8) == 'service_')
				e.checked = document.qlist.allbox.checked;
		}
	}
	
	function CheckAllShopList()
	{
		for (var i=0; i<document.qlist.elements.length; i++)
		{
			var e = document.qlist.elements[i];

			if (e.type == 'checkbox' && e.name.substring(0,14) == 'checkshoplist_')
				e.checked = document.qlist.allbox_shoplist.checked;
		}
	}
	

	function CheckToggle(checkname)
	{
		checkname.checked = !checkname.checked;
	}

	var total_secs;

	function TimerDown(secs)
	{
		total_secs = secs;
		CountDown();
	}

	function TimerDisplay(secs)
	{
		timer_min = Math.floor(secs / 60);
		timer_sec = secs % 60;

		if (timer_min < 10) timer_min = '0'+ timer_min;
		if (timer_sec < 10) timer_sec = '0'+ timer_sec;

		return timer_min+':'+timer_sec;
	}

	function CountDown()
	{
		total_secs--;
		if (total_secs >= 0)
		{
			document.fcounter.counter.value = TimerDisplay(total_secs);
			down=setTimeout("CountDown()",1000);
		}
	}

	function toggleBox(mybox1, mybox2)
	{
		if (document.all)
		{
			if (document.all.item(mybox1).style.display == "none")
			{
				document.all.item(mybox1).style.display = "";
				document.all.item(mybox2).style.display = "none";
			}
			else
			{
				document.all.item(mybox2).style.display = "";
				document.all.item(mybox1).style.display = "none";
			}
		}
		else
		{
			if (document.getElementById(mybox1).style.display == "none")
			{
				document.getElementById(mybox1).style.display = "";
				document.getElementById(mybox2).style.display = "none";
			}
			else
			{
				document.getElementById(mybox2).style.display = "";
				document.getElementById(mybox1).style.display = "none";
			}
		}
	}

//-->
</script>
{/literal}

<br>

<table width="100%" border="0" cellpadding="0" cellspacing="10">
<tr>
	<td align="left" valign="top" width="150px">

{if $tool_domain_selected && $tool_shard_selected}
		<table width="100%" border="0" cellpadding="1" bgcolor="#cccccc" class="view">
		<tr>
			<th colspan="10">Refresh</th>
		</tr>
		<form action="tool_shop.php?domain={$tool_domain_selected}&shard={$tool_shard_selected}" method="post" name="fcounter">
		<tr>
			<td>
				<select name="services_refresh" style="width:100%;" onchange="this.form.submit();">
{section name=refresh loop=$tool_refresh_list}
					<option value="{$tool_refresh_list[refresh].secs}" {if $tool_refresh_rate == $tool_refresh_list[refresh].secs}selected{/if}>{$tool_refresh_list[refresh].desc}</option>
{/section}
				</select>
			</td>
		</tr>
{if $tool_refresh_rate > 0}
		<tr>
			<td>
				<input type="text" name="counter" value="" readonly class="refresh_counter">
				<script language="Javascript" type="text/javascript">
					<!--
					TimerDown({$tool_refresh_rate});
					-->
				</script>
			</td>
		</tr>
{/if}
		</form>
		</table>
		<br>
{/if}

		<table width="100%" border="0" cellpadding="1" bgcolor="#cccccc" class="view">
		<tr>
			<th colspan="10">Domains</th>
		</tr>
{section name=domain loop=$tool_domain_list}
		<tr>
			<td align="center" class="{if $tool_domain_selected == $tool_domain_list[domain].domain_id}domainlistselected{else}domainlist{/if}"><a href="tool_shop.php?domain={$tool_domain_list[domain].domain_id}">{$tool_domain_list[domain].domain_name}</a></td>
		</tr>
{/section}
		</table>

{if $tool_domain_selected}
		<br>
		<table width="100%" border="0" cellpadding="1" bgcolor="#cccccc" class="view">
		<tr>
			<th colspan="10">Shards</th>
		</tr>
{section name=shard loop=$tool_shard_list}
{if $tool_domain_selected == $tool_shard_list[shard].shard_domain_id}
		<tr>
			<td align="center" class="{if $tool_shard_selected == $tool_shard_list[shard].shard_id}shardlistselected{else}shardlist{/if}"><a href="tool_shop.php?domain={$tool_domain_selected}&shard={$tool_shard_list[shard].shard_id}">{$tool_shard_list[shard].shard_name}</a></td>
		</tr>
{/if}
{/section}
		</table>
{/if}

	</td>

	<td width="10px">&nbsp;</td>

	<td align="right" valign="top">

{if !$tool_domain_selected}
		<table width="100%" border="0" cellpadding="1" bgcolor="#cccccc" class="view">
		<tr>
			<td class="row0">You need to select a domain.</td>
		</tr>
		</table>
{elseif $tool_domain_error}
		<table width="100%" border="0" cellpadding="1" bgcolor="#cccccc" class="view">
		<tr>
			<td class="row0"><span class="alert">{$tool_domain_error}</span></td>
		</tr>
		</table>
{else}
{if $tool_as_error}
		<table width="100%" border="0" cellpadding="1" bgcolor="#cccccc" class="view">
		<tr>
			<td class="row0"><span class="alert">{$tool_as_error}</span></td>
		</tr>
		</table>
		<br>
{/if}


		<form action="tool_shop.php" method="post" name="qlist">
		<table width="100%" border="0" cellpadding="1" bgcolor="#cccccc" class="view">
		<tr>
			<td class="heads"><input class="check" type="checkbox" name="allbox" value="1" onclick="CheckAll();"></td>
			<td class="heads">AliasName</td>
			<td class="heads">Shard</td>
			<td class="heads">ShortName</td>
			<td class="heads">Hostname</td>
			<td class="heads">Running State</td>
			<td class="heads">Running Tags</td>
			<td class="heads">State</td>
			<td class="heads">Report</td>
			<td class="heads">Counters</td>
			<td class="heads">User SL</td>
			<td class="heads">Tick SL</td>
			<td class="heads">Memory</td>
			<td class="heads">NbPlayers</td>
			<td class="heads">UpTime</td>
		</tr>
{assign var="egs_counter" value="0"}
{section name=service loop=$tool_services_list}
{assign var="service_shard_id" value=$tool_services_list[service].ShardName}
{if $tool_shard_filters.$service_shard_id || $tool_shard_filters._all_ || ($tool_shard_filters._unknown_ && !$tool_services_list[service].ShardName)}
{cycle assign="trclass" values="row0,row1"}
{assign var="tdclass1" value=""}
{assign var="tdclass2" value=""}
{if $tool_services_list[service]._flags_.rs_stopped}{assign var="tdclass1" value="class=\"cell_inactive1\""}{assign var="tdclass2" value="class=\"cell_inactive2\""}{assign var="trclass" value="row_stopped"}{/if}
{if $tool_services_list[service]._flags_.rs_starting}{assign var="tdclass1" value="class=\"cell_inactive1\""}{assign var="tdclass2" value="class=\"cell_inactive2\""}{assign var="trclass" value="row_starting"}{/if}
{if $tool_services_list[service]._flags_.alert_red}{assign var="trclass" value="row_red"}{/if}
{if $tool_services_list[service]._flags_.alert_orange_dark}{assign var="trclass" value="row_orange_dark"}{/if}
{if $tool_services_list[service]._flags_.alert_orange_light}{assign var="trclass" value="row_orange_light"}{/if}
{assign var="check_name" value=$tool_services_list[service].AliasName}
{assign var="egs_counter" value="`$egs_counter+1`"}
<!-- {$egs_counter} -->
		<tr class="{$trclass}">
			<td><input class="check" type="checkbox" name="service_{$tool_services_list[service].AliasName}" value="{$tool_services_list[service].AliasName}" {if $tool_service_select_list.$check_name || (!$tool_service_select_list && $tool_services_list[service]._flags_.rs_online)}checked{/if}></td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass1}>{$tool_services_list[service].AliasName}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass1}>{if $tool_services_list[service].ShardName != ""}{$tool_services_list[service].ShardName}{else}?{/if}{if $tool_services_list[service].ShardId != ""}/{$tool_services_list[service].ShardId}{/if}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass1}>{$tool_services_list[service].ShortName}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass1}>{$tool_services_list[service].Hostname}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass1}>{$tool_services_list[service].RunningState}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass1}>{$tool_services_list[service].RunningTags}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass2}>{$tool_services_list[service].State}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass2}>{$tool_services_list[service].NoReportSince}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass2}>{$tool_services_list[service].StartCounter}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass2}>{$tool_services_list[service].UserSpeedLoop}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass2}>{$tool_services_list[service].TickSpeedLoop}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass2}>{$tool_services_list[service].ProcessUsedMemory}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass2}>{$tool_services_list[service].NbPlayers}</td>
			<td onclick="CheckToggle(document.qlist.service_{$tool_services_list[service].AliasName})" {$tdclass2}>{$tool_services_list[service].UpTime}</td>
		</tr>
{/if}
{/section}
		</table>

		<!-- ugly trick to block the first submit button being triggered when hitting ENTER to send the form -->
		<div style="display: none;"><input type="submit" name="fake" value="fake" onclick="alert('PLEASE DO NOT USE THE &lt;ENTER&gt; KEY !'); return false;"></div>
		<!-- end ugly trick :) -->

{if $egs_counter > 0}
{if $restriction_tool_shop}
		<br>
		<table width="100%" border="0" cellpadding="1" bgcolor="#cccccc" class="view">
		<tr>
			<td align="right" width="100px"><b>Command : </b></td>
			<td>&nbsp;&nbsp;&nbsp;&nbsp;
				物品ID：<input type="text" name="tpl_item_template_id" value="{$tool_template_id_value}" size="10">&nbsp;
				数量：<input type="text" name="tpl_item_num" value="{$tool_item_num_value}" size="10">&nbsp;
				价格：<input type="text" name="tpl_item_price" value="{$tool_item_price_value}" size="10">&nbsp;
				<input type="submit" name="services_pl" value="添加物品">&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="submit" name="services_pl" value="删除物品">&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="submit" name="services_pl" value="刷新物品列表">&nbsp;
			</td>
		</tr>
		</table>
{/if}


{if $tool_shop_list_data}
{assign var="service_name" value=""}
		<br>
		<table width="100%" border="0" cellpadding="1" bgcolor="#cccccc" class="view">
{section name=entity loop=$tool_shop_list_data}
{if $service_name != $tool_shop_list_data[entity].service}
{assign var="service_name" value=$tool_shop_list_data[entity].service}
		<tr>
			<td class="heads"><input class="check" type="checkbox" name="allbox_shoplist" value="1" onclick="CheckAllShopList();"></td>
			<th>ShardID</th>
			<th>IDX</th>
			<th>模板ID</th>
			<th>物品名称</th>
			<th>数量</th>
			<th>价格</th>
		</tr>
{/if}
{cycle assign="trclass" values="row0,row1"}
		<tr class="{$trclass}">
			<td><input class="check" type="checkbox" name="checkshoplist_{$tool_shop_list_data[entity].shardid}_{$tool_shop_list_data[entity].idx}" value="{$tool_shop_list_data[entity].shardid}_{$tool_shop_list_data[entity].idx}"></td>
			<td align="center" onclick="CheckToggle(document.qlist.checkshoplist_{$tool_shop_list_data[entity].shardid}_{$tool_shop_list_data[entity].idx})">{$tool_shop_list_data[entity].shardid}</td>
			<td align="center" onclick="CheckToggle(document.qlist.checkshoplist_{$tool_shop_list_data[entity].shardid}_{$tool_shop_list_data[entity].idx})">{$tool_shop_list_data[entity].idx}</td>
			<td align="center" onclick="CheckToggle(document.qlist.checkshoplist_{$tool_shop_list_data[entity].shardid}_{$tool_shop_list_data[entity].idx})">{$tool_shop_list_data[entity].tid}</td>
			<td align="center" onclick="CheckToggle(document.qlist.checkshoplist_{$tool_shop_list_data[entity].shardid}_{$tool_shop_list_data[entity].idx})">{$tool_shop_list_data[entity].name}</td>
			<td align="center" onclick="CheckToggle(document.qlist.checkshoplist_{$tool_shop_list_data[entity].shardid}_{$tool_shop_list_data[entity].idx})">{$tool_shop_list_data[entity].num}</td>
			<td align="center" onclick="CheckToggle(document.qlist.checkshoplist_{$tool_shop_list_data[entity].shardid}_{$tool_shop_list_data[entity].idx})">{$tool_shop_list_data[entity].price}</td>
		</tr>
{/section}
		</table>
{/if}

{if $tool_execute_command}
		<br>
		<table width="100%" border="0" cellpadding="1" bgcolor="#cccccc" class="view">
		<tr>
			<th>Command Results for '{$tool_execute_command}' :</th>
		</tr>
		<tr>
			<td><textarea width="100%" rows="50" class="command" readonly >{section name=exe loop=$tool_execute_result}{$tool_execute_result[exe]}{/section}</textarea></td>
		</tr>
		</table>
{/if}

{else}
		<br>
		<table width="100%" border="0" cellpadding="1" bgcolor="#cccccc" class="view">
		<tr>
			<td class="row0"><span class="alert">No EGS to work with!</span></td>
		</tr>
		</table>
{/if}

		<input type="hidden" name="pl_su" value="{$shard_su_name}">
		<input type="hidden" name="relocate_shardid" value="na">
		<input type="hidden" name="relocate_eid" value="na">
		</form>

{/if}

	</td>
</tr>
</table>


{include file="page_footer.tpl"}
