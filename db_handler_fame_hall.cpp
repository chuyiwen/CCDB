/*******************************************************************************

Copyright 2010 by tiankong Interactive Game Co., Ltd.
All rights reserved.

This software is the confidential and proprietary information of
tiankong Interactive Game Co., Ltd. ('Confidential Information'). You shall
not disclose such Confidential Information and shall use it only in
accordance with the terms of the license agreement you entered into with
tiankong Interactive  Co., Ltd.

*******************************************************************************/

// 名人堂

#include "StdAfx.h"
#include "../common/ServerDefine/famehall_server_define.h"
#include "db_handler.h"

// 家族珍宝
BOOL db_handler::load_act_treasures( LPVOID& p_buffer , int& n_total_num)
{
	common_select(p_buffer, &n_total_num, INVALID_VALUE, INVALID_VALUE, 
				MakeDelegateOfDBHandler(&db_handler::makesql_load_act_treasures), 
				MakeDelegateOfDBHandler(&db_handler::procres_load_act_treasures));
	return TRUE;
}

VOID db_handler::makesql_load_act_treasures(odbc::sql_language_disposal *p_stream, DWORD nClanType, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("famehall_act_treasure", "clan_type,treasure_id,act_name_id");
	p_stream->write_string(" order by clan_type asc,act_time asc;");
}

VOID db_handler::procres_load_act_treasures( OUT LPVOID& p_buffer, OUT int *p_number, odbc::execute_result *p_result )
{

	//M_trans_pointer(p, p_buffer, NET_DB2S_get_act_treasure_list);
	NET_DB2S_get_act_treasure_list * p = ( NET_DB2S_get_act_treasure_list * ) p_buffer ;  
	ASSERT_P_VALID(p);
	ASSERT_P_VALID(p_number);
	*p_number = p_result->get_row_count();
	ASSERT(*p_number > 0);

	int n_clantype = ECLT_BEGIN,  n_index = 0;
	for(; n_index < *p_number; )
	{
		n_clantype = (*p_result)[0].get_dword();
		++(p->nRecNum[n_clantype]);
		p->treasureData[n_index].n16TreasureID	= (*p_result)[1].get_short();
		p->treasureData[n_index].dwNamePrefixID	= (*p_result)[2].get_dword();
		++n_index; p_result->next_row();
	} 

	ASSERT(*p_number == n_index);
}


// 入伙名人堂记录
BOOL db_handler::load_fame_hall_snaps(LPVOID& p_buffer, int& n_total_num)
{
	common_select(p_buffer, &n_total_num, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_fame_hall_snaps), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_fame_hall_snaps));
	return TRUE;
}
VOID db_handler::makesql_load_fame_hall_snaps(odbc::sql_language_disposal *p_stream, DWORD dwClanType, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("famehall_entersnap", "clan_type,role_id,enter_name_id,enter_time");
	p_stream->write_string(" order by clan_type asc,enter_time asc");

}
VOID db_handler::procres_load_fame_hall_snaps(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	//M_trans_pointer(p, p_buffer, NET_DB2S_get_fame_hall_enter_snap);
	NET_DB2S_get_fame_hall_enter_snap * p = ( NET_DB2S_get_fame_hall_enter_snap * ) p_buffer ;  

	ASSERT(p_number != NULL);
	*p_number = p_result->get_row_count();

	int n_clantype = ECLT_BEGIN, n_index = 0;
	for(; n_index < *p_number; )
	{
		n_clantype = (*p_result)[0].get_dword();
		++(p->nRecNum[n_clantype]);

		p->enterSnap[n_index].dw_role_id		= (*p_result)[1].get_dword();
		p->enterSnap[n_index].dwEnterNameID	= (*p_result)[2].get_dword();
		p->enterSnap[n_index].dwtEnterTime	= (*p_result)[3].get_dword();
		++n_index; p_result->next_row();
	} 

	ASSERT(*p_number == n_index);

}
// 获取声望重置时间
BOOL db_handler::load_rep_reset_timestamp(LPVOID& p_buffer)
{
	//M_trans_pointer(p, p_buffer, NET_DB2S_get_rep_reset_times_tamp);
	NET_DB2S_get_rep_reset_times_tamp * p = ( NET_DB2S_get_rep_reset_times_tamp * ) p_buffer ;  

	PVOID p_out = p->dwResetTime;
	int n_count = 0;
	return common_select(p_out, &n_count, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_reset_timestamp), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_rep_reset_timestamp));
}
VOID db_handler::makesql_load_reset_timestamp(odbc::sql_language_disposal *p_stream, DWORD dwClanType, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("famehall_rep_reset_timestamp", "clan_type,reset_timestamp");
}
VOID db_handler::procres_load_rep_reset_timestamp(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	//M_trans_pointer(p_dw_time, p_buffer, tagDWORDTime);
	tagDWORDTime * p_dw_time = ( tagDWORDTime * ) p_buffer ;  

	ASSERT(VALID_POINT(p_number));
	*p_number = p_result->get_row_count();
	for(int n = 0; n < *p_number; ++n)
	{
		int n_clantype = (*p_result)[0].get_dword();
		ASSERT(JDG_VALID(ECLT, n_clantype));
		p_dw_time[n_clantype] = (*p_result)[1].get_dword();
		p_result->next_row();
	} 
}

