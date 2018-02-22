/*******************************************************************************

Copyright 2010 by tiankong Interactive Game Co., Ltd.
All rights reserved.

This software is the confidential and proprietary information of
tiankong Interactive Game Co., Ltd. ('Confidential Information'). You shall
not disclose such Confidential Information and shall use it only in
accordance with the terms of the license agreement you entered into with
tiankong Interactive  Co., Ltd.

*******************************************************************************/
/**
 *	@file		db_handler_account
 *	@author		mwh
 *	@date		2011/02/12	initial
 *	@version	0.0.1.0
 *	@brief		账号
*/


#include "StdAfx.h"
#include "../../common/WorldDefine/currency_define.h"
#include "../common/ServerDefine/role_data_server_define.h"
#include "../common/ServerDefine/common_server_define.h"
#include "db_handler.h"

// 读取账号通用
BOOL db_handler::load_one_account_common(LPVOID p_buffer, DWORD dw_account_id)
{
	return common_select(p_buffer, 
						 NULL, 
						 dw_account_id, 
						 INVALID_VALUE, 
						 MakeDelegateOfDBHandler(&db_handler::makesql_load_one_account_common), 
				         MakeDelegateOfDBHandler(&db_handler::procres_load_one_account_common));
}

VOID db_handler::makesql_load_one_account_common(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("account_common", 
		"safecode_crc,reset_time,bag_password_crc,baibao_yuanbao,	\
		ware_size,ware_silver,warestep,yuanbao_recharge,			\
		IsReceive,total_recharge,receive_type,receive_type_ex,		\
		web_type,score,robort");
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}

VOID db_handler::procres_load_one_account_common(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	//M_trans_pointer(p, p_buffer, s_account_common);
	s_account_common * p = ( s_account_common * ) p_buffer ;  
	p->s_safe_code_.dw_safe_code_crc	= (*p_result)[0].get_dword();
	p->s_safe_code_.dw_reset_time	= (*p_result)[1].get_dword();
	p->dw_bag_password_crc_				= (*p_result)[2].get_dword();
	p->n_baibao_yuanbao_			= (*p_result)[3].get_int();
	p->n16_ware_size_				= (*p_result)[4].get_int();
	p->n64_ware_silver_			= (*p_result)[5].get_long();
	p->n16_ware_step			= (*p_result)[6].get_short();
	p->n_yuanbao_recharge		= (*p_result)[7].get_int();
	p->b_receive				= (*p_result)[8].get_int();
	p->dw_total_recharge		= (*p_result)[9].get_dword();
	p->n16_receive_type			= (*p_result)[10].get_short();
	p->dw_receive_type			= (*p_result)[11].get_dword();
	p->dw_web_type				= (*p_result)[12].get_dword();
	p->n32_score				= (*p_result)[13].get_int();
	p->b_Robort					= (*p_result)[14].get_int();
	
}
// 对account_common表加锁
BOOL db_handler::lock_table_account_common()
{
	return lock_tables("account_common");
}
// 新增账号记录
BOOL db_handler::insert_account_common(DWORD dw_account_id,const char* sz_account)
{
	return common_update(dw_account_id, (PVOID)sz_account, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_insert_account_common));
}

VOID db_handler::makesql_insert_account_common(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	const char* sz_account = (char*)p_buffer;

	odbc::tag_mysql_connect* pCon = game_db_interface_->get_idlesse_connect();

	p_stream->insert_item("account_common");
	p_stream->write_string("account_id=") << dw_account_id;
	p_stream->write_string(",account_name='").write_string(sz_account, pCon).write_string("'");
	/*if(dw_account_id <= 25708)
		p_stream->write_string(",IsReceive=0");*/

	game_db_interface_->return_use_connect(pCon);
}

// 更新安全码
BOOL db_handler::update_safe_code(DWORD dw_account_id, DWORD dw_safe_code_crc)
{
	return common_update(dw_account_id, &dw_safe_code_crc, 1, sizeof(DWORD), MakeDelegateOfDBHandler(&db_handler::makesql_update_safe_code));
}

VOID db_handler::makesql_update_safe_code(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	p_stream->update_item("account_common");
	p_stream->write_string("safecode_crc=") << *(DWORD*)p_buffer;
	p_stream->write_string(",reset_time=") << (DWORD)INVALID_VALUE;

	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}
// 设置安全码重置时间
BOOL db_handler::update_safe_code_reset_time(DWORD dw_account_id, DWORD dw_reset_time)
{
	return common_update(dw_account_id, &dw_reset_time, 1, sizeof(DWORD), MakeDelegateOfDBHandler(&db_handler::makesql_update_safe_code_reset_time));
}

VOID db_handler::makesql_update_safe_code_reset_time(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	p_stream->update_item("account_common");
	p_stream->write_string("reset_time=") << *(DWORD*)p_buffer;

	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}

// 更新背包密码
BOOL db_handler::update_bag_password(DWORD dw_account_id, DWORD dw_bag_password)
{
	return common_update(dw_account_id, &dw_bag_password, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_bag_password));
}

