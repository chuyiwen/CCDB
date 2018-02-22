/*******************************************************************************

Copyright 2010 by tiankong Interactive Game Co., Ltd.
All rights reserved.

This software is the confidential and proprietary information of
tiankong Interactive Game Co., Ltd. ('Confidential Information'). You shall
not disclose such Confidential Information and shall use it only in
accordance with the terms of the license agreement you entered into with
tiankong Interactive  Co., Ltd.

*******************************************************************************/

// 帮派

#include "StdAfx.h"
#include "../common/ServerDefine/guild_server_define.h"
#include "../common/ServerDefine/guild_server_define.h"
#include "../../common/WorldDefine/container_define.h"
#include "db_handler.h"

// 读取所有帮派资料
BOOL db_handler::load_all_guild(LPVOID p_buffer, int& n_guild_count)
{
	return common_select(p_buffer, &n_guild_count, INVALID_VALUE, INVALID_VALUE, 
				MakeDelegateOfDBHandler(&db_handler::makesql_load_all_guild),
				MakeDelegateOfDBHandler(&db_handler::procres_load_all_guild));
}

VOID db_handler::makesql_load_all_guild(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("guild", 
		"id,"			"name,"			"creater_name_id,"	"special_state,"		"level,"
		"hold_city0,"	"hold_city1,"	"hold_city2,"		"fund,"				"material,"
		"reputation,"	"daily_cost,"	"peace,"			"rank,"				"tenet,"
		"symbol,"		"group_purchase," "leader_id,"		"remain_spread_times,"	"commendation,"
		"formal,"		"sign_num,"		 "sign_data,"		"create_time,"		"enemy_data,"
		"league_id,"	"unleague_time," "prosperity,"		"uplevel_time,"		"pos_name,"      
		"pos_power,"	"dkp,"			 "change_dkp,"		"symbol_value,"		"script_data,"	
		"text,"			"family_name,"	 "npc_name,"		"daogao,"			"jujue_time");
}

VOID db_handler::procres_load_all_guild(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	//M_trans_pointer(p, p_buffer, s_guild_load);
	s_guild_load * p = ( s_guild_load * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		p->dwID				= (*p_result)[0].get_dword();
		p->dwFounderNameID	= (*p_result)[2].get_dword();
		p->dwSpecState		= (*p_result)[3].get_dword();
		p->byLevel			= (*p_result)[4].get_byte();
		p->byHoldCity[0]	= (*p_result)[5].get_byte();
		p->byHoldCity[1]	= (*p_result)[6].get_byte();
		p->byHoldCity[2]	= (*p_result)[7].get_byte();
		p->nFund			= (*p_result)[8].get_int();
		p->nMaterial		= (*p_result)[9].get_int();
		p->nReputation		= (*p_result)[10].get_int();
		p->nDailyCost		= (*p_result)[11].get_int();
		p->n16Peace			= (INT16)(*p_result)[12].get_int();
		p->n16Rank			= (INT16)(*p_result)[13].get_int();
		p->nApplyLevel		= (*p_result)[16].get_int();
		p->dwLeaderRoleID	= (*p_result)[17].get_dword();
		p->byAffairRemainTimes = (BYTE)(*p_result)[18].get_dword();
		p->bCommendation	= (*p_result)[19].get_bool();
		p->bFormal			= (*p_result)[20].get_bool();
		p->bySignatoryNum	= (*p_result)[21].get_byte();
		p->dwLeagueID		= (*p_result)[25].get_dword();
		p->nProsperity		= (*p_result)[27].get_int();
		p->bSignUpAttact		= (*p_result)[32].get_bool();
		p->dwValue			= (*p_result)[33].get_dword();


		(*p_result)[1].get_blob(p->sz_name, sizeof(p->sz_name));
		(*p_result)[14].get_blob(p->sz_tenet, sizeof(p->sz_tenet));
		(*p_result)[15].get_blob(p->sz_symbol, sizeof(p->sz_symbol));

		(*p_result)[22].get_blob(p->dwSignRoleID, sizeof(p->dwSignRoleID));
		DataTime2DwordTime(p->dwCreateTime,	(*p_result)[23].get_string(), (*p_result)[23].get_length());
		(*p_result)[24].get_blob(p->dwEnemyID, sizeof(p->dwEnemyID));
		DataTime2DwordTime(p->dwUnLeagueBeginTime, (*p_result)[26].get_string(), (*p_result)[26].get_length());
		DataTime2DwordTime(p->dwMianzhanTime, (*p_result)[28].get_string(), (*p_result)[28].get_length());
		DataTime2DwordTime(p->dwJujueTime, (*p_result)[39].get_string(), (*p_result)[39].get_length());
		(*p_result)[29].get_blob(p->szPosName, sizeof(p->szPosName));	
		(*p_result)[30].get_blob(p->dwPosPower, sizeof(p->dwPosPower));	
		(*p_result)[31].get_blob(p->n16DKP, sizeof(p->n16DKP));
		(*p_result)[34].get_blob(p->n32ScriptData, sizeof(p->n32ScriptData));
		(*p_result)[35].get_blob(p->szText, sizeof(p->szText));
		(*p_result)[36].get_blob(p->n_family_name, (*p_result)[36].get_length());
		(*p_result)[37].get_blob(p->n_name, (*p_result)[37].get_length());
		//(*p_result)[38].get_blob(p->bDaogao, sizeof(p->bDaogao));

		p_result->next_row();
		++p;
	}
}

// 读取种植信息
BOOL db_handler::load_all_guild_plant_data(LPVOID p_buffer, int& n_count)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_guild_plant),
		MakeDelegateOfDBHandler(&db_handler::procres_load_all_guild_plant));
}

VOID db_handler::makesql_load_all_guild_plant(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("guild_plant", 
		"id," "data");
}

VOID db_handler::procres_load_all_guild_plant(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	s_guild_plant * p = ( s_guild_plant * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		p->dw_guild_id				= (*p_result)[0].get_dword();
		(*p_result)[1].get_blob(p->s_data, (*p_result)[1].get_length());

		p_result->next_row();
		++p;
	}
}
// 新建帮派
BOOL db_handler::insert_guild(s_create_guild_info* p_create_info)
{
	return common_update(INVALID_VALUE, p_create_info, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_guild));
}

