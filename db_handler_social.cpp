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

#include "../../common/WorldDefine/QuestDef.h"
#include "../../common/WorldDefine/SocialDef.h"
#include "../../common/WorldDefine/ItemDefine.h"
#include "../../common/WorldDefine/RoleDefine.h"
#include "../../common/WorldDefine/mall_define.h"

#include "../common/ServerDefine/role_data_server_define.h"
#include "../common/ServerDefine/quest_server_define.h"
#include "../common/ServerDefine/mall_server_define.h"
#include "../common/ServerDefine/vipstall_server_define.h"
#include "../common/ServerDefine/title_server_define.h"
#include "db_handler.h"

// 新加好友
BOOL db_handler::insert_friend(DWORD dw_role_id, LPVOID p_friend_save)
{
	return common_update(dw_role_id, p_friend_save, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_friend));
}
VOID db_handler::makesql_insert_friend(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friend_save)
{
	M_trans_pointer(p, p_friend_save, s_friend_save);

	p_stream->insert_item("friend");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",friend_id=") << p->dw_friend_id_;
	p_stream->write_string(",group_id=") << p->n_group_id_;
}

// 更新好友
BOOL db_handler::update_friend(DWORD dw_role_id, LPVOID p_friend_save)
{
	return common_update(dw_role_id, p_friend_save, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_friend));
}
VOID db_handler::makesql_update_friend(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friend_save)
{
	M_trans_pointer(p, p_friend_save, s_friend_save);

	p_stream->update_item("friend");
	p_stream->write_string("group_id=") << p->n_group_id_;

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and friend_id=") << p->dw_friend_id_;
}

// 删除好友
BOOL db_handler::delelte_friend(DWORD dw_role_id, DWORD dw_friend_id)
{
	return common_update(dw_role_id, &dw_friend_id, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesqll_delete_friend));
}
VOID db_handler::makesqll_delete_friend(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friend_id)
{
	p_stream->delete_item("friend");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and friend_id=") << (*(DWORD *)p_friend_id);
}

// 删除角色在好友表中所有记录
BOOL db_handler::delete_all_friend_relation(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_all_friend_relation));
}
VOID db_handler::makesql_delete_all_friend_relation(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	p_stream->delete_item("friend");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" or friend_id=") << dw_role_id;
}

// 新增好友度
BOOL db_handler::insert_friendship(DWORD dw_role_id, LPVOID p_friendship_save)
{
	return common_update(dw_role_id, p_friendship_save, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_friendship));
}
VOID db_handler::makesql_insert_friendship(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friendship_save)
{
	M_trans_pointer(p, p_friendship_save, s_friendship_save);
	p_stream->insert_item("friendship");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",friend_id=") << p->dw_friend_id_;
	p_stream->write_string(",friend_val=") << p->n_frival_;
}

// 新增好友度
BOOL db_handler::update_friendship(DWORD dw_role_id, LPVOID p_friendship_save)
{
	return common_update(dw_role_id, p_friendship_save, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_friendship));
}
VOID db_handler::makesql_update_friendship(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friendship_save)
{
	M_trans_pointer(p, p_friendship_save, s_friendship_save);

	p_stream->update_item("friendship");
	p_stream->write_string("friend_val=") << p->n_frival_;
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and friend_id=") << p->dw_friend_id_;
}

// 删除好友度
BOOL db_handler::delete_friendship(DWORD dw_role_id, DWORD dw_friend_id)
{
	return common_update(dw_role_id, &dw_friend_id, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_friendship));
}
VOID db_handler::makesql_delete_friendship(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friend_id)
{
	ASSERT(dw_role_id < (*(DWORD *)p_friend_id));

	p_stream->delete_item("friendship");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and friend_id=") << (*(DWORD *)p_friend_id);
}

// 删除角色时，删除所有好友度相关
BOOL db_handler::delete_all_friendship_relation(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_all_friendship_relation));
}
VOID db_handler::makesql_delete_all_friendship_relation(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	p_stream->delete_item("friendship");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" or friend_id=") << dw_role_id;
}

// 新加黑名单
BOOL db_handler::insert_black(DWORD dw_role_id, DWORD dw_black_id)
{
	return common_update(dw_role_id, &dw_black_id, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_black));
}
VOID db_handler::makesql_insert_black(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_black_id)
{
	p_stream->insert_item("blacklist");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",black_id=") << (*(DWORD *)p_black_id);
}

