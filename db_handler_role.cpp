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
#include "../common/ServerDefine/leftmsg_server_define.h"
#include "../common/ServerDefine/role_data_server_define.h"
#include "db_handler.h"

// 创建角色
BOOL db_handler::create_role(OUT DWORD &dw_new_role_id, DWORD dw_account_id, const s_create_role_info &role_info, DWORD &dw_role_name_crc)
{
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->insert_item("role_data");
	p_stream->write_string("account_id=") << dw_account_id;
	p_stream->write_string(",role_id=") << dw_max_role_id_ + 1;
	p_stream->write_string(",role_name='").write_string(role_info.sz_role_name, p_conn);

	_tcscpy_s(sz_role_name_buffer_, _countof(sz_role_name_buffer_), role_info.sz_role_name);
	_tcslwr(sz_role_name_buffer_);
	dw_role_name_crc = get_tool()->crc32(sz_role_name_buffer_);
	p_stream->write_string("',role_name_crc=") << dw_role_name_crc;

	p_stream->write_string(",sex=") << role_info.avatar.bySex;
	p_stream->write_string(",hair_model_id=") << role_info.avatar.wHairMdlID;
	p_stream->write_string(",hair_color_id=") << role_info.avatar.wHairTexID;
	p_stream->write_string(",face_model_id=") << role_info.avatar.wFaceMdlID;
	p_stream->write_string(",face_detail_id=") << role_info.avatar.wFaceDetailTexID;
	p_stream->write_string(",dress_model_id=") << role_info.avatar.wDressMdlID;
	p_stream->write_string(",visualizeid=") << role_info.avatar.byVisualize;

	p_stream->write_string(",map_id=") << role_info.dw_map_id_;
	p_stream->write_string(",x=") << role_info.fX;			
	p_stream->write_string(",y=") << role_info.fY;
	p_stream->write_string(",z=") << role_info.fZ;
	p_stream->write_string(",face_x=") << role_info.f_face_x_;	
	p_stream->write_string(",face_y=") << role_info.f_face_y_;
	p_stream->write_string(",face_z=") << role_info.f_face_z_;

	p_stream->write_string(",level=") << role_info.by_level_;
	p_stream->write_string(",class=") << role_info.e_class_type_;

	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), role_info.create_time_))
	{
		p_stream->write_string(",create_time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}
	else
	{
		print_message(_T("Error: Func DwordTime2DataTime() run error in db_handler::create_role()!!!!!\r\n"));
		ASSERT(0);
	}

	BOOL bRet = game_db_interface_->sql_execute(p_stream);

	game_db_interface_->return_use_connect(p_conn);
	game_db_interface_->return_io(p_stream);

	if(bRet)
	{
		++dw_max_role_id_;
		++n_role_count_;
		dw_new_role_id = dw_max_role_id_;
	}

	++dw_write_count_;

	return bRet;
}

// 激活删除角色
BOOL db_handler::active_deleted_role(DWORD dw_role_id)
{
	return TRUE;
}

// 备份角色
DWORD db_handler::backup_role(DWORD dw_account_id, DWORD dw_role_id)
{
	DWORD dw_error_code;

	// 备份roledata表中相关数据
	dw_error_code = do_backup_role(dw_account_id, dw_role_id);
	if(E_Success != dw_error_code)
	{
		return dw_error_code;
	}

	// 备份item表中相关数据
	dw_error_code = do_backup_item(dw_role_id);
	if(E_Success != dw_error_code)
	{
		return dw_error_code;
	}

	return dw_error_code;
}


// 备份角色
BOOL db_handler::do_backup_role(DWORD dw_account_id, DWORD dw_role_id)
{
	BOOL b_ret = TRUE;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	ASSERT(VALID_POINT(p_stream));

	// 拼合SQL语句数据库操作语句
	p_stream->clear();
	p_stream->write_string("insert into role_data_del select * from role_data where account_id=") << dw_account_id;
	p_stream->write_string(" and role_id=") << dw_role_id;

	// 执行SQL
	b_ret = game_db_interface_->sql_execute(p_stream);

	// 归还sql格式化IO
	game_db_interface_->return_io(p_stream);

	++dw_write_count_;

	return b_ret;
}

// 备份道具
BOOL db_handler::do_backup_item(DWORD dw_role_id)
{
	BOOL b_ret = TRUE;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	ASSERT(VALID_POINT(p_stream));

	// 拼合SQL语句数据库操作语句
	p_stream->clear();
	p_stream->write_string("insert into item_del select * from item where owner_id=") << dw_role_id;

	// 执行SQL
	b_ret = game_db_interface_->sql_execute(p_stream);

	// 归还sql格式化IO
	game_db_interface_->return_io(p_stream);

	++dw_write_count_;

	return b_ret;
}


// 删除角色
DWORD db_handler::delete_role(DWORD dw_account_id, DWORD dw_role_id)
{
	if( !do_delete_role(dw_account_id, dw_role_id))
		return e_database_delete_role_failed;

	delete_role_buffs(dw_role_id);
	delete_role_items(dw_role_id);

	return E_Success;
}

// 删除角色
BOOL db_handler::do_delete_role(DWORD dw_account_id, DWORD dw_role_id)
{
	BOOL b_ret = TRUE;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	ASSERT(VALID_POINT(p_stream));

	// 拼合SQL语句数据库操作语句
	p_stream->delete_item("role_data");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and account_id=") << dw_account_id;

	// 执行SQL
	b_ret = game_db_interface_->sql_execute(p_stream);

	// 归还sql格式化IO
	game_db_interface_->return_io(p_stream);

	++dw_write_count_;

	return b_ret;
}

// 删除buff
BOOL db_handler::delete_role_buffs(DWORD dw_role_id)
{
	BOOL b_ret = TRUE;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	ASSERT(VALID_POINT(p_stream));

	// 拼合SQL语句数据库操作语句
	p_stream->delete_item("buff");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;

	// 执行SQL
	b_ret = game_db_interface_->sql_execute(p_stream);

	// 归还sql格式化IO
	game_db_interface_->return_io(p_stream);

	++dw_write_count_;

	return b_ret;
}

// 删除物品
BOOL db_handler::delete_role_items(DWORD dw_role_id)
{
	BOOL b_ret = TRUE;

	// 获得sql语句格式化io
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	ASSERT(VALID_POINT(p_stream));

	// 拼合SQL语句数据库操作语句
	p_stream->delete_item("item");
	p_stream->where_item();
	p_stream->write_string("owner_id=") << dw_role_id;
	p_stream->write_string(" and (container_type_id!=") << EICT_RoleWare;
	p_stream->write_string(" or (container_type_id=") << EICT_RoleWare;
	p_stream->write_string(" and bind=") << EBS_Bind;
	p_stream->write_string("))");

	// 执行SQL
	b_ret = game_db_interface_->sql_execute(p_stream);

	// 归还sql格式化IO
	game_db_interface_->return_io(p_stream);

	++dw_write_count_;

	return b_ret;
}


// 记录删除角色
BOOL db_handler::register_deleted_role(DWORD dw_account_id, DWORD dw_role_id, LPCTSTR sz_role_name, LPCSTR sz_ip)
{
	BOOL b_ret = TRUE;

	// 获得sql语句格式化io和连接
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();
	ASSERT(VALID_POINT(p_stream));
	ASSERT(VALID_POINT(p_conn));

	// 记录删除操作相关信息
	p_stream->insert_item("role_del");
	p_stream->write_string("account_id=") << dw_account_id;
	p_stream->write_string(",role_name='").write_string(sz_role_name, p_conn);
	p_stream->write_string("',role_id=") << dw_role_id;
	p_stream->write_string(",ip='").write_string(sz_ip);
	p_stream->write_string("',delete_time=now()");

	// 执行SQL
	b_ret = game_db_interface_->sql_execute(p_stream);

	// 归还sql格式化IO和连接
	game_db_interface_->return_use_connect(p_conn);
	game_db_interface_->return_io(p_stream);

	++dw_write_count_;

	return b_ret;
}

// 更新道具CD时间
BOOL db_handler::replace_item_cd_time(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer)
{
	BOOL b_ret = TRUE;

	// 获得sql语句格式化io和连接
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	*pp_buffer = p_buffer;

	// 拼合SQL语句数据
	p_stream->replace_item("item_cd_time");
	p_stream->write_string("role_id=") << dw_role_id;
	if(n_count > 0)
	{
		p_stream->write_string(",cd_time='");
		p_stream->write_blob(p_buffer, sizeof(tagCDTime) * n_count, p_conn);
		p_stream->write_string("'");

		*pp_buffer = (BYTE*)p_buffer + sizeof(tagCDTime) * n_count;
	}

	// 执行SQL
	b_ret = game_db_interface_->sql_execute(p_stream);

	// 归还sql格式化IO和连接
	game_db_interface_->return_use_connect(p_conn);
	game_db_interface_->return_io(p_stream);

	++dw_write_count_;

	return b_ret;
}

// 读取不在线留言
BOOL db_handler::load_leave_msg( DWORD dw_role_id, PVOID p_leave_msg, DWORD &dw_size )
{
	int n_msg_number = 0;
	PVOID p_buffer = p_leave_msg;
	BOOL b_ret = common_select(p_buffer, &n_msg_number, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_leave_msg),
		MakeDelegateOfDBHandler(&db_handler::procres_load_leave_msg));
	if (b_ret)
	{
		dw_size = (DWORD)((BYTE*)p_buffer - (BYTE*)p_leave_msg);
	}
	return b_ret;
}
VOID db_handler::makesql_load_leave_msg( odbc::sql_language_disposal *p_stream, DWORD dwDumy, DWORD dw_role_id )
{
	p_stream->clear();
	p_stream->write_string("select msg_data from left_msg where role_id=") << dw_role_id;
	p_stream->write_string(" order by msg_id asc");
}
VOID db_handler::procres_load_leave_msg( OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result )
{
	if(!VALID_POINT(p_result) || p_result->get_row_count() <= 0)
		return;

	int n_row_count = p_result->get_row_count();

	int i;
	for (i=0; i<n_row_count; ++i)
	{
		uint uLen = (*p_result)[0].get_length();
		BOOL bRtv = (*p_result)[0].get_blob(p_buffer, uLen);
		if (!bRtv) continue;
		p_buffer = (BYTE*) p_buffer + uLen;
		p_result->next_row();
	}

	if (VALID_POINT(p_number)) *p_number = i;
}