VOID db_handler::makesql_insert_guild(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, s_create_guild_info);
	s_create_guild_info * p = ( s_create_guild_info * ) p_buffer ;  

	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->insert_item("guild");
	p_stream->write_string("id=") << p->dw_guild_id;
	p_stream->write_string(",name='").write_blob(p->sz_name, (_tcsclen(p->sz_name) + 1) * sizeof(TCHAR), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",creater_name_id=") << p->dw_create_role_name_id;
	p_stream->write_string(",level=") << p->by_guild_level;
	p_stream->write_string(",fund=") << p->n_guild_fund;
	p_stream->write_string(",material=") << p->n_guild_material;
	p_stream->write_string(",reputation=") << p->n_guild_rep;
	p_stream->write_string(",peace=") << p->n16_guild_peace;
	
	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_create_time))
	{
		p_stream->write_string(",create_time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}
	else
	{
		print_message(_T("file:%s line:%d DwordTime2DataTime() error !!!!!\r\n"), _TFILE, _TLINE);
		ASSERT(0);
	}

	p_stream->write_string(",special_state=") << p->dw_spec_state;

	p_stream->write_string(",group_purchase=") << p->n_group_purchase;
	p_stream->write_string(",leader_id=") << p->dw_create_role_name_id;
	p_stream->write_string(",remain_spread_times=") << p->by_affair_remain_times;

	p_stream->write_string(",sign_data='").write_blob(p->dw_sign_role_id, sizeof(p->dw_sign_role_id), p_conn);
	p_stream->write_string("'");

	p_stream->write_string(",enemy_data='").write_blob(p->dw_enemy_id, sizeof(p->dw_enemy_id), p_conn);
	p_stream->write_string("'");

	p_stream->write_string(",pos_name='").write_blob(p->sz_pos_name, sizeof(p->sz_pos_name), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",pos_power='").write_blob(p->dw_pos_power, sizeof(p->dw_pos_power), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",family_name='").write_blob(p->n_family_name, sizeof(p->n_family_name), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",npc_name='").write_blob(p->n_name, sizeof(p->n_name), p_conn);
	p_stream->write_string("'");

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);
}

// 删除帮派
BOOL db_handler::delete_guild(DWORD dw_guild_id)
{
	return common_update(INVALID_VALUE, &dw_guild_id, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_guild));
}

VOID db_handler::makesql_delete_guild(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->delete_item("guild");
	p_stream->where_item();
	p_stream->write_string("id=") << *((DWORD*)p_buffer);

	game_db_interface_->return_use_connect(p_conn);
}

BOOL db_handler::resetSignUpAttack()
{
	return common_update(INVALID_VALUE, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_reset_sign_up_attack));
}

VOID db_handler::makesql_reset_sign_up_attack(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->update_item("guild");
	p_stream->write_string("change_dkp=0");

	game_db_interface_->return_use_connect(p_conn);
}
// 修改帮派信条
BOOL db_handler::update_guild_tenet(DWORD dw_guild_id, LPTSTR sz_tenet)
{
	return common_update(dw_guild_id, sz_tenet, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_tenet));
}

VOID db_handler::makesql_update_guild_tenet(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, TCHAR);
	TCHAR * p = ( TCHAR * ) p_buffer ;  

	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();
	
	p_stream->update_item("guild");
	p_stream->write_string("tenet='").write_blob(p, (_tcsclen(p) + 1) * sizeof(TCHAR), p_conn);
	p_stream->write_string("'");

	p_stream->where_item();
	p_stream->write_string("id=") << dw_guild_id;

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);
}
// 修改帮派图标
BOOL db_handler::update_guild_symbol(DWORD dw_guild_id, LPTSTR sz_tenet)
{
	return common_update(dw_guild_id, sz_tenet, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_symbol));
}

VOID db_handler::makesql_update_guild_symbol(odbc::sql_language_disposal* p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, NET_DB2C_change_guild_symbol);
	TCHAR * p = ( TCHAR * ) p_buffer ;  

	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->update_item("guild");
	p_stream->write_string("symbol='").write_blob(p, (_tcsclen(p) + 1) * sizeof(TCHAR), p_conn);
	p_stream->write_string("'");

	p_stream->where_item();
	p_stream->write_string("id=") << dw_guild_id;

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);

}

// 保存帮派所有属性
BOOL db_handler::update_guild_all(tagGuildBase *p_guild_base, int n_guild_num)
{
	return	common_update(INVALID_VALUE, p_guild_base, n_guild_num, sizeof(tagGuildBase), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_all));
}

VOID db_handler::makesql_update_guild_all(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, tagGuildBase);
	tagGuildBase * p = ( tagGuildBase * ) p_buffer ;  

	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->update_item("guild");

	p_stream->write_string("special_state=") << p->dwSpecState;
	p_stream->write_string(",level=") << p->byLevel;
	p_stream->write_string(",hold_city0=") << p->byHoldCity[0];
	p_stream->write_string(",hold_city1=") << p->byHoldCity[1];
	p_stream->write_string(",hold_city2=") << p->byHoldCity[2];
	p_stream->write_string(",fund=") << p->nFund;
	p_stream->write_string(",material=") << p->nMaterial;
	p_stream->write_string(",reputation=") << p->nReputation;
	p_stream->write_string(",daily_cost=") << p->nDailyCost;
	p_stream->write_string(",peace=") << p->n16Peace;
	p_stream->write_string(",rank=") << p->n16Rank;
	p_stream->write_string(",group_purchase=") << p->nApplyLevel;
	p_stream->write_string(",remain_spread_times=") << p->byAffairRemainTimes;
	p_stream->write_string(",leader_iD=") << p->dwLeaderRoleID;
	p_stream->write_string(",formal=") << p->bFormal;
	p_stream->write_string(",sign_num=") << p->bySignatoryNum;
	p_stream->write_string(",sign_data='").write_blob(p->dwSignRoleID, sizeof(p->dwSignRoleID), p_conn);
	p_stream->write_string("'");
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dwMianzhanTime);
	p_stream->write_string(",uplevel_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dwJujueTime);
	p_stream->write_string(",jujue_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	p_stream->write_string(",enemy_data='").write_blob(p->dwEnemyID, sizeof(p->dwEnemyID), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",league_id=") << p->dwLeagueID;
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dwUnLeagueBeginTime);
	p_stream->write_string(",unleague_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	p_stream->write_string(",prosperity=") << p->nProsperity;
	p_stream->write_string(",pos_name='").write_blob(p->szPosName, sizeof(p->szPosName), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",pos_power='").write_blob(p->dwPosPower, sizeof(p->dwPosPower), p_conn);	
	p_stream->write_string("'");
	p_stream->write_string(",dkp='").write_blob(p->n16DKP, sizeof(p->n16DKP), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",change_dkp=") << p->bSignUpAttact;
	/*DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dwChangeSymbolTime);
	p_stream->write_string(",change_guild_symbol='").write_string(sz_time_buffer_);
	p_stream->write_string("'");*/
	p_stream->write_string(",symbol_value=") << p->dwValue;
	p_stream->write_string(",script_data='").write_blob(p->n32ScriptData, sizeof(p->n32ScriptData), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",text='").write_blob(p->szText, sizeof(p->szText), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",family_name='").write_blob(p->n_family_name, sizeof(p->n_family_name), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",npc_name='").write_blob(p->n_name, sizeof(p->n_name), p_conn);
	p_stream->write_string("'");
	//p_stream->write_string(",daogao='").write_blob(p->bDaogao, sizeof(p->bDaogao), p_conn);
	//p_stream->write_string("'");

	p_stream->where_item();
	p_stream->write_string("id=") << p->dwID;

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);
}

