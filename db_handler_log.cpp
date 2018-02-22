/*******************************************************************************

Copyright 2010 by tiankong Interactive Game Co., Ltd.
All rights reserved.

This software is the confidential and proprietary information of
tiankong Interactive Game Co., Ltd. ('Confidential Information'). You shall
not disclose such Confidential Information and shall use it only in
accordance with the terms of the license agreement you entered into with
tiankong Interactive  Co., Ltd.

*******************************************************************************/

// 日志

#include "StdAfx.h"
#include "../common/ServerDefine/log_server_define.h"
#include "db_handler.h"


// 日志表明
LPCSTR log_of_fund				= "log_fund";
LPCSTR log_of_item				= "log_item";
LPCSTR log_of_money				= "log_money";
LPCSTR log_of_gm_cmd			= "log_gmcmd";
LPCSTR log_of_yuanbao			= "log_yuanbao";
LPCSTR log_of_exvolume			= "log_exvolume";
LPCSTR log_of_exploits			= "log_exploits";
LPCSTR log_of_timestat			= "log_time_stat";
LPCSTR log_of_material			= "log_material";
LPCSTR log_of_mall_sell			= "log_mallsell";
LPCSTR log_of_item_times		= "log_itemtimes";
LPCSTR log_of_reputation		= "log_reputation";
LPCSTR log_of_money_sell_pack	= "log_mallsell_pack";
LPCSTR log_of_bank				= "log_bank";
LPCSTR log_of_paimai			= "log_paimai";
LPCSTR log_of_leave_pracitce	= "log_leave_practice";
LPCSTR log_of_serial_reward		= "log_serial_reward";
LPCSTR log_of_kick				= "log_kick";

// 创建日志表方法
BOOL db_handler::common_create_log_table(LPCSTR str_table, LPCSTR sz_table_postfix)
{
	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = log_db_interface_->get_io();
	ASSERT(VALID_POINT(p_stream));

	p_stream->clear();
	p_stream->write_string("CREATE TABLE IF NOT EXISTS ");
	p_stream->write_string(str_table);
	p_stream->write_string(sz_table_postfix);
	p_stream->write_string(" LIKE ");
	p_stream->write_string(str_table);

	if(!log_db_interface_->sql_execute(p_stream))
	{
		print_message(_T("\n\n\nLog table<%s"), get_tool()->unicode8_to_unicode(str_table));
		print_message(_T("%s> create failed!\n\n\n"), get_tool()->unicode8_to_unicode(sz_table_postfix));

		// 归还sql格式化IO
		log_db_interface_->return_io(p_stream);

		return FALSE;
	}

	print_message(_T("Log table<%s"), get_tool()->unicode8_to_unicode(str_table));
	print_message(_T("%s> create successful!\n"), get_tool()->unicode8_to_unicode(sz_table_postfix));

	// 归还sql格式化IO
	log_db_interface_->return_io(p_stream);

	return TRUE;
}


// 金钱日志
BOOL db_handler::create_log_money_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_money, sz_table_postfix);
}

VOID db_handler::insert_money_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_money_log));
}

VOID db_handler::makesql_insert_money_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, s_log_silver);
	s_log_silver * p = ( s_log_silver * ) p_buffer ;  

	p_stream->insert_item(log_of_money, sz_table_postfix);

	p_stream->write_string("account_id=") << p->dw_account_id;
	p_stream->write_string(",role_id=") << p->dw_role_id;
	p_stream->write_string(",cmd_id=") << p->dw_cmd_id;
	p_stream->write_string(",container_type=") << p->n8_log_con_type;
	p_stream->write_string(",num=") << p->n64_silver;
	p_stream->write_string(",total_num=") << p->n64_total_silver;
	p_stream->write_string(",role_id_relation=") << p->dw_role_id_rel;
	p_stream->write_string(",time=now()");
}

// 元宝日志
BOOL db_handler::create_log_yuanbao_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_yuanbao, sz_table_postfix);
}