VOID db_handler::makesql_update_bag_password(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	p_stream->update_item("account_common");
	p_stream->write_string("bag_password_crc=") << *(DWORD*)p_buffer;

	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}
// 跟新角色仓库上限
BOOL db_handler::update_ware_size(DWORD dw_account_id, LPVOID p_buffer)
{
	return common_update(dw_account_id, p_buffer, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_ware_size));
}
VOID db_handler::makesql_update_ware_size(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	NET_DB2C_ware_size_update* p = (NET_DB2C_ware_size_update*)p_buffer;

	p_stream->update_item("account_common");
	p_stream->write_string("ware_size=") << p->n16_ware_size;
	p_stream->write_string(",warestep=") << p->n16_ware_step;

	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}

// 更新包裹非绑定币
BOOL db_handler::update_bag_money(DWORD	dw_role_id,	LPVOID p_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, 0,MakeDelegateOfDBHandler(&db_handler::makesql_update_bag_money) );
}

VOID db_handler::makesql_update_bag_money(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	NET_DB2C_bag_money_update* p = (NET_DB2C_bag_money_update*)p_buffer;

	p_stream->clear();

	p_stream->update_item("role_data");
	p_stream->write_string("bag_gold=") << p->n_bag_gold_;
	p_stream->write_string(",bag_silver=") << p->n_bag_silver_;
	p_stream->write_string(",bag_copper=") << p->n_bag_copper_;
}

// 更新仓库货币
BOOL db_handler::update_ware_money(DWORD dw_account_id, int64 n64_ware_silver)
{
	return common_update(dw_account_id, (LPVOID)&n64_ware_silver, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_ware_money));
}

VOID db_handler::makesql_update_ware_money(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	p_stream->update_item("account_common");
	p_stream->write_string("ware_silver=ware_silver+") << *(int64*)p_buffer;
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}
// 更新元宝
BOOL db_handler::update_baibao_yuanbao(DWORD dw_account_id, LPVOID p_buffer)
{
	return common_update(dw_account_id, p_buffer, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_baibao_yuanbao));
}
VOID db_handler::makesql_update_baibao_yuanbao(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	p_stream->update_item("account_common");
	p_stream->write_string("baibao_yuanbao=baibao_yuanbao+") << *(int*)p_buffer;

	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}

// 更新积分
BOOL db_handler::update_exchang_volume(DWORD dw_account_id, LPVOID p_buffer)
{
	return common_update(dw_account_id, p_buffer, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_exchange_volume));
}

VOID db_handler::makesql_update_exchange_volume(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	p_stream->update_item("account_common");
	p_stream->write_string("score=score+") << *(int*)p_buffer;

	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}

// 更新领取标志
BOOL db_handler::update_receive(DWORD dw_account_id, LPVOID p_buffer)
{
	return common_update(dw_account_id, p_buffer, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_receive));
}

VOID db_handler::makesql_update_receive(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	p_stream->update_item("account_common");
	p_stream->write_string("IsReceive=1");
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}

// 更新领取标志
BOOL db_handler::update_receive_ex(DWORD dw_account_id, LPVOID p_buffer)
{
	return common_update(dw_account_id, p_buffer, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_receive_ex));
}

VOID db_handler::makesql_update_receive_ex(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	p_stream->update_item("account_common");
	p_stream->write_string("receive_type_ex=") << *((DWORD*)p_buffer);
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}

// 更新充值总量
BOOL db_handler::update_total_recharge(DWORD dw_account_id, LPVOID p_buffer)
{
	return common_update(dw_account_id, p_buffer, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_total_recharge));
}

VOID db_handler::makesql_update_total_recharge(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	p_stream->update_item("account_common");
	p_stream->write_string("total_recharge=total_recharge+") << *((INT32*)p_buffer);
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}

// 更新网页领奖标志
BOOL db_handler::update_web_receive(DWORD dw_account_id, LPVOID p_buffer)
{
	return common_update(dw_account_id, p_buffer, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_web_receive));
}

VOID db_handler::makesql_update_web_receive(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	p_stream->update_item("account_common");
	p_stream->write_string("web_type=0");
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}

// 删除序列号礼包
BOOL db_handler::delete_serial_reward(DWORD dw_account_id, LPVOID p_buffer)
{
	return common_update(dw_account_id, p_buffer, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_delete_serial_reward));
}

VOID db_handler::makesql_delete_serial_reward(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	odbc::tag_mysql_connect* pCon = game_db_interface_->get_idlesse_connect();

	const char* sz_serial = (char*)p_buffer;
	p_stream->clear();
	p_stream->delete_item("serial_reward");
	p_stream->where_item();
	p_stream->write_string("serial='").write_string(sz_serial, pCon).write_string("'");

	game_db_interface_->return_use_connect(pCon);
	/*odbc::tag_mysql_connect* pCon = game_db_interface_->get_idlesse_connect();

	const char* sz_serial = (char*)p_buffer;
	p_stream->clear();
	p_stream->delete_item("serial_reward");
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
	p_stream->write_string(" and ");
	p_stream->write_string("serial='").write_string(sz_serial, pCon).write_string("'");

	game_db_interface_->return_use_connect(pCon);*/
}