BOOL db_handler::update_guild_all_plant(s_guild_plant *p_guild_plant, int n_guild_num)
{
	return	common_update(INVALID_VALUE, p_guild_plant, n_guild_num, sizeof(s_guild_plant), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_plant));
}

VOID db_handler::makesql_update_guild_plant(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	s_guild_plant * p = ( s_guild_plant * ) p_buffer ;  

	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->replace_item("guild_plant");
	
	p_stream->write_string("id=") << p->dw_guild_id;
	p_stream->write_string(",data='").write_blob(p->s_data, sizeof(p->s_data), p_conn);
	p_stream->write_string("'");
	

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);
}
// 保存帮派属性
BOOL db_handler::update_guild_attribute(tagGuildBase *p_guild_base)
{
	return	common_update(INVALID_VALUE, p_guild_base, 1, sizeof(tagGuildBase), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_all));
}
BOOL db_handler::update_guild_plant(s_guild_plant* p_guild_plant)
{
	return	common_update(INVALID_VALUE, p_guild_plant, 1, sizeof(s_guild_plant), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_plant));
}
// 修改帮派职位名称
BOOL db_handler::update_guild_pos_name(DWORD dw_guild_id, NET_DB2C_change_guild_pos_name *p_guild_pos_name)
{
	return common_update(dw_guild_id, p_guild_pos_name, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_pos_name));
}
VOID db_handler::makesql_update_guild_pos_name(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, NET_DB2C_change_guild_pos_name);
	NET_DB2C_change_guild_pos_name * p = ( NET_DB2C_change_guild_pos_name * ) p_buffer ;  

	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->update_item("guild");
	p_stream->write_string("pos_name='").write_blob(&p->sz_pos_name[0][0], sizeof(p->sz_pos_name), p_conn);
	p_stream->write_string("'");

	p_stream->where_item();
	p_stream->write_string("id=") << dw_guild_id;

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);
}
BOOL db_handler::load_sbk_guild(LPVOID p_buffer, int& n_count)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_sbk_guild),
		MakeDelegateOfDBHandler(&db_handler::procres_load_sbk_guild));
}

VOID db_handler::makesql_load_sbk_guild(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("server_info",	"guild_sbk");
	p_stream->where_item();
	p_stream->write_string("id=0");
}

VOID db_handler::procres_load_sbk_guild(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		DWORD* pGuildID = (DWORD*) p_buffer;
		*pGuildID = (*p_result)[0].get_dword();
		

		p_buffer = pGuildID + sizeof(DWORD);

		p_result->next_row();
	}
}

BOOL db_handler::save_guild_sbk(DWORD dw_guild_id)
{
	return	common_update(dw_guild_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_guild_sbk));
}

VOID db_handler::makesql_save_guild_sbk(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->replace_item("server_info");
	p_stream->write_string("guild_sbk=") << dw_guild_id;

	p_stream->write_string(",id=0");

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);
}

// 修改帮派职位权利
BOOL db_handler::update_guild_pos_power(DWORD dw_guild_id, NET_DB2C_change_guild_pos_power *p_guild_pos_power)
{
	return common_update(dw_guild_id, p_guild_pos_power, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_pos_power));
}
VOID db_handler::makesql_update_guild_pos_power(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, NET_DB2C_change_guild_pos_power);
	NET_DB2C_change_guild_pos_power * p = ( NET_DB2C_change_guild_pos_power * ) p_buffer ;  

	// 取得数据库连接
	odbc::tag_mysql_connect* p_con = game_db_interface_->get_idlesse_connect();

	p_stream->update_item("guild");
	p_stream->write_string("pos_power='").write_blob(&p->dw_pos_power[0], sizeof(p->dw_pos_power), p_con);
	p_stream->write_string("'");

	p_stream->where_item();
	p_stream->write_string("id=") << dw_guild_id;

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_con);
}
// 获取所有帮派成员资料
BOOL db_handler::load_all_guild_member(LPVOID p_buffer, int& n_guil_member)
{
	return common_select(p_buffer, &n_guil_member, INVALID_VALUE, INVALID_VALUE, 
				MakeDelegateOfDBHandler(&db_handler::makesql_load_all_guild_member),
				MakeDelegateOfDBHandler(&db_handler::procres_load_all_guild_member));
}

VOID db_handler::makesql_load_all_guild_member(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("guild_member", 
				"role_id,"	"guild_id,"  "guild_pos,"  "total_contribution," "cur_contribution,"
				"exploit,"	"salary,"  "can_use_guild_ware," "ballot,"	"dkp," "join_time," "total_fund");
}

VOID db_handler::procres_load_all_guild_member(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	//M_trans_pointer(p, p_buffer, s_guild_member_load);
	s_guild_member_load * p = ( s_guild_member_load * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		p->s_guild_member_.dw_role_id			= (*p_result)[0].get_dword();
		p->dw_guild_id_						= (*p_result)[1].get_dword();
		p->s_guild_member_.eGuildPos			= (EGuildMemberPos)(*p_result)[2].get_int();
		p->s_guild_member_.nTotalContribution	= (*p_result)[3].get_int();
		p->s_guild_member_.nContribution		= (*p_result)[4].get_int();
		p->s_guild_member_.nExploit			= (*p_result)[5].get_int();
		p->s_guild_member_.nSalary				= (*p_result)[6].get_int();
		p->s_guild_member_.bUseGuildWare		= (*p_result)[7].get_bool();
		p->s_guild_member_.bBallot				= (*p_result)[8].get_int();
		p->s_guild_member_.nDKP				= (*p_result)[9].get_int();	
		DataTime2DwordTime(p->s_guild_member_.dwJoinTime, (*p_result)[10].get_string(), (*p_result)[10].get_length());
		//p->s_guild_member_.bWar				= (*p_result)[11].get_int();	
		p->s_guild_member_.nTotalFund			= (*p_result)[11].get_int();
		p_result->next_row();
		++p;
	}
}

// 新加帮派成员
BOOL db_handler::insert_guild_member(const s_create_guild_member_info& create_member_info)
{
	return common_update(INVALID_VALUE, const_cast<s_create_guild_member_info*>(&create_member_info), 
					1, 0, MakeDelegateOfDBHandler(&db_handler::maeksql_insert_guild_member));
}

VOID db_handler::maeksql_insert_guild_member(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, s_create_guild_member_info);
	s_create_guild_member_info * p = ( s_create_guild_member_info * ) p_buffer ;  

	p_stream->insert_item("guild_member");
	p_stream->write_string("role_id=") << p->dw_role_id;
	p_stream->write_string(",guild_id=") << p->dw_guild_id;
	p_stream->write_string(",guild_pos=") << p->n8_guild_pos;
	p_stream->write_string(",ballot=1");
	p_stream->write_string(",join_time=now()");
}

// 帮派成员被踢或者自己离帮
BOOL db_handler::delete_guild_member(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_guild_member));
}

