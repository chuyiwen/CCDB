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
 *	@file		db_handler
 *	@author		mwh
 *	@date		2010/10/11	initial
 *	@version	0.0.1.0
 *	@brief		数据库接口类
*/

#include "StdAfx.h"
#include "../common/ServerDefine/mall_server_define.h"
#include "db_handler.h"
#include "db_server_handler.h"

db_handler::db_handler()
{
	game_db_interface_ = new odbc::db_interface;
	log_db_interface_ = new odbc::db_interface;
	user_number_db_interface = new odbc::db_interface;

	b_is_init_ok_ = FALSE;

	dw_max_pet_id_	= MIN_PET_ID;
	n_pet_count_		= 0;

	dw_max_role_id_	= 0;
	n_role_count_		= 0;
	n_load_count_limit_	= 0;

	dw_read_count_	= 0;
	dw_write_count_	= 0;

	
}

db_handler::~db_handler()
{
	SAFE_DELETE(game_db_interface_);
	SAFE_DELETE(log_db_interface_);
	SAFE_DELETE(user_number_db_interface);
}

// 启动函数
BOOL db_handler::start(db_server* p_db_server, LPCTSTR sz_init_file, few_connect_server* p_server)
{

	dw_read_count_	= 0;
	dw_write_count_	= 0;
	server_ = p_server;
	p_db_server_ = p_db_server;

	file_container* p_var = new file_container;
	if(!VALID_POINT(p_var))
	{
		ERROR_CLUE_ON(_T("file: %s line :%d Create file_container Failed!\r\n"), _TFILE, _TLINE);
		return FALSE;
	}
	p_var->load(p_db_server_->get_file_system(), sz_init_file);

	//游戏数据接口初始化
	if(!game_db_interface_->init_db(p_var->get_string(_T("host_name db_game")),
						p_var->get_string(_T("user_name db_game")), 
						p_var->get_string(_T("password db_game")), 
						p_var->get_string(_T("db_name db_game")), 
						(int)p_var->get_dword(_T("port db_game"))))
	{
		ERROR_CLUE_ON(_T("file: %s line :%d game_db_interface initial failed\r\n"),_TFILE, _TLINE);
		SAFE_DELETE(game_db_interface_);
		return FALSE;
	}

	// 日志数据接口初始化
	if(!log_db_interface_->init_db(p_var->get_string(_T("host_name db_log")), 
						p_var->get_string(_T("user_name db_log")), 
						p_var->get_string(_T("password db_log")), 
						p_var->get_string(_T("db_name db_log")), 
						(int)p_var->get_dword(_T("port db_log"))))
	{
		ERROR_CLUE_ON(_T("file: %s line :%d log_db_interface initial failed\r\n"),_TFILE, _TLINE);
		SAFE_DELETE(game_db_interface_);
		SAFE_DELETE(log_db_interface_);
		return FALSE;
	}

	// 人数数据接口初始化
	if(!user_number_db_interface->init_db(p_var->get_string(_T("host_name db_user_number")), 
		p_var->get_string(_T("user_name db_user_number")), 
		p_var->get_string(_T("password db_user_number")), 
		p_var->get_string(_T("db_name db_user_number")), 
		(int)p_var->get_dword(_T("port db_user_number"))))
	{
		ERROR_CLUE_ON(_T("file: %s line :%d db_user_number initial failed\r\n"),_TFILE, _TLINE);
		SAFE_DELETE(game_db_interface_);
		SAFE_DELETE(log_db_interface_);
		SAFE_DELETE(user_number_db_interface);
		return FALSE;
	}

	p_var->clear();
	SAFE_DELETE(p_var);

	get_role_count_and_max_role_id(n_role_count_, dw_max_role_id_);
	get_yuanbao_account_count(n_account_count_);
	get_yuanbao_order_count(n_order_count_);
	get_activity_count(n_activity_count_);
	get_yuanbao_max_order_index(dw_max_order_index_);	
	get_pet_count_and_max_pet_id(n_pet_count_, dw_max_pet_id_);
	get_group_purchase_info_count_size(n_group_purchase_info_count_, n_group_purchase_info_size_);

	// 初始化完成
	b_is_init_ok_ = TRUE;

	return TRUE;
}

// 销毁函数
VOID db_handler::destroy()
{
	if(b_is_init_ok_)
	{
		dw_read_count_	= 0;
		dw_write_count_	= 0;
		b_is_init_ok_		= FALSE;

		game_db_interface_->shut_down();
		log_db_interface_->shut_down();
		user_number_db_interface->shut_down();
	}
}
// 获取roledata记录条数和最大RoleID
VOID db_handler::get_role_count_and_max_role_id(int &n_count, DWORD &dw_max_role_id)
{
	n_count = 0;
	dw_max_role_id = 0;

	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	p_stream->select_item("role_data", "count(role_id),max(role_id)");

	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		n_count = (*p_result)[0].get_int();
		dw_max_role_id = (*p_result)[1].get_dword();
	}

	++dw_read_count_;

	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);
}