VOID db_handler::insert_yuanbao_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_yuanbao_log));
}

VOID db_handler::makesql_insert_yuanbao_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, s_log_yuanbao);
	s_log_yuanbao * p = ( s_log_yuanbao * ) p_buffer ;  

	p_stream->insert_item(log_of_yuanbao, sz_table_postfix);

	p_stream->write_string("account_id=") << p->dw_account_id;
	p_stream->write_string(",role_id=") << p->dw_role_id;
	p_stream->write_string(",cmd_id=") << p->dw_cmd_id;
	p_stream->write_string(",role_id_relation=") << p->dw_role_id_rel;
	p_stream->write_string(",container_type=") << p->n8_log_con_type;
	p_stream->write_string(",num=") << p->n_yuanbao;
	p_stream->write_string(",total_num=") << p->n_total_yuanbao;
	p_stream->write_string(",time=now()");
}

// 赠点日志
BOOL db_handler::create_log_exvolume_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_exvolume, sz_table_postfix);
}

VOID db_handler::insert_exvolume_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_exvolume_log));
}

VOID db_handler::makesql_insert_exvolume_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, s_log_ex_volume);
	s_log_ex_volume * p = ( s_log_ex_volume * ) p_buffer ;  

	p_stream->insert_item(log_of_exvolume, sz_table_postfix);

	p_stream->write_string("account_id=") << p->dw_account_id;
	p_stream->write_string(",role_id=") << p->dw_role_id;
	p_stream->write_string(",cmd_id=") << p->dw_cmd_id;
	p_stream->write_string(",num=") << p->n_ex_volume;
	p_stream->write_string(",total_num=") << p->n_total_ex_volume;
	p_stream->write_string(",time=now()");
}

// 战功日志
BOOL db_handler::create_log_exploits_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_exploits, sz_table_postfix);
}

VOID db_handler::insert_exploits_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::madesql_insert_exploits_log));
}

VOID db_handler::madesql_insert_exploits_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	s_log_exploits* p = (s_log_exploits*) p_buffer;

	p_stream->insert_item(log_of_exploits, sz_table_postfix);
	p_stream->write_string("account_id=") << p->dw_account_id;
	p_stream->write_string(",role_id=") << p->dw_role_id;
	p_stream->write_string(",cmd_id=") << p->dw_cmd_id;
	p_stream->write_string(",num=") << p->n_exploits;
	p_stream->write_string(",total_num=") << p->n_total_exploits;
	p_stream->write_string(",time=now()");

}

// 定时日志
BOOL db_handler::create_log_time_stat_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_timestat, sz_table_postfix);
}

VOID db_handler::insert_time_stat_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_time_stat_log));
}

VOID db_handler::makesql_insert_time_stat_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, s_log_time_stat);
	s_log_time_stat * p = ( s_log_time_stat * ) p_buffer ;  

	p_stream->insert_item(log_of_timestat, sz_table_postfix);

	p_stream->write_string("account_id=") << p->dw_account_id;
	p_stream->write_string(",role_id=") << p->dw_role_id;
	p_stream->write_string(",bag_silver=") << p->n64_bag_silver;
	p_stream->write_string(",ware_silver=") << p->n64_ware_silver;
	p_stream->write_string(",bag_yuanbao=") << p->n_bag_yuanbao;
	p_stream->write_string(",baibao_yuanbao=") << p->n_baibao_yuanbao;
	p_stream->write_string(",ex_volume=") << p->n_ex_volume;
	p_stream->write_string(",time=now()");
}

// 物品日志
BOOL db_handler::create_log_item_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_item, sz_table_postfix);
}

VOID db_handler::insert_item_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_item_log));
}