VOID db_handler::makesql_delete_guild_member(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->delete_item("guild_member");

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 更新帮派职位
BOOL db_handler::update_guild_member_pos(DWORD dw_role_id, INT8 n8_pos)
{
	return common_update(dw_role_id, &n8_pos, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_member_pos));
}

VOID db_handler::makesql_update_guild_member_pos(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("guild_member");
	p_stream->write_string("guild_pos=") << *(INT8*)p_buffer;

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}
// 设置帮派成员访问仓库权限
BOOL db_handler::guild_ware_privilege(DWORD dw_role_id, BOOL b_enable)
{
	return common_update(dw_role_id, &b_enable, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_guild_ware_privilege));
}

VOID db_handler::makesql_guild_ware_privilege(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("guild_member");
	p_stream->write_string("can_use_guild_ware=") << *(bool*)p_buffer;

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 更改帮派成员贡献度
BOOL db_handler::change_member_contribution(LPVOID p_buffer)
{
	return common_update(INVALID_VALUE, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_change_member_contribution));
}

VOID db_handler::makesql_change_member_contribution(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	//M_trans_pointer(p_msg, p_buffer, NET_DB2C_change_contrib);
	NET_DB2C_change_contrib * p_msg = ( NET_DB2C_change_contrib * ) p_buffer ;  

	p_stream->clear();
	p_stream->update_item("guild_member");
	p_stream->write_string("cur_contribution=") << p_msg->n_contribution;
	p_stream->write_string(",total_contribution=") << p_msg->n_total_contribution;
	p_stream->write_string(",total_fund=") << p_msg->n_total_fund;

	p_stream->where_item();
	p_stream->write_string("role_id=") << p_msg->dw_role_id;
}

// 设置帮派成员投票权限
BOOL db_handler::change_ballot(LPVOID p_buffer)
{
	return common_update(INVALID_VALUE, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_change_ballot));
}

VOID db_handler::makesql_change_ballot(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	//M_trans_pointer(p_message, p_buffer, NET_DB2C_change_ballot);
	NET_DB2C_change_ballot * p_message = ( NET_DB2C_change_ballot * ) p_buffer ;  

	p_stream->clear();
	p_stream->update_item("guild_member");
	p_stream->write_string("ballot=") << p_message->b_ballot;
	p_stream->where_item();
	p_stream->write_string("role_id=") << p_message->dw_role_id;
}

//BOOL db_handler::change_war(LPVOID p_buffer)
//{
//	return common_update(INVALID_VALUE, p_buffer, 1, 0, 
//		MakeDelegateOfDBHandler(&db_handler::makesql_change_war));
//}
//
//VOID db_handler::makesql_change_war(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
//{
//	NET_DB2C_change_can_war * p_message = ( NET_DB2C_change_can_war * ) p_buffer ;  
//
//	p_stream->clear();
//	p_stream->update_item("guild_member");
//	p_stream->write_string("canwar=") << p_message->b_war;
//	p_stream->where_item();
//	p_stream->write_string("role_id=") << p_message->dw_role_id;
//}

// 更新帮派成员DKP
BOOL db_handler::change_DKP(LPVOID p_buffer)
{
	return common_update(INVALID_VALUE, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_change_DKP));
}

VOID db_handler::makesql_change_DKP(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, NET_DB2C_change_dkp);
	NET_DB2C_change_dkp * p = ( NET_DB2C_change_dkp * ) p_buffer ;  

	p_stream->clear();
	p_stream->update_item("guild_member");
	p_stream->write_string("dkp=") << p->n_dkp;
	p_stream->where_item();
	p_stream->write_string("role_id=") << p->dw_role_id;
}

// 获取帮派仓库所有道具
BOOL db_handler::load_guild_ware_all_items(LPVOID &p_buffer, DWORD dw_guild_id, int& n_item)
{
	return common_select(p_buffer, &n_item, dw_guild_id, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_guild_ware_items),
		MakeDelegateOfDBHandler(&db_handler::procres_load_items));
}
VOID db_handler::makesql_load_guild_ware_items(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->write_string("select ");
	init_item_load_field(p_stream);
	p_stream->write_string(" from item left join equip using(serial)");

	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_guild_id;
	p_stream->write_string(" and container_type_id=") << EICT_GuildWare;
}

// 获取帮派所有设施
BOOL db_handler::load_guild_facilities(LPVOID p_buffer, int& n_info, DWORD dw_guild_id)
{
	return common_select(p_buffer, &n_info, dw_guild_id, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_guild_facilities), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_guild_facilities));
}

VOID db_handler::makesql_load_guild_facilities(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("guild_upgrade",	"guild_id,"		"type,"	"level,"	"progress,"
										"item_type_1,"	"item_neednum_1,"	
										"item_type_2,"	"item_neednum_2,"
										"item_type_3,"	"item_neednum_3,"
										"item_type_4,"	"item_neednum_4,"
										"max_level,"		"b_up_level,"			"up_begin_time,"
										"b_dead,"		"dead_begin_time,"	"relive_time,"
										"use_type,"		"use_num,"		"use_time,"
										"holiness_step");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;
}

VOID db_handler::procres_load_guild_facilities(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		//M_trans_pointer(pInfo, p_buffer, s_facilities_load);
		s_facilities_load * pInfo = ( s_facilities_load * ) p_buffer ;  

		pInfo->dw_guild_id	= (*p_result)[0].get_dword();
		pInfo->e_type		= (EFacilitiesType)(*p_result)[1].get_int();
		pInfo->by_level		= (*p_result)[2].get_byte();
		pInfo->n16_progress	= (*p_result)[3].get_short();
		pInfo->by_max_level	= (*p_result)[12].get_byte();
		pInfo->b_up_level		= (*p_result)[13].get_byte();
		DataTime2DwordTime(pInfo->dw_begin_up_time, (*p_result)[14].get_string(), (*p_result)[14].get_length());
		pInfo->b_dead		= (*p_result)[15].get_byte();
		DataTime2DwordTime(pInfo->dw_dead_begin_time, (*p_result)[16].get_string(), (*p_result)[16].get_length());
		pInfo->dw_relive_time_limit = (*p_result)[17].get_dword();
		pInfo->byUseType = (*p_result)[18].get_int();
		pInfo->byUseNum = (*p_result)[19].get_int();
		DataTime2DwordTime(pInfo->dwUseTime, (*p_result)[20].get_string(), (*p_result)[20].get_length());
		pInfo->byStep = (*p_result)[21].get_byte();

		for (int n=0; n<MAX_UPGRADE_NEED_ITEM_TYPE; n++)
		{
			pInfo->dw_item_type_id[n]	= (*p_result)[n*2+4].get_dword();
			pInfo->n_item_need[n]		= (*p_result)[n*2+5].get_int();
		}

		p_buffer = &pInfo[1];

		p_result->next_row();
	}
}

// 新增帮派设施
BOOL db_handler::insert_guild_facilities(s_facilities_load* p_info)
{
	return common_update(INVALID_VALUE, p_info, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_guild_facilities));
}