// 获取留言最大索引
BOOL db_handler::load_leave_msg_indexes( DWORD* p_indexes, DWORD dw_role_id, int &n_count )
{
	LPVOID p_buffer = (LPVOID)p_indexes;
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_leave_msg_indexes), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_leave_msg_indexes));
}
VOID db_handler::makesql_load_leave_msg_indexes( odbc::sql_language_disposal *p_stream, DWORD dwDumy, DWORD dw_role_id )
{
	p_stream->write_string("select msg_id from left_msg where role_id=") << dw_role_id;
	p_stream->write_string(" order by msg_id asc");
}
VOID db_handler::procres_load_leave_msg_indexes( OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result )
{
	if (!VALID_POINT(p_result))
		return;

	DWORD* p_indexes = (DWORD*)p_buffer;

	int n_count = p_result->get_row_count();
	int i;
	for (i=0; i<n_count; ++i)
	{
		p_indexes[i] = (*p_result)[0].get_dword();
		p_result->next_row();
	}
	if (VALID_POINT(p_number))*p_number = i;
}

// 删除留言
BOOL db_handler::delete_leave_msg( DWORD dw_role_id, DWORD dwDateTime )
{
	return common_update(dw_role_id, (LPVOID)&dwDateTime, 1, sizeof(DWORD), 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_leave_msg));
}
VOID db_handler::makesql_delete_leave_msg( odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID pDumy )
{
	DWORD dw_datetime = *((DWORD*)pDumy);
	p_stream->clear();
	p_stream->write_string("delete from left_msg where role_id=") << dw_role_id;
	p_stream->write_string(" and msg_id=") << dw_datetime;
}
// 新增留言
BOOL db_handler::insert_leave_msg( DWORD dw_role_id, DWORD dwDateTime, PVOID pLeftMsg )
{
	return common_update(dw_role_id, pLeftMsg, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_leave_msg)); 
}
VOID db_handler::makesql_insert_leave_msg( odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer )
{
	M_trans_pointer(p_leave_msg, p_buffer, s_database_left_msg);
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->clear();
	p_stream->write_string("insert into left_msg set role_id=") << p_leave_msg->dw_role_id;
	p_stream->write_string(", msg_id=")<< p_leave_msg->dw_date_time;

	tag_net_message* p_cmd = (tag_net_message*)(p_leave_msg->by_data);
	p_stream->write_string(", msg_data='");
	p_stream->write_blob(p_cmd, p_cmd->dw_size, p_conn);
	p_stream->write_string("'");
	game_db_interface_->return_use_connect(p_conn);
}