VOID db_handler::makesql_insert_item_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, s_log_item);
	s_log_item * p = ( s_log_item * ) p_buffer ;  

	p_stream->insert_item(log_of_item, sz_table_postfix);

	p_stream->write_string("role_id=") << p->dw_role_id;
	p_stream->write_string(",cmd_id=") << p->dw_cmd_id;
	p_stream->write_string(",type_id=") << p->dw_data_id;
	p_stream->write_string(",optnum=") << p->n16_opt_num;

	p_stream->write_string(",serial1=") << p->n64_serial1;
	p_stream->write_string(",container_type1=") << p->n8_con_type1;
	p_stream->write_string(",result_num1=") << p->n16_res_num1;

	p_stream->write_string(",serial2=") << p->n64_serial2;
	p_stream->write_string(",container_type2=") << p->n8_con_type2;
	p_stream->write_string(",result_num2=") << p->n16_res_num2;

	p_stream->write_string(",role_id_relation=") << p->dw_role_id_rel;

	p_stream->write_string(",time=now()");
}

// 物品次数日志
BOOL db_handler::create_log_item_times_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_item_times, sz_table_postfix);
}

VOID db_handler::insert_item_times_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_item_times_log));
}

VOID db_handler::makesql_insert_item_times_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, s_log_item_times);
	s_log_item_times * p = ( s_log_item_times * ) p_buffer ;  

	p_stream->insert_item(log_of_item_times, sz_table_postfix);

	p_stream->write_string("role_id=") << p->dw_role_id;
	p_stream->write_string(",cmd_id=") << p->dw_cmd_id;
	p_stream->write_string(",serial=") << p->n64_serial;
	p_stream->write_string(",type_id=") << p->dw_data_id;
	p_stream->write_string(",container_type=") << p->n8_con_type;
	p_stream->write_string(",used_times=") << p->n_used_times;
	p_stream->write_string(",max_use_times=") << p->n_max_use_times;

	p_stream->write_string(",time=now()");
}

// 商城出售日志
BOOL db_handler::create_log_mall_sell_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_mall_sell, sz_table_postfix);
}

VOID db_handler::insert_mall_sell_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_mall_sell_log));
}

VOID db_handler::makesql_insert_mall_sell_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, s_log_mall_sell);
	s_log_mall_sell * p = ( s_log_mall_sell * ) p_buffer ;  

	p_stream->insert_item(log_of_mall_sell, sz_table_postfix);

	p_stream->write_string("cmd_id=") << p->dw_cmd_id;
	p_stream->write_string(",serial=") << p->n64_serial;
	p_stream->write_string(",type_id=") << p->dw_data_id;
	p_stream->write_string(",role_id=") << p->dw_role_id;
	p_stream->write_string(",number=") << p->n16_num_sell;
	p_stream->write_string(",exist_time=") << p->dw_exist_item;
	p_stream->write_string(",max_use_times=") << p->n_max_use_times;
	p_stream->write_string(",cost_yuanbao=") << p->n_cost_yuanbao;
	p_stream->write_string(",cost_ex_volume=") << p->n_cost_ex_volume;

	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_first_gain_time);
	p_stream->write_string(",first_gain_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");

	p_stream->write_string(",time=now()");
}

// 商城出售的礼包日志
BOOL db_handler::create_log_mall_sell_pack_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_money_sell_pack, sz_table_postfix);
}

VOID db_handler::insert_mall_sell_pack_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_mall_sell_pack_log));
}

VOID db_handler::makesql_insert_mall_sell_pack_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, s_log_mall_sell_pack);
	s_log_mall_sell_pack * p = ( s_log_mall_sell_pack * ) p_buffer ;  

	p_stream->insert_item(log_of_money_sell_pack, sz_table_postfix);

	p_stream->write_string("pack_id=") << p->dw_pack_id;
	p_stream->write_string(",buyer_id=") << p->dw_buy_role_id;
	p_stream->write_string(",gainer_id=") << p->dw_to_role_id;
	p_stream->write_string(",buy_price=") << p->n_cost_yuanbao;

	p_stream->write_string(",time=now()");
}

