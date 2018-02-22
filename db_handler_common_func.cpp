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
 *	@file		db_handler_common_func
 *	@author		mwh
 *	@date		2010/10/11	initial
 *	@version	0.0.1.0
 *	@brief		通用方法
*/

#include "StdAfx.h"
#include "db_handler.h"

// 请求结果集,同步操作
BOOL db_handler::common_select(OUT LPVOID &p_buffer, OUT int *p_number, DWORD dw_account_id, 
							   DWORD dw_role_id, func_param3 p_func_format, func_result_proc p_func_procres)
{
	BOOL b_ret = TRUE;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	ASSERT(VALID_POINT(p_stream));

	// 清空流
	p_stream->clear();

	// 拼合SQL语句查询的角色数据
	(p_func_format)(p_stream, dw_account_id, dw_role_id);

	// 执行SQL语句
	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(!VALID_POINT(p_result))
	{
		b_ret = FALSE;
		goto Exit;
	}

	// 检验并保存查询结果
	if (p_result->get_row_count() > 0)
	{
		(p_func_procres)(p_buffer, p_number, p_result);
	}
	else
	{
		if (VALID_POINT(p_number))
		{
			*p_number = 0;
		}
		b_ret = FALSE;
	}

Exit:
#ifdef _DEBUG
	print_message(_T("%s\r\n"), get_tool()->ansi_to_unicode(p_stream->get_buffer()));
	print_message(_T("Results Row Count: %d\n"), VALID_POINT(p_result) ? p_result->get_row_count() : 0);
#endif

	++dw_read_count_;

	// 归还IO到IO池，删除结果集
	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);

	return b_ret;
}
// 更新数据(删除/插入/更新),不需结果,同步操作
BOOL db_handler::common_update(DWORD dw_role_id, LPVOID p_buffer, int n_count, int nSzOne, 
							   func_param3_1 p_func_format, OUT LPVOID *pp_buffer/* = NULL*/)
{
	if(n_count <= 0)
	{
		return TRUE;
	}

	BOOL bRet = TRUE;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	ASSERT(VALID_POINT(p_stream));

	// 清空流
	p_stream->clear();

	char *p_cur_data = (char *)p_buffer;
	for(int i=0; i<n_count; ++i)
	{
		// 拼合SQL语句数据库操作语句,并执行SQL语句
		(p_func_format)(p_stream, dw_role_id, p_cur_data);

#ifdef _DEBUG
		print_message(_T("%s\r\n"), get_tool()->ansi_to_unicode(p_stream->get_buffer()));
#endif

		if(!game_db_interface_->sql_execute(p_stream))
		{
			bRet = FALSE;
		}

		// 重置数据指针
		p_cur_data += nSzOne;

		++dw_write_count_;
	}

	if(pp_buffer != NULL)
	{
		*pp_buffer = p_cur_data;
	}

	// 归还sql格式化IO
	game_db_interface_->return_io(p_stream);

	return bRet;
}
// 日志操作,不需结果集,异步操作(删除/插入/更新)
VOID db_handler::common_update_log(LPCSTR sz_table_postfix, LPVOID p_buffer, func_param3_2 p_func_format)
{
	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = log_db_interface_->get_io();
	ASSERT(VALID_POINT(p_stream));

	// 清空流
	p_stream->clear();

	// 拼合SQL语句数据库操作语句,并执行SQL语句
	(p_func_format)(p_stream, sz_table_postfix, p_buffer);

#ifdef _DEBUG
	print_message(_T("%s\r\n"), get_tool()->ansi_to_unicode(p_stream->get_buffer()));
#endif

	log_db_interface_->add_asynchronism_query(p_stream);
}
// 表锁定
BOOL db_handler::lock_tables(LPCSTR szTableNames)
{
	return common_update(INVALID_VALUE, (LPVOID)szTableNames, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_lock_tables));
}

VOID db_handler::makesql_lock_tables(odbc::sql_language_disposal *p_stream, DWORD dw_reserve, LPVOID p_buffer)
{
	p_stream->clear();

	p_stream->write_string("Lock Tables ");
	p_stream->write_string((LPCSTR)p_buffer);
	p_stream->write_string(" WRITE");
}

// 表解除锁定
BOOL db_handler::unlock_tables()
{
	return common_update(INVALID_VALUE, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_unlock_tables));
}

VOID db_handler::makesql_unlock_tables(odbc::sql_language_disposal *p_stream, DWORD dw_reserve, LPVOID p_buffer)
{
	p_stream->clear();

	p_stream->write_string("Unlock Tables");
}