// 清除留言
BOOL db_handler::clear_leave_msg( DWORD dw_role_id )
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_leave_msg));
}
VOID db_handler::makesql_clear_leave_msg( odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer )
{
	p_stream->clear();
	p_stream->write_string("delete from left_msg where role_id=")<<dw_role_id;
}

// 清除角色不在线经验
BOOL db_handler::clear_role_leave_exp(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_role_leave_exp));
}
VOID db_handler::makesql_clear_role_leave_exp(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();

	p_stream->update_item("role_data");
	p_stream->write_string("leave_exp=0");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 清除角色不在线义气值
BOOL db_handler::clear_role_leave_brotherhood(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_role_leave_brotherhood));
}
VOID db_handler::makesql_clear_role_leave_brotherhood(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();

	p_stream->update_item("role_data");
	p_stream->write_string("leave_brotherhood=0");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

//gx add 2013.7.3
BOOL db_handler::clear_role_spouse_id(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_role_spouse_id));
}
VOID db_handler::makesql_clear_role_spouse_id(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();

	p_stream->update_item("role_data");
	p_stream->write_string("spouse_id=4294967295");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}
BOOL db_handler::clear_role_vip_info(DWORD dw_role_id,LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_role_vip_info));
}
VOID db_handler::makesql_clear_role_vip_info(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();

	p_stream->update_item("role_data");
	p_stream->write_string("vip_level=0");
	p_stream->write_string(",vip_deadline='2000-00-00 00:00:00'");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}