// 声望排名榜
BOOL db_handler::load_rep_rank(BYTE b_clan_type, PVOID p_buffer, int &n_count)
{
	PVOID p_out = p_buffer;
	BOOL b_ret = common_select(p_out, &n_count, b_clan_type, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_rep_rank), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_rep_rank));
	ASSERT(n_count * sizeof(tagRepRankData) == reinterpret_cast<BYTE *>(p_out) - reinterpret_cast<BYTE *>(p_buffer));
	return b_ret;
}
VOID db_handler::makesql_load_rep_rank(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	static const char* cssz_column_name[ECLT_NUM] = {	"rep_xuanyuan",	"rep_shennong",	"rep_fuxi",	"rep_sanmiao",
		"rep_jiuli",	"rep_yuezhi",	"rep_nvwa",	"rep_gonggong"
	};
	BYTE by_clantype = static_cast<BYTE>(dw_account_id);
	ASSERT(JDG_VALID(ECLT, by_clantype));
	const char* csz_column = cssz_column_name[by_clantype];

	std::string str_sql;
	str_sql.append("role_id,");
	str_sql.append(csz_column);

	p_stream->select_item("clan_data", str_sql.c_str());

	p_stream->where_item();
	p_stream->write_string(csz_column).write_string(">=0 order by ").write_string(csz_column).write_string(" desc limit ") << MAX_REP_RANK_NUM;
}


VOID db_handler::procres_load_rep_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(VALID_POINT(p_buffer));
	//M_trans_pointer(p, p_buffer, tagRepRankData);
	tagRepRankData * p = ( tagRepRankData * ) p_buffer ;  

	ASSERT(VALID_POINT(p_number));
	*p_number = p_result->get_row_count();
	ASSERT(*p_number <= MAX_REP_RANK_NUM);

	for (int nIndex = 0; nIndex < *p_number; ++nIndex)
	{
		p[nIndex].dw_role_id		= (*p_result)[0].get_dword();
		p[nIndex].dwReputValue	= (*p_result)[1].get_int();

		p_result->next_row();
	}
	p_buffer = reinterpret_cast<BYTE *>(p_buffer) + sizeof(tagRepRankData) * (*p_number);
}

// 新增进入名人堂记录
BOOL db_handler::insert_fame_hall_enter_snap(PVOID p_buffer)
{	
	return common_update(INVALID_VALUE, p_buffer, 1, sizeof(NET_DB2C_insert_fame_hall_snap), 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_fame_hall_enter_snap));
}

VOID db_handler::makesql_insert_fame_hall_enter_snap(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, NET_DB2C_insert_fame_hall_snap);
	NET_DB2C_insert_fame_hall_snap * p = ( NET_DB2C_insert_fame_hall_snap * ) p_buffer ;  

	p_stream->clear();
	p_stream->insert_item("famehall_entersnap");
	p_stream->write_string("role_id=") << p->dw_role_id;
	p_stream->write_string(",enter_name_id=") << p->dwEnterNameID;
	p_stream->write_string(",clan_type=") << p->byClanType;
	p_stream->write_string(",enter_time=") << p->dwEnterTime;
}

// 新增家族珍宝
BOOL db_handler::insert_act_treasure(PVOID p_buffer)
{
	return common_update(INVALID_VALUE,p_buffer, 1, sizeof(NET_DB2C_insert_act_clan_treasure), 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_act_treasure));
}

VOID db_handler::makesql_insert_act_treasure(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, NET_DB2C_insert_act_clan_treasure);
	NET_DB2C_insert_act_clan_treasure * p = ( NET_DB2C_insert_act_clan_treasure * ) p_buffer ;  

	p_stream->clear();
	p_stream->insert_item("famehall_act_treasure");
	p_stream->write_string("treasure_id=") << p->u16TreasureID;
	p_stream->write_string(",act_name_id=") << p->dwActNameID;
	p_stream->write_string(",role_id=") << p->dw_role_id;
	p_stream->write_string(",clan_type=") << p->byClanType;
	p_stream->write_string(",act_time=") << p->dwActTime;
}

// 更新声望重置时间
BOOL db_handler::update_rep_reset_time(BYTE b_clan_type, DWORD dw_reset_time)
{
	return common_update(b_clan_type, &dw_reset_time, 1, sizeof(DWORD), 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_rep_reset_timestamp));
}

VOID db_handler::makesql_load_rep_reset_timestamp(odbc::sql_language_disposal *p_stream, DWORD dwClanType, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, DWORD);
	DWORD * p = ( DWORD * ) p_buffer ;  

	p_stream->clear();
	p_stream->insert_item("famehall_rep_reset_timestamp");
	p_stream->write_string("clan_type=") << dwClanType;
	p_stream->write_string(",reset_timestamp=") << *p;
	p_stream->write_string(" ON DUPLICATE KEY UPDATE");
	p_stream->write_string(" reset_timestamp=") << *p;
}