// 删除黑名单
BOOL db_handler::delete_black(DWORD dw_role_id, DWORD dw_black_id)
{
	return common_update(dw_role_id, &dw_black_id, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_black));
}
VOID db_handler::makesql_delete_black(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_black_id)
{
	p_stream->delete_item("blacklist");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and black_id=") << (*(DWORD *)p_black_id);
}

BOOL db_handler::delete_black_all(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_black_all));
}
VOID db_handler::makesql_delete_black_all(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_black_id)
{
	p_stream->delete_item("blacklist");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" or black_id=") << dw_role_id;
}

// 新加敌人
BOOL db_handler::insert_enemy(DWORD dw_role_id, DWORD dw_enemy_id)
{
	return common_update(dw_role_id, &dw_enemy_id, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_enemy));
}
VOID db_handler::makesql_insert_enemy(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_enemy_id)
{
	p_stream->insert_item("enemy");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",enemy_id=") << (*(DWORD *)p_enemy_id);
}

// 删除敌人
BOOL db_handler::delete_enemy(DWORD dw_role_id, DWORD dw_enemy_id)
{
	return common_update(dw_role_id, &dw_enemy_id, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_enemy));
}
VOID db_handler::makesql_delete_enemy(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_enemy_id)
{
	p_stream->delete_item("enemy");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and enemy_id=") << (*(DWORD *)p_enemy_id);
}

BOOL db_handler::delete_enemy_all(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_enemy_all));
}
VOID db_handler::makesql_delete_enemy_all(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_enemy_id)
{
	p_stream->delete_item("enemy");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" or enemy_id=") << dw_role_id;
}

// 新增称号
BOOL db_handler::insert_title(DWORD dw_role_id, LPVOID p_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_title));
}
VOID db_handler::makesql_insert_title(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, s_title_save);

	p_stream->replace_item("title");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",id=") << p->n_title_id_;
	p_stream->write_string(",count=") << p->dw_state_mark_;

	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_time);
	p_stream->write_string(",time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");

}

// 删除称号
BOOL db_handler::delete_title(DWORD dw_role_id, DWORD dw_title_id)
{
	return common_update(dw_role_id, &dw_title_id, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_title));
}

VOID db_handler::makesql_delete_title(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_title_id)
{
	p_stream->delete_item("title");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and id=") << (*(DWORD *)p_title_id);

}

// 新增元宝账户
BOOL db_handler::insert_yuanbao_account(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_yuanbao_account));
} 
VOID db_handler::makesql_insert_yuanbao_account(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->insert_item("yuanbao_account");
	p_stream->write_string("role_id=") << dw_role_id;
}

// 更新账户元宝
BOOL db_handler::update_account_yuanbao(DWORD dw_role_id, int n_yuanbao)
{
	return common_update(dw_role_id, &n_yuanbao, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_account_yuanbao));
}
VOID db_handler::makesql_update_account_yuanbao(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("yuanbao_account");
	p_stream->write_string("yuanbao=") <<(*(int*)p_buffer);

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 更新账户金钱
BOOL db_handler::update_account_silver(DWORD dw_role_id, LPVOID p_money)
{
	return common_update(dw_role_id, p_money, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_account_silver));
}
VOID db_handler::makesql_update_account_silver(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_money)
{
	M_trans_pointer(p, p_money, tagMoney);

	p_stream->update_item("yuanbao_account");
	p_stream->write_string("gold=") <<p->nGold;
	p_stream->write_string(",silver=") <<p->nSilver;

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 订单出售
BOOL db_handler::update_submit_sell(DWORD dw_role_id, bool b_sell)
{
	return common_update(dw_role_id, &b_sell, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_submit_sell));
}
VOID db_handler::makesql_update_submit_sell(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("yuanbao_account");
	p_stream->write_string("b_sell_order=") << (*(BYTE*)p_buffer);

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 订单收购
BOOL db_handler::update_submit_buy(DWORD dw_role_id, bool b_buy)
{
	return common_update(dw_role_id, &b_buy, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_submit_buy));
}
VOID db_handler::makesql_update_submit_buy(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("yuanbao_account");
	p_stream->write_string("b_buy_order=") << (*(BYTE*)p_buffer);

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 新增订单
BOOL db_handler::insert_one_order(DWORD dw_role_id, LPVOID p_order)
{
	return common_update(dw_role_id, p_order, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_one_order));
}
VOID db_handler::makesql_insert_one_order(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, tagYBOrder);

	p_stream->insert_item("yuanbao_order");
	p_stream->write_string("id=") << p->dwID;
	p_stream->write_string(",role_id=") << p->dw_role_id;
	p_stream->write_string(",order_type=") << p->nType;
	p_stream->write_string(",price=") << p->nPrice;
	p_stream->write_string(",number=") << p->n_count;
	p_stream->write_string(",order_mode=") << p->nYBMode;
	p_stream->write_string(",start_time=") << p->dwStartTime;
}