// 清除角色挂机次数
BOOL db_handler::clear_role_hang_count(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_role_hang_count));
}
VOID db_handler::makesql_clear_role_hang_count(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();
	p_stream->update_item("role_data");
	p_stream->write_string("hang_num=0");
	p_stream->write_string(",gift_step=") << 0;
	p_stream->write_string(",gift_id=") << 1100001;
	p_stream->write_string(",gift_leaving_time=") << 60000;
	p_stream->write_string(",gift_get=") << 0;
	p_stream->write_string(",gift_group_id=") << 1;
}

// 清空角色签到数据
BOOL db_handler::clean_role_sign(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clean_role_sign));
}

VOID db_handler::makesql_clean_role_sign(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();
	p_stream->update_item("sign");
	p_stream->write_string("data = null, reward_data = null, mianqian_time = 0");
}

BOOL db_handler::clear_role_day_clear(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_role_day_clear));
}

VOID db_handler::makesql_clear_role_day_clear(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();
	p_stream->update_item("role_data");
	p_stream->write_string("day_clear='");
	BYTE	m_byDayClear[ROLE_DAY_CLEAR_NUM];
	ZeroMemory(m_byDayClear, sizeof(m_byDayClear));
	p_stream->write_blob(m_byDayClear, sizeof(m_byDayClear));
	p_stream->write_string("'");
}

BOOL db_handler::reset_role_at_0(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, p_msg, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_reset_role_at_0));
}
VOID db_handler::makesql_reset_role_at_0(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	NET_C2DB_reset_role_value_at_0 *pProtocol = (NET_C2DB_reset_role_value_at_0*)p_buffer;
	
	p_stream->clear();
	p_stream->update_item("role_data");
	p_stream->write_string("perday_hang_getexp_timems=") << pProtocol->nPerDayHangGetExpTimeMS;
	p_stream->write_string(",cooldownrevive_cd=") << pProtocol->nCoolDownReviveCD;
}
// 清空角色元宝兑换次数
BOOL db_handler::clear_role_yuanbao_exchange_num(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_role_exchange_num));
}

