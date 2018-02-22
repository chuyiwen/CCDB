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
#include "db_handler.h"

#include "../common/ServerDefine/bank_server_define.h"

// 获取钱庄最大编号
BOOL db_handler::get_bank_max_id(DWORD&	dw_bank_id)
{
	DWORD dwMax = 0;
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	p_stream->select_item("bank", "max(id)");
	p_stream->where_item();
	p_stream->write_string("id>") << 0;

	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		dwMax = (DWORD)(*p_result)[0].get_long();
	}

	++dw_read_count_;

	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);

	dw_bank_id = max(dw_bank_id, dwMax);

	return TRUE;
}

// 保存钱庄信息
BOOL db_handler::insert_bank(NET_S2DB_insert_bank* p_bank)
{
	return common_update(INVALID_VALUE, p_bank, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_bank));
}

VOID db_handler::makesql_insert_bank(odbc::sql_language_disposal* p_stream, DWORD dw_bank_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, NET_S2DB_insert_bank);

	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->insert_item("bank");

	p_stream->write_string("id=") << p->st_bank.dw_id;
	p_stream->write_string(",bidup=") << p->st_bank.dw_bidup;
	p_stream->write_string(",bidup_id=") << p->st_bank.dw_bidup_id;
	p_stream->write_string(",chaw=") << p->st_bank.dw_chaw;
	p_stream->write_string(",sell_id=") << p->st_bank.dw_sell_id;
	p_stream->write_string(",timetype=") << p->st_bank.by_time_type;
	p_stream->write_string(",sell=") << p->st_bank.dw_sell;
	p_stream->write_string(",type=") << p->st_bank.by_type;

	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->st_bank.dw_begin_time))
	{
		p_stream->write_string(",begintime='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}

	game_db_interface_->return_use_connect(p_conn);
}

// 删除钱庄信息
BOOL db_handler::delete_bank(NET_S2DB_delete_bank* p_bank)
{
	return common_update(p_bank->dw_bank_id, NULL, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_bank));
}

VOID db_handler::makesql_delete_bank(odbc::sql_language_disposal* p_stream, DWORD dw_bank_id, LPVOID p_buffer)
{
	p_stream->delete_item("bank");
	p_stream->where_item();
	p_stream->write_string("id=") << dw_bank_id;
}

// 更新钱庄
BOOL db_handler::update_bank(NET_S2DB_update_bank* p_bank)
{
	return common_update(INVALID_VALUE, &p_bank->st_bank, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_update_bank));
}

VOID db_handler::makesql_update_bank(odbc::sql_language_disposal* p_stream, DWORD dw_bank_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, tag_bank);

	p_stream->update_item("bank");
	p_stream->write_string("bidup=") << p->dw_bidup;
	p_stream->write_string(",bidup_id=") << p->dw_bidup_id;
	p_stream->write_string(",chaw=") << p->dw_chaw;
	p_stream->write_string(",sell_id=") << p->dw_sell_id;
	p_stream->write_string(",timetype=") << p->by_time_type;
	p_stream->write_string(",sell=") << p->dw_sell;
	p_stream->write_string(",type=") << p->by_type;

	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_begin_time))
	{
		p_stream->write_string(",begintime='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}
	p_stream->where_item();
	p_stream->write_string("id=") << p->dw_id;
}

// 读取所有钱庄信息
BOOL db_handler::load_all_bank(LPVOID p_buffer)
{
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE,
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_bank),
		MakeDelegateOfDBHandler(&db_handler::procres_load_all_bank));
}

VOID db_handler::makesql_load_all_bank(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->select_item("bank",
		"id,"		"bidup,"		"bidup_id,"		"type,"
		"sell,"		"chaw,"			"sell_id,"		"begintime,"
		"timetype");
}

VOID db_handler::procres_load_all_bank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	INT	n_total_count = p_result->get_row_count();

	INT	n_max_count = n_total_count;
	INT	n_cur_count = 0;

	while(n_max_count > 0)
	{
		if(n_max_count > MAX_LOAD_BANK_NUM)
		{
			n_cur_count = MAX_LOAD_BANK_NUM;
		}
		else
		{
			n_cur_count = n_max_count;
		}

		memset(p_buffer, 0, sizeof(p_buffer));
		NET_DB2S_load_all_bank* p_send = (NET_DB2S_load_all_bank*)p_buffer;
		M_msg_init(p_send, NET_DB2S_load_all_bank);

		M_trans_pointer(p, p_send->st_bank, tag_bank);

		for(INT i = 0; i < n_cur_count; i++)
		{
			p->dw_id = (*p_result)[0].get_dword();
			p->dw_bidup = (*p_result)[1].get_dword();
			p->dw_bidup_id = (*p_result)[2].get_dword();
			p->by_type = (*p_result)[3].get_byte();
			p->dw_sell = (*p_result)[4].get_dword();
			p->dw_chaw = (*p_result)[5].get_dword();
			p->dw_sell_id = (*p_result)[6].get_dword();
			DataTime2DwordTime(p->dw_begin_time, (*p_result)[7].get_string(), (*p_result)[7].get_length());
			p->by_time_type = (*p_result)[8].get_byte();

			p_result->next_row();
			++p;
		}
		p_send->n_num = n_cur_count;
		p_send->dw_size = sizeof(NET_DB2S_load_all_bank) + (n_cur_count-1)*sizeof(tag_bank);

		server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

		n_max_count -= n_cur_count;
	}
}