// 修正物品世界唯一最大/最小索引
VOID db_handler::get_world_max_min_serial(int64 &n64_max, int64 &n64_min)
{
	int64 n64_max_serial = MIN_ITEM_SERIAL_INTERNAL;
	int64 n64_min_serial = MAX_ITEM_SERIAL_OUTTER;

	n64_max = n64_max_serial;
	n64_min = n64_min_serial;

	get_world_max_min_serial("item", n64_max_serial, n64_min_serial);
	n64_max = max(n64_max_serial, n64_max);
	n64_min = min(n64_min_serial, n64_min);

	get_world_max_min_serial("item_del", n64_max_serial, n64_min_serial);
	n64_max = max(n64_max_serial, n64_max);
	n64_min = min(n64_min_serial, n64_min);

	get_world_max_min_serial("item_baibao", n64_max_serial, n64_min_serial);
	n64_max = max(n64_max_serial, n64_max);
	n64_min = min(n64_min_serial, n64_min);
}

// 获取最大和最小序列号
VOID db_handler::get_world_max_min_serial(const char* sz_table, int64 &n64Max, int64 &n64Min)
{
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	p_stream->select_item(sz_table, "max(serial),min(serial)");
	p_stream->where_item();
	p_stream->write_string("serial>") << 0;

	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		n64Max = (*p_result)[0].get_long();
		n64Min = (*p_result)[1].get_long();
	}

	++dw_read_count_;

	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);
}

// 获取邮件最大ID
VOID db_handler::get_max_mail_serial(const char* sz_table, DWORD& dwMax)
{
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	p_stream->select_item(sz_table, "max(mail_id)");
	p_stream->where_item();
	p_stream->write_string("mail_id>") << 0;

	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		dwMax = (DWORD)(*p_result)[0].get_long();
	}

	++dw_read_count_;

	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);

}
// 获取元宝账户数量
VOID db_handler::get_yuanbao_account_count(int &n_account_count)
{
	n_account_count = 0;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	// 拼合SQL语句
	p_stream->select_item("yuanbao_account", "count(role_id)");

	// 执行SQL语句
	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		n_account_count = (*p_result)[0].get_int();
	}

	++dw_read_count_;

	// 归还IO到IO池，删除结果集
	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);
}

// 获取元宝订单数量
VOID db_handler::get_yuanbao_order_count(int &n_order_count)
{
	n_order_count = 0;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	// 拼合SQL语句
	p_stream->select_item("yuanbao_order", "count(id)");
	p_stream->where_item();
	p_stream->write_string("order_mode=") << 0;

	// 执行SQL语句
	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		n_order_count = (*p_result)[0].get_int();
	}

	++dw_read_count_;

	// 归还IO到IO池，删除结果集
	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);
}

// 获得角色元宝订单数
VOID db_handler::get_one_role_yuanbao_order_count(DWORD dw_role_id, int &n_order_number)
{
	n_order_number = 0;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	// 拼合SQL语句
	p_stream->select_item("yuanbao_order", "count(id)");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;

	// 执行SQL语句
	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		n_order_number = (*p_result)[0].get_int();
	}

	++dw_read_count_;

	// 归还IO到IO池，删除结果集
	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);
}

// 获得活动数量
VOID db_handler::get_activity_count(int &n_activity_count)
{
	n_activity_count = 0;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	// 拼合SQL语句
	p_stream->select_item("activity", "count(id)");

	// 执行SQL语句
	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		n_activity_count = (*p_result)[0].get_int();
	}

	++dw_read_count_;

	// 归还IO到IO池，删除结果集
	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);
}

// 获得元宝订单最大索引
VOID db_handler::get_yuanbao_max_order_index( DWORD& dw_max_index )
{
	dw_max_index	= 0;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	// 拼合SQL语句
	p_stream->select_item("yuanbao_order", "max(id)");

	// 执行SQL语句
	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		dw_max_index = (*p_result)[0].get_dword();
	}

	++dw_read_count_;

	// 归还IO到IO池，删除结果集
	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);
}

// 获取宠物总数和最大ID
VOID db_handler::get_pet_count_and_max_pet_id( int &n_count, DWORD &dw_max_pet_id )
{
	n_count = 0;
	dw_max_pet_id = MIN_PET_ID;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	// 拼合SQL语句
	p_stream->select_item("pet_data", "count(pet_id),max(pet_id)");

	// 执行SQL语句
	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		n_count = (*p_result)[0].get_int();
		dw_max_pet_id = (*p_result)[1].get_dword();
		if (!IS_PET(dw_max_pet_id))
		{
			dw_max_pet_id = MIN_PET_ID;
		}
	}

	++dw_read_count_;

	// 归还IO到IO池，删除结果集
	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);
}
// 获取团购总数和数据大小
VOID db_handler::get_group_purchase_info_count_size( int &n_gp_info_number, int &n_data_size )
{
	n_gp_info_number	= 0;
	n_data_size	= 0;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	// 拼合SQL语句
	p_stream->select_item("group_purchase", "participator_list");

	// 执行SQL语句
	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		n_gp_info_number = p_result->get_row_count();
		n_data_size = (sizeof(s_gp_info)-sizeof(DWORD)) * n_gp_info_number;

		for (int i=0; i<n_gp_info_number; i++)
		{
			n_data_size += (*p_result)[0].get_length();

			p_result->next_row();
		}
	}

	++dw_read_count_;

	// 归还IO到IO池，删除结果集
	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);
}