VOID db_handler::makesql_clear_role_exchange_num(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();
	p_stream->update_item("role_data");
	p_stream->write_string("yuanbao_exchange_num=0");
}

// 清空角色拍卖次数
BOOL db_handler::clear_role_paimai_count(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_role_paimai_count));
}

VOID db_handler::makesql_clear_role_paimai_count(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();
	p_stream->update_item("role_data");
	p_stream->write_string("paimailimit=0");
}

// 清空角色战功商店购买次数
BOOL db_handler::clean_role_exploits_num(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_role_exploits_num));
}

VOID db_handler::makesql_clear_role_exploits_num(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();
	p_stream->update_item("role_data");
	p_stream->write_string("exploitslimit=0");
}

// 清空角色活跃度数据
BOOL db_handler::clean_role_active_data(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_role_active_data));
}

VOID db_handler::makesql_clear_role_active_data(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();
	p_stream->update_item("role_data");
	p_stream->write_string("active_num = 0");
	p_stream->write_string(",active_data = null");
	p_stream->write_string(",active_receive = null");
	p_stream->write_string(",guild_active_num = 0");
	p_stream->write_string(",guild_active_data = null");
	p_stream->write_string(",guild_active_receive = null");

}

// 清空角色钱庄拍卖次数
BOOL db_handler::clear_role_bank_count(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_role_bank_count));
}

VOID db_handler::makesql_clear_role_bank_count(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();
	p_stream->update_item("role_data");
	p_stream->write_string("banklimit=0");
}

// 更新1v1每日积分
BOOL db_handler::update_day_1v1_score(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_update_day_1v1_score));
}

VOID db_handler::makesql_update_day_1v1_score(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();
	p_stream->update_item("role_data");
	p_stream->write_string("day_1v1_num=0");
	p_stream->write_string(",day_1v1_score = day_1v1_score + 30");
}

// 更新1v1周积分
BOOL db_handler::update_week_1v1_score(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_update_week_1v1_score));
}

VOID db_handler::makesql_update_week_1v1_score(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();

	p_stream->update_item("role_data");
	p_stream->write_string("week_1v1_score = cur_1v1_score");
}

BOOL db_handler::reset_role_1v1_score(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, NULL, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_reset_role_1v1_score));
}

VOID db_handler::makesql_reset_role_1v1_score(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	p_stream->clear();

	p_stream->update_item("role_data");
	p_stream->write_string("cur_1v1_score=50");
	p_stream->write_string(",day_1v1_score=80");
	p_stream->write_string(",day_1v1_num=0");
	p_stream->write_string(",score_1v1_award=0");
}

// 更新1v1领奖标志
BOOL db_handler::update_1v1_award(DWORD dw_role_id, LPVOID p_msg)
{
	return common_update(dw_role_id, p_msg, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_role_1v1_award));
}

VOID db_handler::makesql_update_role_1v1_award(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	NET_C2DB_update_1v1_award* p = (NET_C2DB_update_1v1_award*)p_buffer;

	p_stream->clear();

	p_stream->update_item("role_data");
	p_stream->write_string("score_1v1_award=") << p->n16_award;
	p_stream->where_item();
	p_stream->write_string("role_id=") << p->dw_role_id; 
}