VOID db_handler::makesql_insert_guild_facilities(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	//M_trans_pointer(p_info, p_buffer, s_facilities_load);
	s_facilities_load * p_info = ( s_facilities_load * ) p_buffer ;  

	p_stream->clear();
	p_stream->insert_item("guild_upgrade");
	p_stream->write_string("guild_id=") << p_info->dw_guild_id;	
	p_stream->write_string(",type=") << p_info->e_type;
	p_stream->write_string(",level=") << p_info->by_level;
	p_stream->write_string(",max_level=") << p_info->by_max_level;
	p_stream->write_string(",progress=") << p_info->n16_progress;
	p_stream->write_string(",use_type=") << p_info->byUseType;
	p_stream->write_string(",use_num=") << p_info->byUseNum;
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p_info->dwUseTime);
	p_stream->write_string(",use_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	p_stream->write_string(",holiness_step=") << p_info->byStep;
	for (int n=1; n<=MAX_UPGRADE_NEED_ITEM_TYPE; n++)
	{
		p_stream->write_string(",item_type_") << n;
		p_stream->write_string("=") << p_info->dw_item_type_id[n-1];
		p_stream->write_string(",item_neednum_") << n;
		p_stream->write_string("=") << p_info->n_item_need[n-1];
	}
}

// 更新帮派设施
BOOL db_handler::update_guild_facilities(s_facilities_load* p_info)
{
	return common_update(INVALID_VALUE, p_info, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_facilities));
}

VOID db_handler::makesql_update_guild_facilities(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	//M_trans_pointer(p_info, p_buffer, s_facilities_load);
	s_facilities_load * p_info = ( s_facilities_load * ) p_buffer ;  

	p_stream->clear();
	p_stream->update_item("guild_upgrade");
	p_stream->write_string("level=") << p_info->by_level;
	p_stream->write_string(",max_level=") << p_info->by_max_level;
	p_stream->write_string(",progress=") << p_info->n16_progress;
	for (int n=1; n<=MAX_UPGRADE_NEED_ITEM_TYPE; n++)
	{
		p_stream->write_string(",item_type_") << n;
		p_stream->write_string("=") << p_info->dw_item_type_id[n-1];
		p_stream->write_string(",item_neednum_") << n;
		p_stream->write_string("=") << p_info->n_item_need[n-1];
	}

	p_stream->write_string(",b_up_level=") << p_info->b_up_level;
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p_info->dw_begin_up_time);
	p_stream->write_string(",up_begin_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	p_stream->write_string(",b_dead=") << p_info->b_dead;
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p_info->dw_dead_begin_time);
	p_stream->write_string(",dead_begin_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	p_stream->write_string(",relive_time=") << p_info->dw_relive_time_limit;
	p_stream->write_string(",use_type=") << p_info->byUseType;
	p_stream->write_string(",use_num=") << p_info->byUseNum;
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p_info->dwUseTime);
	p_stream->write_string(",use_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	p_stream->write_string(",holiness_step=") << p_info->byStep;

	p_stream->where_item();
	p_stream->write_string("guild_id=") << p_info->dw_guild_id;
	p_stream->write_string(" and type=") << p_info->e_type;
}

// 删除帮派设施
BOOL db_handler::delete_guild_facilities(DWORD dw_guild_id)
{
	return common_update(dw_guild_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_guild_facilities));
}

VOID db_handler::makesql_delete_guild_facilities(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	p_stream->clear();
	p_stream->delete_item("guild_upgrade");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;
}

// 新建帮派PVP数据
BOOL db_handler::insert_guild_pvp_data(int n_act_id, NET_DB2C_insert_pvp_data* p_pvp)
{
	return common_update(n_act_id, p_pvp, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_guild_pvp_data));
}

VOID db_handler::makesql_insert_guild_pvp_data(odbc::sql_language_disposal* p_stream, DWORD dwActID, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, NET_DB2C_insert_pvp_data);
	NET_DB2C_insert_pvp_data * p = ( NET_DB2C_insert_pvp_data * ) p_buffer ;  

	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->insert_item("guild_pvp");
	p_stream->write_string("act_id=") << dwActID;
	p_stream->write_string(",pvp_data='").write_blob(p->dw_guild_id, sizeof(p->dw_guild_id), p_conn);
	p_stream->write_string("'");

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);
}
// 取得帮派PVP数据
BOOL db_handler::load_guild_pvp_data(LPVOID p_buffer, int& nDataNum)
{
	return common_select(p_buffer, &nDataNum, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_guild_pvp_data), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_guild_pvp_data));
}

VOID db_handler::makesql_load_guild_pvp_data(odbc::sql_language_disposal* p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("guild_pvp", "act_id," "pvp_data");
}

VOID db_handler::procres_load_guild_pvp_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	//M_trans_pointer(p, p_buffer, s_load_guild_pvp_data);
	s_load_guild_pvp_data * p = ( s_load_guild_pvp_data * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	for(int i = 0; i < *p_number; i++)
	{
		p->n_act_id = (*p_result)[0].get_int();
		(*p_result)[1].get_blob(p->dw_guild_id, sizeof(p->dw_guild_id));

		p++;
		p_result->next_row();
	}
}
// 更新帮派PVP数据
BOOL db_handler::update_pvp_data(int n_act_id, NET_DB2C_up_pvp_data* p_pvp)
{
	return common_update(n_act_id, p_pvp, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_pvp_data));
}

VOID db_handler::makesql_update_pvp_data(odbc::sql_language_disposal* p_stream, DWORD dwActID, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, NET_DB2C_up_pvp_data);
	NET_DB2C_up_pvp_data * p = ( NET_DB2C_up_pvp_data * ) p_buffer ;

	// 取得数据库连接
	odbc::tag_mysql_connect* pCon = game_db_interface_->get_idlesse_connect();

	p_stream->update_item("guild_pvp");
	p_stream->write_string("pvp_data='").write_blob(p->dw_guild_id, sizeof(p->dw_guild_id), pCon);
	p_stream->write_string("'");
	p_stream->where_item();
	p_stream->write_string("act_id=") << dwActID;

	// 归还数据库连接
	game_db_interface_->return_use_connect(pCon);
}

// 读取帮会boss击杀
BOOL db_handler::load_guild_boss_kill(LPVOID p_buffer)
{
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_guild_boss_kill), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_guild_boss_kill));
}

VOID db_handler::makesql_load_guild_boss_kill(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->clear();
	p_stream->select_item("guild_kill_boss", "monster_id,"	"guild_id");
}