// 帮派基金日志
BOOL db_handler::create_log_fund_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_fund, sz_table_postfix);
}

VOID db_handler::insert_fund_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_fund_log));
}

VOID db_handler::makesql_insert_fund_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, tagLogFund);
	tagLogFund * p = ( tagLogFund * ) p_buffer ;  

	p_stream->insert_item(log_of_fund, sz_table_postfix);

	p_stream->write_string("guild_id=") << p->dw_guild_id;
	p_stream->write_string(",account_id=") << p->dw_account_id;
	p_stream->write_string(",role_id=") << p->dw_role_id;
	p_stream->write_string(",cmd_id=") << p->dw_cmd_id;
	p_stream->write_string(",fund=") << p->n_fund;
	p_stream->write_string(",total_fund=") << p->n_total_funk;
	p_stream->write_string(",time=now()");
}

// 材料日志
BOOL db_handler::create_log_material_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_material, sz_table_postfix);
}

VOID db_handler::insert_material_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_material_log));
}

VOID db_handler::makesql_insert_material_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, tagLogMaterial);
	tagLogMaterial * p = ( tagLogMaterial * ) p_buffer ;  

	p_stream->insert_item(log_of_material, sz_table_postfix);

	p_stream->write_string("guild_id=") << p->dw_guild_id;
	p_stream->write_string(",account_id=") << p->dw_account_id;
	p_stream->write_string(",role_id=") << p->dw_role_id;
	p_stream->write_string(",cmd_id=") << p->dw_cmd_id;
	p_stream->write_string(",material=") << p->n_material;
	p_stream->write_string(",total_material=") << p->n_total_material;
	p_stream->write_string(",time=now()");
}

//声望日志
BOOL db_handler::create_log_reputation_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_reputation, sz_table_postfix);
}

VOID db_handler::insert_reputation_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_reputation_log));
}

VOID db_handler::makesql_insert_reputation_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, tagLogReputation);
	tagLogReputation * p = ( tagLogReputation * ) p_buffer ;  

	p_stream->insert_item(log_of_reputation, sz_table_postfix);

	p_stream->write_string("guild_id=") << p->dw_guild_id;
	p_stream->write_string(",account_id=") << p->dw_account_id;
	p_stream->write_string(",role_id=") << p->dw_role_id;
	p_stream->write_string(",cmd_id=") << p->dw_cmd_id;
	p_stream->write_string(",reputation=") << p->n_reputation;
	p_stream->write_string(",total_reputation=") << p->n_total_reputation;
	p_stream->write_string(",time=now()");
}

// 拍卖日志
BOOL db_handler::create_log_paimai_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_paimai, sz_table_postfix);
}

VOID db_handler::insert_paimai_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_paimai_log));
}

VOID db_handler::makesql_insert_paimai_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	tagLogPaimai* p = (tagLogPaimai*) p_buffer;

	p_stream->insert_item(log_of_paimai, sz_table_postfix);

	p_stream->write_string("serial=") << p->n64_serial;
	p_stream->write_string(",sell_id=") << p->dw_sell_id;
	p_stream->write_string(",buy_id=") << p->dw_buy_id;
	p_stream->write_string(",bidup_id=") << p->dw_bidup_id;
	p_stream->write_string(",bidup=") << p->dw_bidup;
	p_stream->write_string(",chaw=") << p->dw_chaw;
	p_stream->write_string(",time=now()");
}

// 离线修炼日志
BOOL db_handler::create_log_leave_practice_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_leave_pracitce, sz_table_postfix);
}

VOID db_handler::insert_leave_practice_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_leave_pracitce_log));
}

VOID db_handler::makesql_insert_leave_pracitce_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	tagLogLeavePractice* p = (tagLogLeavePractice*)p_buffer;

	p_stream->insert_item(log_of_leave_pracitce, sz_table_postfix);

	p_stream->write_string("roleid=") << p->dw_role_id;
	p_stream->write_string(",droplove=") << p->dw_drop_love;
	p_stream->write_string(",curlove=") << p->dw_cur_love;
	p_stream->write_string(",time=now()");
}