// 完成订单
BOOL db_handler::complete_one_order(DWORD dw_role_id, LPVOID p_order)
{
	return common_update(dw_role_id, p_order, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_complete_one_order));
}
VOID db_handler::makesql_complete_one_order(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, tagCompleteOrder);

	p_stream->update_item("yuanbao_order");
	p_stream->write_string("order_mode=") << p->nYBOMode;
	p_stream->write_string(",end_time=") << p->dwEndTime;

	p_stream->where_item();
	p_stream->write_string("id=") << p->dwID;
}

// 刷新订单
BOOL db_handler::refresh_one_order(DWORD dw_role_id, LPVOID p_order)
{
	return common_update(dw_role_id, p_order, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_refresh_one_order));
}
VOID db_handler::makesql_refresh_one_order(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, tagRefresh);

	p_stream->update_item("yuanbao_order");
	p_stream->write_string("avg_price=") << p->nAvgPrice;
	p_stream->write_string(",deal_num=") << p->nDealNum;

	p_stream->where_item();
	p_stream->write_string("id=") << p->dwID;
}

// 删除订单
BOOL db_handler::delete_one_order(DWORD dw_role_id, LPVOID p_del_time)
{
	return common_update(dw_role_id, p_del_time, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_one_order));
}
VOID db_handler::makesql_delete_one_order(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->delete_item("yuanbao_order");
	p_stream->where_item();
	p_stream->write_string("start_time<") << (*(DWORD*)p_buffer);
}

// 新增团购信息
BOOL db_handler::add_group_purchase_info(s_gp_info* p_gp_info)
{
	return common_update(INVALID_VALUE, p_gp_info, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_add_group_purchase_info));
}
VOID db_handler::makesql_add_group_purchase_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, s_gp_info);

	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->insert_item("group_purchase");
	p_stream->write_string("guild_id=") << p->dw_guild_id;
	p_stream->write_string(",role_id=") << p->dw_role_id;
	p_stream->write_string(",mall_id=") << p->dw_mall_id;
	p_stream->write_string(",item_price=") << p->n_price;
	p_stream->write_string(",participator_num=") << p->n_participator_num;
	p_stream->write_string(",require_num=") << p->n_require_num;
	p_stream->write_string(",remain_time=") << p->n_remain_time;
	p_stream->write_string(",participator_list='");
	p_stream->write_blob(p->dw_participators, sizeof(p->dw_participators), p_conn);
	p_stream->write_string("'");

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);
}

// 删除团购信息
BOOL db_handler::delete_group_purchase_info(s_gp_info_key* p_gp_info_key)
{
	return common_update(INVALID_VALUE, p_gp_info_key, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_group_purchase_info));
}
VOID db_handler::makesql_delete_group_purchase_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, s_gp_info_key);

	p_stream->delete_item("group_purchase");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << p->dw_guild_id;
	p_stream->write_string(" and role_id=") << p->dw_role_id;
	p_stream->write_string(" and mall_id=") << p->dw_mall_id;
}

// 删除帮派团购信息
BOOL db_handler::delete_guild_group_purchase_info(DWORD dw_guild_id)
{
	return common_update(dw_guild_id, NULL, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_guild_group_purchase_info));
}
VOID db_handler::makesql_delete_guild_group_purchase_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer)
{
	p_stream->delete_item("group_purchase");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << dw_guild_id;
}

// 更新团购信息
BOOL db_handler::update_group_purchase_info(s_gp_info_key* p_gp_info_key, DWORD dwNewParticipator)
{
	// 获得sql语句格式化io和连接
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	ASSERT(VALID_POINT(p_stream));
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	// 拼合SQL语句数据
	p_stream->select_item("group_purchase", "ParticipatorNum,ParticipatorList");
	p_stream->where_item();
	p_stream->write_string("guild_id=") << p_gp_info_key->dw_guild_id;
	p_stream->write_string(" and role_id=") << p_gp_info_key->dw_role_id;
	p_stream->write_string(" and mall_id=") << p_gp_info_key->dw_mall_id;

	// 执行SQL语句
	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);