// 更新角色属性
BOOL db_handler::update_role_attribute( DWORD dw_role_id, PVOID p_buffer )
{
	return common_update(dw_role_id, p_buffer, 1, sizeof(NET_DB2C_update_role_att), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_role_attribute));
}
VOID db_handler::makesql_update_role_attribute( odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer )
{
	M_trans_pointer(p_recv, p_buffer, NET_DB2C_update_role_att);

	p_stream->clear();
	p_stream->update_item("role_data");
	switch(p_recv->n_type_)
	{
	case ertsa_exp:
		p_stream->write_string("exp_cur_level");
		break;
	case ertsa_level:
		p_stream->write_string("level");
		break;
	case ertsa_brotherhood:
		p_stream->write_string("vitality");
		break;
	case ertsa_love:
		p_stream->write_string("rage");
		break;
	case ertsa_wuhuen:
		p_stream->write_string("endurance");
		break;
	default:
		ASSERT(0);
		break;
	}
	p_stream->write_string("=") << p_recv->n_value_;
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 更新角色属性点
BOOL db_handler::update_role_attribute_point( DWORD dw_role_id, LPVOID p_msg )
{
	return common_update(dw_role_id, p_msg, 1, sizeof(NET_DB2C_update_role_att_point), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_role_attribute_point));
}
VOID db_handler::makesql_update_role_attribute_point( odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer )
{
	M_trans_pointer(p_recv, p_buffer, NET_DB2C_update_role_att_point);

	p_stream->clear();
	p_stream->update_item("role_data");

	p_stream->write_string("att_avail=") << p_recv->n_att_point_left_;

	p_stream->write_string(",physique_added=") << p_recv->n_att_point_add_[0];
	p_stream->write_string(",strength_added=") << p_recv->n_att_point_add_[1];
	p_stream->write_string(",pneuma_added=") << p_recv->n_att_point_add_[2];
	p_stream->write_string(",innerforce_added=") << p_recv->n_att_point_add_[3];
	p_stream->write_string(",technique_added=") << p_recv->n_att_point_add_[4];
	p_stream->write_string(",agility_added=") << p_recv->n_att_point_add_[5];

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 更新角色天赋点
BOOL db_handler::update_role_talent_point( DWORD dw_role_id, LPVOID p_msg )
{
	return common_update(dw_role_id, p_msg, 1, sizeof(NET_DB2C_update_role_talent_point), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_role_talent_point));
}
VOID db_handler::makesql_update_role_talent_point( odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer )
{
	M_trans_pointer(p_recv, p_buffer, NET_DB2C_update_role_talent_point);
	p_stream->clear();
	p_stream->update_item("role_data");

	std::stringstream str;

	str << "talent_type" << p_recv->n_index_+1 << "=" << p_recv->s_talent_.eType;
	str << ",talent_val" << p_recv->n_index_+1 << "=" << p_recv->s_talent_.nPoint;
	p_stream->write_string(str.str().c_str());

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 角色改名
BOOL db_handler::change_role_name(DWORD dw_account_id, DWORD dw_role_id, LPCTSTR sz_new_role_name , DWORD dw_name_crc, DWORD dw_time)
{
	BOOL b_ret = TRUE;

	// 获得sql语句格式化io和连接
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	// 拼合SQL语句数据
	p_stream->update_item("role_data");
	p_stream->write_string("role_name='").write_string(sz_new_role_name, p_conn);
	p_stream->write_string("',role_name_crc=") << dw_name_crc;
	p_stream->write_string(",last_change_name_time=") << dw_time;
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and account_id=") << dw_account_id;

	// 执行SQL
	b_ret = game_db_interface_->sql_execute(p_stream);

	// 归还sql格式化IO和连接
	game_db_interface_->return_use_connect(p_conn);
	game_db_interface_->return_io(p_stream);

	++dw_write_count_;

	return b_ret;
}

// 恢复删除角色保护时间
BOOL db_handler::role_guard_cancel_time(DWORD dw_account_id, DWORD dw_role_id)
{
	BOOL b_ret = TRUE;

	// 获得sql语句格式化io和连接
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	// 拼合SQL语句数据
	p_stream->update_item("role_data");
	p_stream->write_string("delete_role_guard_time=") << (DWORD)INVALID_VALUE;
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and account_id=") << dw_account_id;

	// 执行SQL
	b_ret = game_db_interface_->sql_execute(p_stream);

	// 归还sql格式化IO和连接
	game_db_interface_->return_use_connect(p_conn);
	game_db_interface_->return_io(p_stream);

	++dw_write_count_;

	return b_ret;
}

// 设置删除角色保护时间
BOOL db_handler::role_guard_set_time(DWORD dw_account_id, DWORD dw_role_id, DWORD dw_time)
{
	BOOL b_ret = TRUE;

	// 获得sql语句格式化io和连接
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	// 拼合SQL语句数据
	p_stream->update_item("role_data");
	p_stream->write_string("delete_role_guard_time=") << dw_time;
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and account_id=") << dw_account_id;

	// 执行SQL
	b_ret = game_db_interface_->sql_execute(p_stream);

	// 归还sql格式化IO和连接
	game_db_interface_->return_use_connect(p_conn);
	game_db_interface_->return_io(p_stream);

	++dw_write_count_;

	return b_ret;
}

// 角色更改帮会
BOOL db_handler::change_role_guild(DWORD dw_role_id, DWORD dw_guild_id)
{
	return common_update(dw_role_id, &dw_guild_id, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_change_role_guild));
}
VOID db_handler::makesql_update_change_role_guild(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("role_data");
	p_stream->write_string("guild_id=") << *(DWORD*)p_buffer;

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 角色离开帮会时间
BOOL  db_handler::update_role_leave_guild_time(DWORD dw_role_id, LPVOID p_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_role_leave_guild_time));
}
VOID db_handler::makesql_update_role_leave_guild_time(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p_leave_time, p_buffer, tagDWORDTime);

	p_stream->update_item("role_data");

	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), *p_leave_time))
	{
		p_stream->write_string("leave_guild_time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	
}
//更新vip信息
BOOL db_handler::update_role_vip_info(DWORD dw_role_id,LPVOID p_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_role_vip_info));
}
VOID db_handler::makesql_update_role_vip_info(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer)
{
	NET_DB2C_change_role_VIP_Info* p = (NET_DB2C_change_role_VIP_Info*)p_buffer;
	if (!p)
		return;
	p_stream->update_item("role_data");
	p_stream->write_string("vip_level=") << p->n_VIP_Level;

	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_VIP_deadline))
	{
		p_stream->write_string(",vip_deadline='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}
	p_stream->where_item();
	p_stream->write_string("role_id=") << p->dw_role_id;

}
// 更新进入限制
BOOL db_handler::update_role_map_limit(DWORD dw_role_id, int n_type)
{
	return common_update(dw_role_id, &n_type, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_role_map_limit));
}
VOID db_handler::makesql_update_role_map_limit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("map_limit");
	p_stream->write_string("enter_num=0");
	p_stream->where_item();
	p_stream->write_string("type=") << *(int*)p_buffer; 
}