// 序列号礼包
BOOL db_handler::create_log_serial_reward(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_serial_reward, sz_table_postfix);
}

VOID db_handler::insert_serial_reward(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_serial_reward));
}

VOID db_handler::makesql_insert_serial_reward(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	tagLogSerialReward* p = (tagLogSerialReward*)p_buffer;

	p_stream->clear();
	p_stream->insert_item(log_of_serial_reward, sz_table_postfix);

	p_stream->write_string("account_id=") << p->dw_account_id;
	p_stream->write_string(",type=") << p->n_type;
	p_stream->write_string(",serial='").write_string(p->sz_serial).write_string("'");
	p_stream->write_string(",time = now()");
}

// 钱庄日志
BOOL db_handler::create_log_bank_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_bank, sz_table_postfix);
}

VOID db_handler::insert_bank_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_bank_log));
}

VOID db_handler::makesql_insert_bank_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	tagLogBank* p = (tagLogBank*)p_buffer;

	p_stream->insert_item(log_of_bank, sz_table_postfix);

	p_stream->write_string("sell_id=") << p->dw_sell_id;
	p_stream->write_string(",buy_id=") << p->dw_buy_id;
	p_stream->write_string(",bidup_id=") << p->dw_bidup_id;
	p_stream->write_string(",type=") << p->by_type;
	p_stream->write_string(",bidup=") << p->dw_bidup;
	p_stream->write_string(",chaw=") << p->dw_chaw;
	p_stream->write_string(",sell=") << p->dw_sell;
	p_stream->write_string(",time=now()");

}
// GM 指令日志
BOOL db_handler::create_log_gm_cmd_table(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_gm_cmd, sz_table_postfix);
}

VOID db_handler::insert_gm_cmd_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_gm_cmd_log));
}

VOID db_handler::makesql_insert_gm_cmd_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, tagLogGMCmd);
	tagLogGMCmd * p = ( tagLogGMCmd * ) p_buffer ;  

	p_stream->insert_item(log_of_gm_cmd, sz_table_postfix);

	p_stream->write_string("role_id=") << p->dw_role_id;
	p_stream->write_string(",gm_cmd='").write_string(p->sz_gm_cmd).write_string("'");
	p_stream->write_string(",error_code=") << p->dw_error_code;
	p_stream->write_string(",time=now()");
}


// 创建踢人
BOOL db_handler::create_log_kick(LPCSTR sz_table_postfix)
{
	return common_create_log_table(log_of_kick, sz_table_postfix);
}

VOID db_handler::insert_kick_log(LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	common_update_log(sz_table_postfix, p_buffer, MakeDelegateOfDBHandler(&db_handler::makesql_insert_kick_log));
}

VOID db_handler::makesql_insert_kick_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer)
{
	tagKickRole * p = ( tagKickRole * ) p_buffer ;  

	p_stream->insert_item(log_of_kick, sz_table_postfix);

	p_stream->write_string("accountid=") << p->dw_account_id;
	p_stream->write_string(",roleid=") << p->dw_role_id;
	p_stream->write_string(",cmdid=") << p->dw_cmd_id;
	p_stream->write_string(",time=now()");
}

VOID db_handler::undate_online_num(LPVOID p_buffer)
{
	NET_DB2C_update_online_num *p = (NET_DB2C_update_online_num*)p_buffer;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = user_number_db_interface->get_io();
	ASSERT(VALID_POINT(p_stream));
	
	p_stream->clear();
	p_stream->replace_item("platform_online_user");
	p_stream->write_string("server_name = '").write_string(p->szName).write_string("'");
	p_stream->write_string(",online_count = ") << p->nNum;

	
	if(!user_number_db_interface->sql_execute(p_stream))
	{

		
	}


	// 归还sql格式化IO
	user_number_db_interface->return_io(p_stream);

}