#ifdef _DEBUG
	print_message(_T("%s\r\n"), get_tool()->ansi_to_unicode(p_stream->get_buffer()));
	print_message(_T("Results Row Count: %d\n"), VALID_POINT(p_result) ? p_result->get_row_count() : 0);
#endif

	// 归还sql格式化IO和连接
	game_db_interface_->return_use_connect(p_conn);
	game_db_interface_->return_io(p_stream);

	++dw_write_count_;

	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		// 构建新的响应者列表
		INT16 nParticipatorNum = (*p_result)[0].get_short();

		BYTE *p_buffer = new BYTE[sizeof(s_gp_info)+nParticipatorNum*sizeof(DWORD)];
		s_gp_info *p_gp_info = (s_gp_info*)p_buffer;
		(*p_result)[1].get_blob(p_gp_info->dw_participators, (*p_result)[1].get_length());
		p_gp_info->dw_participators[nParticipatorNum] = dwNewParticipator;
		p_gp_info->n_participator_num = nParticipatorNum + 1;
		p_gp_info->dw_guild_id = p_gp_info_key->dw_guild_id;
		p_gp_info->dw_role_id = p_gp_info_key->dw_role_id;
		p_gp_info->dw_mall_id = p_gp_info_key->dw_mall_id;

		// 更新到数据库
		common_update(INVALID_VALUE, p_gp_info, 1, INVALID_VALUE, 
			MakeDelegateOfDBHandler(&db_handler::makesql_update_group_purchase_info));

		SAFE_DELETE_ARRAY(p_buffer);
	}

	return TRUE;
}
VOID db_handler::makesql_update_group_purchase_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, s_gp_info);

	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->update_item("group_purchase");

	p_stream->write_string("participator_num=") << p->n_participator_num;

	//响应者列表
	p_stream->write_string(",participator_list='");
	p_stream->write_blob(p->dw_participators, p->n_participator_num * sizeof(DWORD), p_conn);
	p_stream->write_string("'");

	p_stream->where_item();
	p_stream->write_string("guild_id=") << p->dw_guild_id;
	p_stream->write_string(" and role_id=") << p->dw_role_id;
	p_stream->write_string(" and mall_id=") << p->dw_mall_id;

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);
}

// 更新团购时间
BOOL db_handler::update_group_purchase_time()
{
	// 更新到数据库
	common_update(INVALID_VALUE, NULL, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_group_purchase_time));

	return TRUE;
}
VOID db_handler::makesql_update_group_purchase_time(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("group_purchase");

	p_stream->write_string("remain_time=remain_time-60");
	p_stream->where_item();
	p_stream->write_string("remain_time>0");
}

// VIP 摊位申请
BOOL db_handler::apply_vip_stall(s_vip_stall_to_db* pInfo)
{
	// 更新到数据库
	common_update(INVALID_VALUE, pInfo, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_apply_vip_stall));

	return TRUE;
}
VOID db_handler::makesql_apply_vip_stall(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, s_vip_stall_to_db);

	p_stream->clear();
	p_stream->update_item("vip_stall");
	p_stream->write_string("role_id=") << p->dw_owner_id;
	p_stream->write_string(",remain_time=") <<p->n_remain_time;
	p_stream->where_item();
	p_stream->write_string("stall_id=") << p->by_stall_index;
}

// VIP摊位剩余时间
BOOL db_handler::update_vip_stall_time()
{
	// 更新到数据库
	common_update(INVALID_VALUE, NULL, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_vip_stall_time));

	return TRUE;
}
VOID db_handler::makesql_update_vip_stall_time(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->clear();

	p_stream->update_item("vip_stall");
	p_stream->write_string("remain_time=remain_time-60");
	p_stream->where_item();
	p_stream->write_string("remain_time>0");
}

// 更新宝箱数目
BOOL db_handler::update_treasure_count(DWORD dw_role_id, int& nSum)
{
	// 更新到数据库
	common_update(dw_role_id, &nSum, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_treasure_count));

	return TRUE;
}
VOID db_handler::makesql_update_treasure_count(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, DWORD);
	p_stream->clear();

	p_stream->update_item("role_data");
	p_stream->write_string("treasure_sum=") << *p;
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}