// 删除进入限制
BOOL db_handler::delete_role_map_limit(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_role_map_limit));
}
VOID db_handler::makesql_delete_role_map_limit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->delete_item("map_limit");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 新增进入限制
BOOL db_handler::insert_role_map_limit(DWORD dw_role_id, s_enter_map_limit* p_map_limit)
{
	return common_update(dw_role_id, p_map_limit, 1, sizeof(s_enter_map_limit), 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_map_limit));
}
VOID db_handler::makesql_insert_map_limit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	M_trans_pointer(p, p_reserved, s_enter_map_limit);

	p_stream->insert_item("map_limit");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",map_id=") << p->dw_map_id_;
	p_stream->write_string(",enter_num=") << p->dw_enter_num_;
	p_stream->write_string(",type=") << p->e_enter_limit_;
}

// 角色穿着模型ID
BOOL db_handler::change_role_dress_model_id(DWORD dw_role_id, WORD w_value)
{
	return common_update(dw_role_id, &w_value, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_change_role_dress_model_id));
}
VOID db_handler::makesql_change_role_dress_model_id(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("role_data");
	p_stream->write_string("dress_model_id=") << *(WORD*)p_buffer;

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 设置角色删除标志
BOOL db_handler::set_role_delete_flag(DWORD dw_role_id, DWORD dw_time)
{
	return common_update(dw_role_id, &dw_time, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_set_role_delete_flag));
}
VOID db_handler::makesql_set_role_delete_flag(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID pTime)
{
	p_stream->update_item("role_data");
	p_stream->write_string("remove_flag=") << 1;
	p_stream->write_string(",remove_time=") << *(DWORD*)pTime;

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 重置玩家元气值
BOOL db_handler::reset_all_role_vigour( DWORD dw_role_id, LPVOID p_buffer )
{
	return common_update(dw_role_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_reset_all_role_vigour));
}
VOID db_handler::makesql_reset_all_role_vigour(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("role_data");
	p_stream->write_string("vigour=level");
	p_stream->write_string(",perday_vigour_get_total = 0");
	p_stream->write_string(",today_online_tick=0");
}

