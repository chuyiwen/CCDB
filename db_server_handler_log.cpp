/*******************************************************************************

Copyright 2010 by tiankong Interactive Game Co., Ltd.
All rights reserved.

This software is the confidential and proprietary information of
tiankong Interactive Game Co., Ltd. ('Confidential Information'). You shall
not disclose such Confidential Information and shall use it only in
accordance with the terms of the license agreement you entered into with
tiankong Interactive  Co., Ltd.

*******************************************************************************/

#include "StdAfx.h"

#include "db_server_handler.h"
#include "db_handler.h"
#include "../common/ServerDefine/log_server_define.h"

VOID db_server::create_current_log_table()
{	
	print_message(_T("\n\n"));

	char sz_time[8] = {'\0'};
	tagDWORDTime dw_time = GetCurrentDWORDTime();
	sprintf_s(sz_time, sizeof(sz_time), "_%02d%02d", dw_time.month, dw_time.day);

	// 创建新表
	if(db_handler_->create_log_money_table(sz_time)
		&& db_handler_->create_log_yuanbao_table(sz_time)
		&& db_handler_->create_log_exvolume_table(sz_time)
		&& db_handler_->create_log_exploits_table(sz_time)
		&& db_handler_->create_log_time_stat_table(sz_time)
		&& db_handler_->create_log_item_table(sz_time)
		&& db_handler_->create_log_item_times_table(sz_time)
		&& db_handler_->create_log_mall_sell_table(sz_time)
		&& db_handler_->create_log_mall_sell_pack_table(sz_time)
		&& db_handler_->create_log_fund_table(sz_time)
		&& db_handler_->create_log_material_table(sz_time)
		&& db_handler_->create_log_reputation_table(sz_time)
		&& db_handler_->create_log_gm_cmd_table(sz_time)
		&& db_handler_->create_log_bank_table(sz_time)
		&& db_handler_->create_log_paimai_table(sz_time)
		&& db_handler_->create_log_leave_practice_table(sz_time)
		&& db_handler_->create_log_serial_reward(sz_time))
		/*&& db_handler_->create_log_kick(sz_time)*/
	{
		st_log_time_ = dw_time;
		sprintf_s(sz_log_file_postfix_, sizeof(sz_log_file_postfix_), sz_time);
	}

	print_message(_T("\n\n"));
}

DWORD db_server::insert_money_log(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_log_silver);
	NET_DB2C_log_silver * p = ( NET_DB2C_log_silver * ) p_msg ;  
	db_handler_->insert_money_log(sz_log_file_postfix_, &p->s_log_silver_);

	return 0;
}

DWORD db_server::insert_yuanbao_log(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_log_yuanbao);
	NET_DB2C_log_yuanbao * p = ( NET_DB2C_log_yuanbao * ) p_msg ;  
	db_handler_->insert_yuanbao_log(sz_log_file_postfix_, &p->s_log_yuanbao_);

	return 0;
}

DWORD db_server::insert_exvolume_log(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_log_exvolume);
	NET_DB2C_log_exvolume * p = ( NET_DB2C_log_exvolume * ) p_msg ;  
	db_handler_->insert_exvolume_log(sz_log_file_postfix_, &p->s_log_ex_volume_);

	return 0;
}

DWORD db_server::insert_log_exploits(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_log_exvolume* p = (NET_DB2C_log_exvolume*) p_msg;

	db_handler_->insert_exploits_log(sz_log_file_postfix_, &p->s_log_ex_volume_);

	return 0;
}

DWORD db_server::insert_time_stat_log(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_log_time_stat);
	NET_DB2C_log_time_stat * p = ( NET_DB2C_log_time_stat * ) p_msg ;  
	db_handler_->insert_time_stat_log(sz_log_file_postfix_, &p->s_log_times_stat_);

	return 0;
}

DWORD db_server::insert_item_log(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_log_item);
	NET_DB2C_log_item * p = ( NET_DB2C_log_item * ) p_msg ;  
	db_handler_->insert_item_log(sz_log_file_postfix_, &p->s_log_item_);

	return 0;
}

DWORD db_server::insert_mall_sell_log(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_log_mall_sell);
	NET_DB2C_log_mall_sell * p = ( NET_DB2C_log_mall_sell * ) p_msg ;  
	db_handler_->insert_mall_sell_log(sz_log_file_postfix_, &p->s_log_mall_sell_);

	return 0;
}

DWORD db_server::insert_mall_sell_pack_log(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_log_mall_sell_pack);
	NET_DB2C_log_mall_sell_pack * p = ( NET_DB2C_log_mall_sell_pack * ) p_msg ;  
	db_handler_->insert_mall_sell_pack_log(sz_log_file_postfix_, &p->s_log_mall_sell_pack_);

	return 0;
}

DWORD db_server::insert_item_times_log(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_log_item_times);
	NET_DB2C_log_item_times * p = ( NET_DB2C_log_item_times * ) p_msg ;  
	db_handler_->insert_item_times_log(sz_log_file_postfix_, &p->s_log_item_times_);

	return 0;
}

DWORD db_server::insert_fund_log(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_log_fund);
	NET_DB2C_log_fund * p = ( NET_DB2C_log_fund * ) p_msg ;  
	db_handler_->insert_fund_log(sz_log_file_postfix_, &p->s_log_fund_);

	return 0;
}

DWORD db_server::insert_material_log(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_log_material);
	NET_DB2C_log_material * p = ( NET_DB2C_log_material * ) p_msg ;  
	db_handler_->insert_material_log(sz_log_file_postfix_, &p->s_log_material_);

	return 0;
}

DWORD db_server::insert_reputation_log(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_log_reputation);
	NET_DB2C_log_reputation * p = ( NET_DB2C_log_reputation * ) p_msg ;  
	db_handler_->insert_reputation_log(sz_log_file_postfix_, &p->s_log_reputation_);

	return 0;
}

DWORD db_server::insert_gm_cmd_log(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_log_gm_cmd);
	NET_DB2C_log_gm_cmd * p = ( NET_DB2C_log_gm_cmd * ) p_msg ;  
	db_handler_->insert_gm_cmd_log(sz_log_file_postfix_, &p->s_log_gm_cmd_);

	return 0;
}

DWORD db_server::insert_bank_log(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_log_bank* p = (NET_DB2C_log_bank*) p_msg;

	db_handler_->insert_bank_log(sz_log_file_postfix_, &p->s_log_bank);

	return 0;
}

DWORD db_server::insert_paimai_log(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_log_paimai* p = (NET_DB2C_log_paimai*) p_msg;

	db_handler_->insert_paimai_log(sz_log_file_postfix_, &p->s_log_paimai);

	return 0;
}

DWORD db_server::insert_leave_pracitice_log(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_log_leave_practice* p = (NET_DB2C_log_leave_practice*)p_msg;

	db_handler_->insert_leave_practice_log(sz_log_file_postfix_, &p->s_log_leave_pracitce);

	return 0;
}

DWORD db_server::insert_serial_reward_log(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_log_serial_reward* p = (NET_DB2C_log_serial_reward*)p_msg;

	db_handler_->insert_serial_reward(sz_log_file_postfix_, &p->s_log_serial_reward);

	return 0;
}

DWORD db_server::insert_kick_log(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_log_kick* p = (NET_DB2C_log_kick*)p_msg;
	
	db_handler_->insert_kick_log(sz_log_file_postfix_, &p->s_log_kick);

	return 0;
}

DWORD db_server::update_online_num(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_online_num* p = (NET_DB2C_update_online_num*)p_msg;

	db_handler_->undate_online_num(p);

	return 0;
}