VOID db_handler::procres_load_guild_boss_kill(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	BYTE* p = new BYTE[1024];
	ZeroMemory(p, sizeof(*p));

	NET_DB2C_load_guild_skill_boss* p_send = (NET_DB2C_load_guild_skill_boss*)p;
 	M_msg_init(p_send, NET_DB2C_load_guild_skill_boss);

	INT n_num = p_result->get_row_count();
	p_send->n_num = n_num;

	s_guild_kill_boss* p_data = p_send->st_guild_kill_boss;

	while(n_num)
	{
		p_data->dw_monster_id = (*p_result)[0].get_dword();
		p_data->dw_guild_id = (*p_result)[1].get_dword();
		p_data++;

		n_num--;
		p_result->next_row();
	}

	p_send->dw_size = sizeof(NET_DB2C_load_guild_skill_boss) + (p_send->n_num-1)*sizeof(s_guild_kill_boss);
	server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

	SAFE_DELETE_ARRAY(p);

}

BOOL db_handler::load_guild_war_history(LPVOID p_buffer)
{
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_guild_war_history), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_guild_war_history));
}

VOID db_handler::makesql_load_guild_war_history(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->clear();
	p_stream->select_item("guild_war_history", "guild_id,"	"enemy_id,"	"time," "type," "enemy_leader_id");
}

VOID db_handler::procres_load_guild_war_history(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
 
	INT n_num = p_result->get_row_count();
	
	INT nMsgSize = n_num * sizeof(tagGuildWarHistory) + sizeof(INT) + 64;
	BYTE* p = new BYTE[nMsgSize];
	ZeroMemory(p, sizeof(*p));

	NET_DB2C_load_guild_war_history* p_send = (NET_DB2C_load_guild_war_history*)p;
	M_msg_init(p_send, NET_DB2C_load_guild_war_history);

	
	p_send->n_num = n_num;

	tagGuildWarHistory* p_data = p_send->st_guild_war_history;

	while(n_num)
	{
		p_data->dw_guild_id = (*p_result)[0].get_dword();
		p_data->dw_enemy_id = (*p_result)[1].get_dword();
		DataTime2DwordTime(p_data->dw_time, (*p_result)[2].get_string(), (*p_result)[2].get_length());
		p_data->e_war_history_type = (EWarHistoryTyep)(*p_result)[3].get_dword();
		p_data->dw_enemy_leader_id = (*p_result)[4].get_dword();
		p_data++;
		n_num--;

		p_result->next_row();
	}

	p_send->dw_size = sizeof(NET_DB2C_load_guild_war_history) + (p_send->n_num-1)*sizeof(NET_DB2C_load_guild_war_history);
	server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

	SAFE_DELETE_ARRAY(p);

}


BOOL db_handler::insert_guild_kill_boss(s_guild_kill_boss* p_data)
{
	return common_update(INVALID_VALUE, p_data, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_guild_boss_kill));
}


VOID db_handler::makesql_insert_guild_boss_kill(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	s_guild_kill_boss* p = (s_guild_kill_boss*)p_buffer;

	p_stream->clear();

	p_stream->insert_item("guild_kill_boss");

	p_stream->write_string("monster_id=") << p->dw_monster_id;
	p_stream->write_string(",guild_id=") << p->dw_guild_id;
}

BOOL db_handler::insert_guild_war_history(tagGuildWarHistory* p_data)
{
	return common_update(INVALID_VALUE, p_data, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_guild_war_history));
}

VOID db_handler::makesql_insert_guild_war_history(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	tagGuildWarHistory* p = (tagGuildWarHistory*)p_buffer;

	p_stream->clear();

	p_stream->insert_item("guild_war_history");

	p_stream->write_string("guild_id=") << p->dw_guild_id;
	p_stream->write_string(",enemy_id=") << p->dw_enemy_id;
	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_),p->dw_time))
	{
		p_stream->write_string(",time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}
	p_stream->write_string(",type=") << p->e_war_history_type;
	p_stream->write_string(",enemy_leader_id=") << p->dw_enemy_leader_id;
}

BOOL db_handler::delete_guild_war_history(tagGuildWarHistory* p_data)
{
	return common_update(INVALID_VALUE, p_data, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_guild_war_history));
}

VOID db_handler::makesql_delete_guild_war_history(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	tagGuildWarHistory* p = (tagGuildWarHistory*)p_buffer;

	p_stream->clear();

	p_stream->delete_item("guild_war_history");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << p->dw_guild_id;
	p_stream->write_string(" and enemy_id=") << p->dw_enemy_id;
	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_),p->dw_time))
	{
		p_stream->write_string(" and time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}

}
// 读取帮会招募榜信息
BOOL db_handler::load_guild_recruit(LPVOID p_buffer)
{
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_guild_recruit), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_guild_recruit));
}

VOID db_handler::makesql_load_guild_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->clear();
	p_stream->select_item("guild_recruit", "role_id");
}

VOID db_handler::procres_load_guild_recruit(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dw_total_count = p_result->get_row_count();

	int n_max_count = dw_total_count;
	int n_cur_count = 0;

	while(n_max_count > 0)
	{
		if(n_max_count > MAX_GUILDMEM_NUM_PER_MSG)
		{
			n_cur_count = MAX_GUILDMEM_NUM_PER_MSG;
		}
		else
		{
			n_cur_count = n_max_count;
		}
		memset(p_buffer, 0, sizeof(p_buffer));
		NET_DB2C_load_guild_recruit* p_send = (NET_DB2C_load_guild_recruit*)p_buffer;
		M_msg_init(p_send, NET_DB2C_load_guild_recruit);

		DWORD *p = p_send->p_data;


		for(int i = 0; i < n_cur_count; i++)
		{
			*p = (*p_result)[0].get_dword();
			p_result->next_row();
			p++;
		}

		p_send->n_num = n_cur_count;
		p_send->dw_size = sizeof(NET_DB2C_load_guild_recruit) + (n_cur_count-1)*sizeof(DWORD);

		server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

		n_max_count -= n_cur_count;
	}
}

// 添加帮会招募信息
BOOL db_handler::insert_guild_recruit(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_guild_recruit));
}

VOID db_handler::makesql_insert_guild_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->clear();
	p_stream->insert_item("guild_recruit");
	p_stream->write_string("role_id=") << dw_role_id;
}

// 删除帮会招募信息
BOOL db_handler::delete_guild_recruit(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_guild_recruit));
}

VOID db_handler::makesql_delete_guild_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->clear();
	p_stream->delete_item("guild_recruit");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 清除帮会招募榜
BOOL db_handler::clean_guild_recruit()
{
	return common_update(INVALID_VALUE, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clean_guild_recruit));
}

VOID db_handler::makesql_clean_guild_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->clear();
	p_stream->delete_item("guild_recruit");
}

// 获取帮派所有技能
BOOL db_handler::load_guild_skill_info(LPVOID p_buffer, DWORD dw_guild_id, int& n_count)
{
	return common_select(p_buffer, &n_count, dw_guild_id, NULL, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_guild_skill_info), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_guild_skill_info));
}

VOID db_handler::makesql_load_guild_skill_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("guild_skill", "skill_id," "progress," "level," "researching");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;
}