// 重置幸运
// 重置噬魂
BOOL db_handler::clean_shihun(DWORD dw_role_id, LPVOID p_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clean_shihun));
}

VOID db_handler::makesql_clean_shihun(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("role_data");
	p_stream->write_string("shihun=0");
}

// 重置幸运和强化次数
BOOL db_handler::clean_all_role_luck( DWORD dw_role_id, LPVOID p_buffer )
{
	return common_update(dw_role_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_clean_all_role_luck));
}

VOID db_handler::makesql_clean_all_role_luck(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("role_data");
	p_stream->write_string("culture=0");

}

//增加环随机刷新次数
BOOL db_handler::inc_circle_refreshnumber_and_questnum( DWORD dw_role_id, LPVOID p_buffer )
{
	return common_update(dw_role_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_inc_circle_refreshnumber_and_questnum));
}

VOID db_handler::makesql_inc_circle_refreshnumber_and_questnum(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	NET_C2DB_UpdateCircleRefreshNumberAndQuestNum *pProtocol = (NET_C2DB_UpdateCircleRefreshNumberAndQuestNum*)p_buffer;
	p_stream->update_item("role_data");
	p_stream->write_string(" circle_quest_refresh = circle_quest_refresh + ") << (pProtocol->nDelta);
	p_stream->write_string(",circle_quest_perdaynumber = ") << (pProtocol->nPerDayNum);
	p_stream->write_string(",circle_quest_refresh_daymax = ") << (pProtocol->nRefreshMax);

}

BOOL db_handler::clear_destory_equip_count( DWORD dw_role_id, LPVOID p_buffer )
{
	return common_update(dw_role_id, p_buffer, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_clear_destory_equip_count));
}

VOID db_handler::makesql_clear_destory_equip_count(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("role_data");
	p_stream->write_string("destory_equip_count=0");
}

// 1v1积分竞技场
BOOL db_handler::update_role_1v1_score(DWORD dw_role_id, LPVOID p_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_role_1v1_score));
}

VOID db_handler::makesql_update_role_1v1_score(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	NET_DB2C_update_role_1v1_score* p = (NET_DB2C_update_role_1v1_score*)p_buffer;

	p_stream->clear();

	p_stream->update_item("role_data");
	p_stream->write_string("cur_1v1_score=") << p->nScroe;
	p_stream->write_string(",day_1v1_num=") << p->nJoinNum;
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 离线玩家1v1竞技场积分
BOOL db_handler::update_noline_1v1_score(DWORD dw_role_id, LPVOID p_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_noline_1v1_score));
}

VOID db_handler::makesql_update_noline_1v1_score(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	NET_DB2C_update_noline_role_1v1_score* p = (NET_DB2C_update_noline_role_1v1_score*)p_buffer;
	p_stream->clear();

	p_stream->update_item("role_data");
	p_stream->write_string("cur_1v1_score = cur_1v1_score + ") << p->nScore;
	p_stream->write_string(",day_1v1_num = day_1v1_num + 1");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}


// 保存坐骑
BOOL db_handler::update_mounts(DWORD dw_role_id, LPVOID p_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_mounts));
}

VOID db_handler::makesql_update_mounts(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	NET_DB2C_update_mounts* p = (NET_DB2C_update_mounts*)p_buffer;
	p_stream->clear();

	p_stream->update_item("mounts");
	p_stream->write_string("step=") << p->sMounts.nStep;
	p_stream->write_string(",grade=") << p->sMounts.nGrade;
	p_stream->write_string(",exp=") << p->sMounts.nExp;
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}