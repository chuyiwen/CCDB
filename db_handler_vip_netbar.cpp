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
#include "../common/ServerDefine/vip_netbar_server_define.h"
#include "db_handler.h"

BOOL db_handler::update_vip_net_bar_player(DWORD dw_account_id, DWORD dw_login_time)
{
	return common_update(dw_account_id, &dw_login_time, 1, sizeof(DWORD), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_vip_net_bar_player));
}

VOID db_handler::makesql_update_vip_net_bar_player(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, PVOID p_buffer)
{
	p_stream->replace_item("vip_netbar");
	p_stream->write_string("account_id=") << dw_account_id;
	p_stream->write_string(",login_time=") << *((DWORD*)p_buffer);
}

BOOL db_handler::load_vip_net_bar(OUT PVOID p_buffer, DWORD dw_date)
{
	M_trans_pointer(p_vip_net_bar_data, p_buffer, s_db_vnb_players);
	PVOID pTmp = p_vip_net_bar_data->dw_account_ids;

	common_select(pTmp, &p_vip_net_bar_data->n_his_players, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_history_vip_net_bar_players), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_vip_net_bar_players));

	common_select(pTmp, &p_vip_net_bar_data->n_todays_players, dw_date, INVALID_VALUE,
		MakeDelegateOfDBHandler(&db_handler::makesql_load_today_vip_net_bar_players), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_vip_net_bar_players));

	return TRUE;
}

VOID db_handler::makesql_load_history_vip_net_bar_players(odbc::sql_language_disposal *p_stream, DWORD dw_date, DWORD dw_data)
{
	p_stream->select_item("vip_netbar", "account_id, login_time");
}

VOID db_handler::makesql_load_today_vip_net_bar_players(odbc::sql_language_disposal *p_stream, DWORD dw_date, DWORD dw_data)
{
	p_stream->select_item("vip_netbar", "account_id, login_time");
	p_stream->where_item();
	p_stream->write_string("login_time=")<<dw_date;
}

VOID db_handler::procres_load_vip_net_bar_players(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	M_trans_pointer(p_account_ids, p_buffer, DWORD);
	*p_number = p_result->get_row_count();

	for(INT16 i=0; i<*p_number; ++i)
	{
		p_account_ids[i]		= (*p_result)[0].get_dword();
		p_result->next_row();
	}

	p_buffer = &p_account_ids[*p_number];
}