VOID db_handler::procres_load_guild_skill_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	*p_number = p_result->get_row_count();
	for(int i=0; i<*p_number; ++i)
	{
		//M_trans_pointer(pInfo, p_buffer, s_guild_skill_info);
		s_guild_skill_info * pInfo = ( s_guild_skill_info * ) p_buffer ;  

		pInfo->dw_skill_id	= (*p_result)[0].get_dword();
		pInfo->n16_progress	= (INT16)((*p_result)[1].get_int());
		pInfo->n_level		= (*p_result)[2].get_int();
		pInfo->b_researching	= (*p_result)[3].get_bool();

		p_buffer = &pInfo[1];

		p_result->next_row();
	}
}
// 新增帮派技能
BOOL db_handler::insert_guild_skill_info(DWORD dw_guild_id, s_guild_skill_info* pSkillInfo)
{
	return common_update(dw_guild_id, (LPVOID)pSkillInfo, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_guild_skill_info));
}

VOID db_handler::makesql_insert_guild_skill_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	//M_trans_pointer(p_skill_info, p_buffer, s_guild_skill_info);
	s_guild_skill_info * p_skill_info = ( s_guild_skill_info * ) p_buffer ;  

	p_stream->clear();

	p_stream->insert_item("guild_skill");
	p_stream->write_string("guild_id=") << dw_guild_id;
	p_stream->write_string(",skill_id=") << p_skill_info->dw_skill_id;
	p_stream->write_string(",progress=") << p_skill_info->n16_progress;
	p_stream->write_string(",level=") << p_skill_info->n_level;
	p_stream->write_string(",researching=") << p_skill_info->b_researching;
}

// 更新帮派技能
BOOL db_handler::update_guild_skill_info(DWORD dw_guild_id, s_guild_skill_info* pSkillInfo)
{
	return common_update(dw_guild_id, (LPVOID)pSkillInfo, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_skill_info));
}

VOID db_handler::makesql_update_guild_skill_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	//M_trans_pointer(p_skill_info, p_buffer, s_guild_skill_info);
	s_guild_skill_info * p_skill_info = ( s_guild_skill_info * ) p_buffer ;  

	p_stream->clear();
	
	p_stream->update_item("guild_skill");
	p_stream->write_string("progress=") << p_skill_info->n16_progress;
	p_stream->write_string(",level=") << p_skill_info->n_level;
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;
	p_stream->write_string(" and skill_id=") << p_skill_info->dw_skill_id;
}

// 更改帮派研究技能
BOOL db_handler::change_research_guild_skill(DWORD dw_guild_id, DWORD dw_skill_id)
{
	BOOL b_ret = common_update(dw_guild_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_research_guild_skill));

	if (b_ret && VALID_VALUE(dw_skill_id))
	{
		b_ret = common_update(dw_guild_id, (LPVOID)&dw_skill_id, 1, 0, 
			MakeDelegateOfDBHandler(&db_handler::makesql_change_research_guild_skill));
	}

	return b_ret;
}

VOID db_handler::makesql_change_research_guild_skill(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID pSkillID)
{
	p_stream->clear();

	p_stream->update_item("guild_skill");
	p_stream->write_string("researching=") << TRUE;
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;
	p_stream->write_string(" and skill_id=") << *(DWORD*)pSkillID;
}

VOID db_handler::makesql_clear_research_guild_skill(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID pSkillID)
{
	p_stream->clear();

	p_stream->update_item("guild_skill");
	p_stream->write_string("researching=") << FALSE;
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;
}

// 删除帮派技能
BOOL db_handler::delete_guild_skill_info(DWORD dw_guild_id)
{
	return common_update(dw_guild_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_guild_skill_info));
}

VOID db_handler::makesql_delete_guild_skill_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID pSkillID)
{
	p_stream->clear();

	p_stream->delete_item("guild_skill");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;
}

// 获取帮派弹劾
BOOL db_handler::load_guild_delate(LPVOID p_buffer, DWORD dw_guild_id)
{
	return common_select(p_buffer, NULL, dw_guild_id, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_guild_delate), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_guild_delate));
}

VOID db_handler::makesql_load_guild_delate(odbc::sql_language_disposal * p_stream, DWORD dw_guild_id, DWORD dw_role_id)
{
	p_stream->clear();

	p_stream->select_item("guild_delate", 
		"guild_id,"		"initiate_role_id,"		"be_delate_role_id,"		"agree_num,"
		"oppose_num,"	"delate_begin_time,"	"delate_end_time,"		"b_succeed,"
		"content");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;
}

VOID db_handler::procres_load_guild_delate(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	//ASSERT(p_number != NULL);

	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	int n_count = p_result->get_row_count();
	for(int i=0; i<n_count; ++i)
	{
		//M_trans_pointer(pInfo, p_buffer, s_guild_delate_load);
		s_guild_delate_load * pInfo = ( s_guild_delate_load * ) p_buffer ;  

		pInfo->dwGuildID = (*p_result)[0].get_dword();
		pInfo->dwInitiateRoleID = (*p_result)[1].get_dword();
		pInfo->dwBeDelateRoleID = (*p_result)[2].get_dword();
		pInfo->nAgreeNum		= (*p_result)[3].get_int();
		pInfo->nOpposeNum		= (*p_result)[4].get_int();
		DataTime2DwordTime(pInfo->dwDelateBeginTime, (*p_result)[5].get_string(), (*p_result)[5].get_length());
		DataTime2DwordTime(pInfo->dwDelateEneTime, (*p_result)[6].get_string(), (*p_result)[6].get_length());
		pInfo->bSucceed			= (*p_result)[7].get_int();

		(*p_result)[8].get_blob(pInfo->sz_content, (*p_result)[8].get_length());

		p_result->next_row();
	}

	game_db_interface_->return_use_connect(p_conn);
}

// 新增帮派弹劾
BOOL db_handler::insert_guild_delate(DWORD dw_guild_id)
{
	return common_update(dw_guild_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_guild_delate));
}

VOID db_handler::makesql_insert_guild_delate(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	p_stream->clear();

	p_stream->insert_item("guild_delate");
	p_stream->write_string("guild_id=") << dw_guild_id;
}

// 删除帮派弹劾
BOOL db_handler::delete_guild_delate(DWORD dw_guild_id)
{
	return common_update(dw_guild_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_guild_delate));
}

VOID db_handler::makesql_delete_guild_delate(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	p_stream->clear();

	p_stream->delete_item("guild_delate");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;
}

// 更新帮派弹劾
BOOL db_handler::update_guild_delate(LPVOID p_buffer, DWORD dw_guild_id)
{
	return common_update(dw_guild_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_delate));
}

VOID db_handler::makesql_update_guild_delate(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, tagGuildDelateBase);
	tagGuildDelateBase * p = ( tagGuildDelateBase * ) p_buffer ;  

	p_stream->clear();

	p_stream->update_item("guild_delate");
	p_stream->write_string("initiate_role_id =") << p->dwInitiateRoleID;
	p_stream->write_string(",be_delate_role_id=") << p->dwBeDelateRoleID;
	p_stream->write_string(",agree_num=") << p->nAgreeNum;
	p_stream->write_string(",oppose_num") << p->nOpposeNum;
	p_stream->write_string(",b_succeed") << p->bSucceed;
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dwDelateBeginTime);
	p_stream->write_string(",delate_begin_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dwDelateEneTime);
	p_stream->write_string(",delate_end_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << p->dwGuildID;
}

// 更新帮派弹劾内容
BOOL db_handler::update_guild_delate_content(DWORD dw_guild_id, LPTSTR szNewTenet)
{
	return common_update(dw_guild_id, szNewTenet, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guild_delate_content));
}

VOID db_handler::makesql_update_guild_delate_content(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, TCHAR);
	TCHAR * p = ( TCHAR * ) p_buffer ;  

	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->update_item("guild_delate");
	p_stream->write_string("content='").write_blob(p, (_tcsclen(p) + 1) * sizeof(TCHAR), p_conn);
	p_stream->write_string("'");

	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);
}

// 获取帮派商会资料
BOOL db_handler::load_guild_commerce_info(LPVOID p_buffer, DWORD dw_guild_id, int& n_count)
{
	return common_select(p_buffer, &n_count, dw_guild_id, NULL, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_guild_commerce_info),
		MakeDelegateOfDBHandler(&db_handler::procres_load_guild_commerce_info));
}

VOID db_handler::makesql_load_guild_commerce_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("guild_commodity", "role_id," "guild_id," "role_level," "tael," "goods");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;
}

VOID db_handler::procres_load_guild_commerce_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		//M_trans_pointer(pInfo, p_buffer, s_guild_commerce_info);
		s_guild_commerce_info * pInfo = ( s_guild_commerce_info * ) p_buffer ;  
		pInfo->dw_role_id		= (*p_result)[0].get_dword();
		pInfo->n_level		= (*p_result)[2].get_int();
		pInfo->n_tael		= (int)(*p_result)[3].get_int();
		(*p_result)[4].get_blob(pInfo->s_good_info, (*p_result)[4].get_length());
		
		p_buffer = &pInfo[1];

		p_result->next_row();
	}
}

// 获取帮派商会排行榜
BOOL db_handler::load_commerce_rank_info(LPVOID p_buffer, DWORD dw_guild_id, int& n_count)
{
	return common_select(p_buffer, &n_count, dw_guild_id, NULL, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_commerce_rank_info), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_commerce_rank_info));
}

VOID db_handler::makesql_load_commerce_rank_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("commerce_rank", "role_id," "guild_id," "times," "tael");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;
}

VOID db_handler::procres_load_commerce_rank_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		//M_trans_pointer(pInfo, p_buffer, tagCommerceRank);
		tagCommerceRank * pInfo = ( tagCommerceRank * ) p_buffer ;  

		pInfo->dw_role_id		= (*p_result)[0].get_dword();
		pInfo->nTimes		= (*p_result)[2].get_int();
		pInfo->nTael		= (int)(*p_result)[3].get_int();

		p_buffer = &pInfo[1];

		p_result->next_row();
	}
}

// 新增商品信息
BOOL db_handler::insert_commodity_info(LPVOID p_buffer)
{
	return common_update(INVALID_VALUE, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_commodity_info));
}

VOID db_handler::makesql_insert_commodity_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	//M_trans_pointer(p_commodity_info, p_buffer, NET_DB2C_create_commodity);
	NET_DB2C_create_commodity * p_commodity_info = ( NET_DB2C_create_commodity * ) p_buffer ;  

	p_stream->clear();	
	p_stream->insert_item("guild_commodity");
	p_stream->write_string("role_id=") << p_commodity_info->dw_role_id;
	p_stream->write_string(",guild_id=") << p_commodity_info->dw_guild_id;
	p_stream->write_string(",role_level=") << p_commodity_info->n_level;
	p_stream->write_string(",tael=") << p_commodity_info->n_tael;
}

// 更新商品金钱
BOOL db_handler::update_commodity_money(DWORD dw_role_id, int n_money)
{
	return common_update(dw_role_id, (LPVOID)&n_money, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_commodity_money));
}

VOID db_handler::makesql_update_commodity_money(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID nTael)
{
	p_stream->clear();

	p_stream->update_item("guild_commodity");
	p_stream->write_string("tael=") << *(int*)nTael;
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 保存商品信息
BOOL db_handler::save_commodity_info(LPVOID p_buffer, DWORD dw_role_id)
{
	return common_update(dw_role_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_commodity_info));
}

VOID db_handler::makesql_save_commodity_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->clear();
	p_stream->update_item("guild_commodity");
	
	p_stream->write_string("goods='");
	p_stream->write_blob(p_buffer, sizeof(tagCommerceGoodInfo)*MAX_COMMODITY_CAPACITY, p_conn);
	p_stream->write_string("'");

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);
}

// 删除商品信息
BOOL db_handler::delete_commodity_info(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_commodity_info));
}

VOID db_handler::makesql_delete_commodity_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->delete_item("guild_commodity");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 设置帮派表扬
BOOL db_handler::set_guild_citation(DWORD dw_guild_id, BOOL b_citation)
{
	return common_update(dw_guild_id, (LPVOID)&b_citation, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_set_guild_citation));
}

VOID db_handler::makesql_set_guild_citation(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	p_stream->clear();
	p_stream->update_item("guild");
	p_stream->write_string("commendation=") << *(BOOL*)p_buffer;
	p_stream->where_item();
	p_stream->write_string("id=") << dw_guild_id;
}

// 更新商会排名
BOOL db_handler::update_commerce_rank_info(DWORD dw_guild_id, LPVOID p_buffer)
{
	return common_update(dw_guild_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_commerce_rank_info));
}

VOID db_handler::makesql_update_commerce_rank_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	//M_trans_pointer(p_rank_info, p_buffer, tagCommerceRank);
	tagCommerceRank * p_rank_info = ( tagCommerceRank * ) p_buffer ;  

	p_stream->clear();
	p_stream->replace_item("commerce_rank");
	p_stream->write_string("role_id=") << p_rank_info->dw_role_id;
	p_stream->write_string(",guild_id=") << dw_guild_id;
	p_stream->write_string(",times=") << p_rank_info->nTimes;
	p_stream->write_string(",tael=") << p_rank_info->nTael;
}

// 删除商会排名
BOOL db_handler::delete_commerce_rank_info(DWORD dw_guild_id, DWORD dw_role_id)
{
	return common_update(dw_guild_id, (LPVOID)&dw_role_id, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_commerce_rank_info));
}

VOID db_handler::makesql_delete_commerce_rank_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	p_stream->clear();
	p_stream->delete_item("commerce_rank");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;

	DWORD dw_role_id = *(DWORD*)p_buffer;
	if (VALID_VALUE(dw_role_id))
	{
		p_stream->write_string(" and role_id=") << dw_role_id;
	}
}
