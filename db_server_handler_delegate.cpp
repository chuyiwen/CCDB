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
 *	@file		db_server_handler_delegate
 *	@author		mwh
 *	@date		2011/02/12	initial
 *	@version	0.0.1.0
 *	@brief		数据库服务器网络消息处理
*/


#include "StdAfx.h"
#include "../common/ServerDefine/role_data_server_define.h"
#include "../common/ServerDefine/clandata_server_define.h"
#include "../common/ServerDefine/item_server_define.h"
#include "../common/ServerDefine/quest_server_define.h"
#include "../common/ServerDefine/title_server_define.h"
#include "../common/ServerDefine/vcard_server_define.h"
#include "../common/ServerDefine/famehall_server_define.h"
#include "../common/ServerDefine/yuanbao_server_define.h"
#include "../common/ServerDefine/mall_server_define.h"
#include "../common/ServerDefine/vipstall_server_define.h"
#include "../common/ServerDefine/activity_server_define.h"
#include "../common/ServerDefine/mail_server_define.h"
#include "../common/ServerDefine/rank_server_define.h"
#include "db_handler.h"
#include "db_server_handler.h"

// 每条消息中活动数据最多个数
const int MAX_ACTIVITY_NUM_PER_MSG = 50;	
// 每条消息中角色元宝订单最多个数
const int MAX_ROLEYBORDER_NUM_PER_MSG = 100;	
// 每条消息中离线玩家数据最多个数
const int MAX_ROLEINFO_NUM_PER_MSG = 100000;	
// 每条消息中团购信息最大容量
const int MAX_GROUPPURCHASE_CAPACITY_PER_MSG = 1000000;	
		
// 游戏服务器初始化成功
DWORD db_server::game_server_init_ok(DWORD p_msg, DWORD dw_reserve)
{
	p_server_->send_msg(GAMESERVERSESSIONID, (LPVOID)p_msg, ((NET_retification*)p_msg)->dw_size);
	return E_Success;
}
// 游戏服务器关闭
DWORD db_server::game_server_showdown(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_world_shutdown send;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	return E_Success;
}

// 向GameServer返回初始化完成
DWORD db_server::game_server_ok(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_world_init_ok	send;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size); 
	return E_Success;
}
// 获取所有玩家基本数据
DWORD db_server::load_all_role_simple(DWORD p_msg, DWORD dw_reserve)
{
	int n_role_count = db_handler_->get_role_count();
	if(0 == n_role_count)
	{
		NET_DB2S_load_all_role_info send;
		send.dw_error_code = E_Success;
		send.n_count = 0;
		p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
		return E_Success;
	}

	s_role_info *p_role_info = new s_role_info[n_role_count];
	s_role_info *p_start = p_role_info;
	int n_count;
	if(!db_handler_->load_all_role_simple(p_role_info, n_count))
	{
		delete []p_role_info;
		return INVALID_VALUE;
	}

	// 分配空间
	char *p_buffer = new char[MAX_ROLEINFO_NUM_PER_MSG * sizeof(s_role_info) + sizeof(NET_DB2S_load_all_role_info)];

	// 初始化消息头
	NET_DB2S_load_all_role_info load_temp;
	NET_DB2S_load_all_role_info *p_load = (NET_DB2S_load_all_role_info *)p_buffer;
	p_load->dw_error_code = E_Success;
	p_load->dw_message_id = load_temp.dw_message_id;
	p_load->dw_size = load_temp.dw_size;
	p_load->n_count = 0;

	while(n_count > 0)
	{
		if(n_count > MAX_ROLEINFO_NUM_PER_MSG)
		{
			memcpy(p_load->role_info_, p_start, MAX_ROLEINFO_NUM_PER_MSG * sizeof(s_role_info));

			// 重新设置消息大小
			p_load->dw_size = (MAX_ROLEINFO_NUM_PER_MSG - 1) 
				* sizeof(s_role_info) + sizeof(NET_DB2S_load_all_role_info);
			p_load->n_count = MAX_ROLEINFO_NUM_PER_MSG;

			n_count -= MAX_ROLEINFO_NUM_PER_MSG;
			p_start = &p_start[MAX_ROLEINFO_NUM_PER_MSG];
		}
		else
		{
			memcpy(p_load->role_info_, p_start, n_count * sizeof(s_role_info));

			// 重新设置消息大小
			p_load->dw_size = (n_count - 1) 
				* sizeof(s_role_info) + sizeof(NET_DB2S_load_all_role_info);
			p_load->n_count = n_count;

			n_count = 0;
			p_start = NULL;
		}

		p_server_->send_msg(GAMESERVERSESSIONID, p_load, p_load->dw_size);
	}

	// 释放内存
	delete []p_buffer;
	delete []p_role_info;

	return E_Success;
}

// 读取元宝账户
DWORD db_server::load_all_yuanbao_account(DWORD p_msg, DWORD dw_reserve)
{
	int n_account_count = db_handler_->get_account_count();
	if(0 == n_account_count)
	{
		NET_DB2S_load_all_yuanbao_account send;
		send.n_count = 0;
		p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
		return E_Success;
	}

	tagYBAccount *p_yuanbao_account = new tagYBAccount[n_account_count];
	tagYBAccount *p_start = p_yuanbao_account;
	int n_count;
	if(!db_handler_->load_all_yuanbao_account(p_yuanbao_account, n_count))
	{
		delete []p_yuanbao_account;
		return INVALID_VALUE;
	}

	// 分配空间
	char *p_buffer = new char[MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagYBAccount) + sizeof(NET_DB2S_load_all_yuanbao_account)];

	// 初始化消息头
	NET_DB2S_load_all_yuanbao_account load_temp;
	NET_DB2S_load_all_yuanbao_account *p_load = (NET_DB2S_load_all_yuanbao_account*)p_buffer;
	p_load->dw_message_id = load_temp.dw_message_id;
	p_load->dw_size = load_temp.dw_size;
	p_load->n_count = 0;

	while(n_account_count > 0)
	{
		if(n_account_count > MAX_ROLEINFO_NUM_PER_MSG)
		{
			memcpy(p_load->account, p_start, MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagYBAccount));

			// 重新设置消息大小
			p_load->dw_size = (MAX_ROLEINFO_NUM_PER_MSG - 1) 
				* sizeof(tagYBAccount) + sizeof(NET_DB2S_load_all_yuanbao_account);
			p_load->n_count = MAX_ROLEINFO_NUM_PER_MSG;

			n_account_count -= MAX_ROLEINFO_NUM_PER_MSG;
			p_start = &p_start[MAX_ROLEINFO_NUM_PER_MSG];
		}
		else
		{
			memcpy(p_load->account, p_start, n_account_count * sizeof(tagYBAccount));

			// 重新设置消息大小
			p_load->dw_size = (n_account_count - 1) 
				* sizeof(tagYBAccount) + sizeof(NET_DB2S_load_all_yuanbao_account);
			p_load->n_count = n_account_count;

			n_account_count = 0;
			p_start = NULL;
		}

		p_server_->send_msg(GAMESERVERSESSIONID, p_load, p_load->dw_size);
	}

	// 释放内存
	delete []p_buffer;
	delete []p_yuanbao_account;

	return E_Success;
}

// 获取所有元宝订单
DWORD db_server::load_all_yuanbao_order(DWORD p_msg, DWORD dw_reserve)
{
	int n_order_count = db_handler_->get_yuanbao_order_count();
	if(0 == n_order_count)
	{
		NET_DB2S_load_all_order send;
		send.n_count = 0;
		send.dw_max_order_id = db_handler_->get_yuanbao_max_order_id();
		p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
		return E_Success;
	}

	tagYuanBaoOrder *p_yuanbao_order = new tagYuanBaoOrder[n_order_count];
	tagYuanBaoOrder *p_start = p_yuanbao_order;
	int n_count;
	if(!db_handler_->load_all_yuanbao_order(p_yuanbao_order, n_count))
	{
		delete []p_yuanbao_order;
		return INVALID_VALUE;
	}

	// 分配空间
	char *p_buffer = new char[MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagYuanBaoOrder) + sizeof(NET_DB2S_load_all_order)];

	// 初始化消息头
	NET_DB2S_load_all_order load_temp; 
	NET_DB2S_load_all_order *p_load = (NET_DB2S_load_all_order*)p_buffer;
	p_load->dw_message_id = load_temp.dw_message_id;
	p_load->dw_size = load_temp.dw_size;
	p_load->n_count = 0;

	while(n_order_count > 0)
	{
		if(n_order_count > MAX_ROLEINFO_NUM_PER_MSG)
		{
			memcpy(p_load->yuanbao_order, p_start, MAX_ROLEINFO_NUM_PER_MSG * sizeof(tagYuanBaoOrder));

			// 重新设置消息大小
			p_load->dw_size = (MAX_ROLEINFO_NUM_PER_MSG - 1) 
				* sizeof(tagYuanBaoOrder) + sizeof(NET_DB2S_load_all_order);
			p_load->n_count = MAX_ROLEINFO_NUM_PER_MSG;

			n_order_count -= MAX_ROLEINFO_NUM_PER_MSG;
			p_start = &p_start[MAX_ROLEINFO_NUM_PER_MSG];
		}
		else
		{
			memcpy(p_load->yuanbao_order, p_start, n_order_count * sizeof(tagYuanBaoOrder));

			// 重新设置消息大小
			p_load->dw_size = (n_order_count - 1) 
				* sizeof(tagYuanBaoOrder) + sizeof(NET_DB2S_load_all_order);
			p_load->n_count = n_order_count;

			n_order_count = 0;
			p_start = NULL;
		}
		p_load->dw_max_order_id	= db_handler_->get_yuanbao_max_order_id();
		p_server_->send_msg(GAMESERVERSESSIONID, p_load, p_load->dw_size);
	}

	// 释放内存
	delete []p_buffer;
	delete []p_yuanbao_order;

	return E_Success;
}

// 读取角色元宝订单
DWORD db_server::load_one_role_all_yuanbao_order(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_get_role_yuanbao_order * p_recv = ( NET_DB2C_get_role_yuanbao_order * ) p_msg ;  

	int n_order_count = 0;
	db_handler_->get_one_role_yuanbao_order_count(p_recv->dw_role_id, n_order_count);

	if(0 == n_order_count)
	{
		NET_DB2S_get_role_yuanbao_order send;
		send.n_count = 0;
		p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
		return E_Success;
	}

	tagYuanBaoOrder *p_yuanbao_order = new tagYuanBaoOrder[n_order_count];
	tagYuanBaoOrder *p_start = p_yuanbao_order;
	int n_count;
	if(!db_handler_->load_one_role_all_yuanbao_order(p_yuanbao_order, n_count, p_recv->dw_role_id))
	{
		delete []p_yuanbao_order;
		return INVALID_VALUE;
	}

	// 分配空间
	char *p_buffer = new char[n_order_count * sizeof(tagYuanBaoOrder) + sizeof(NET_DB2S_get_role_yuanbao_order)];

	// 初始化消息头
	NET_DB2S_get_role_yuanbao_order load_temp; 
	NET_DB2S_get_role_yuanbao_order *p_load = (NET_DB2S_get_role_yuanbao_order*)p_buffer;
	p_load->dw_message_id = load_temp.dw_message_id;
	p_load->dw_size = load_temp.dw_size;
	p_load->n_count = 0;

	while(n_order_count > 0)
	{
		if(n_order_count > MAX_ROLEYBORDER_NUM_PER_MSG)
		{
			memcpy(p_load->yuanbao_order, p_start, MAX_ROLEYBORDER_NUM_PER_MSG * sizeof(tagYuanBaoOrder));

			// 重新设置消息大小
			p_load->dw_size = (MAX_ROLEYBORDER_NUM_PER_MSG - 1) 
				* sizeof(tagYuanBaoOrder) + sizeof(NET_DB2S_get_role_yuanbao_order);
			p_load->n_count = MAX_ROLEYBORDER_NUM_PER_MSG;

			n_order_count -= MAX_ROLEYBORDER_NUM_PER_MSG;
			p_start = &p_start[MAX_ROLEYBORDER_NUM_PER_MSG];
		}
		else
		{
			memcpy(p_load->yuanbao_order, p_start, n_order_count * sizeof(tagYuanBaoOrder));

			// 重新设置消息大小
			p_load->dw_size = (n_order_count - 1) 
				* sizeof(tagYuanBaoOrder) + sizeof(NET_DB2S_get_role_yuanbao_order);
			p_load->n_count = n_order_count;

			n_order_count = 0;
			p_start = NULL;
		}
		p_server_->send_msg(GAMESERVERSESSIONID, p_load, p_load->dw_size);
	}

	// 释放内存
	delete []p_buffer;
	delete []p_yuanbao_order;

	return E_Success;
}

// 获取角色简单数据
DWORD db_server::load_one_role_simple(DWORD p_msg, DWORD dw_reserve)
{
	DWORD dw_begin = timeGetTime();
	DWORD dw_end = timeGetTime();
	NET_DB2C_load_sim_role * p_recv = ( NET_DB2C_load_sim_role * ) p_msg ;  

	simple_role_->dw_error_code = E_Success;
	simple_role_->dw_account_id = p_recv->dw_account_id;
	simple_role_->s_account_common_.init();

	if(!db_handler_->load_one_role_simple(simple_role_->dw_account_id, simple_role_->n_count, simple_role_->sim_role_))
	{
		simple_role_->n_count = 0;		// 读取错误时，减小发包大小
	}

	// 锁表
	db_handler_->lock_table_account_common();

	// 读取安全码(若失败，则数据库连接出问题)
	if(!db_handler_->load_one_account_common(&simple_role_->s_account_common_, p_recv->dw_account_id))
	{
		// 向account_common表中插入记录
		if(!db_handler_->insert_account_common(p_recv->dw_account_id, p_recv->sz_account))
		{
			// 屏显
			print_message(_T("\n\n\n///////////////////////////////////////\n"));
			print_message(_T("Caution: \n\tCritical Error! DB connect lost! May be need to restart server!\n"));
			print_message(_T("///////////////////////////////////////\n\n\n"));

			simple_role_->dw_error_code = e_database_load_sim_role_failed;
		}

		// 再次读取
		db_handler_->load_one_account_common(&simple_role_->s_account_common_, p_recv->dw_account_id);
	}

	// 解锁
	db_handler_->unlock_tables();

	// 重新设置消息大小
	simple_role_->dw_size = sizeof(NET_DB2S_load_simrole) 
		- sizeof(tagSimRole) * (MAX_ROLENUM_ONEACCOUNT - simple_role_->n_count);

	// 发送到游戏服务器
	p_server_->send_msg(GAMESERVERSESSIONID, simple_role_, simple_role_->dw_size);

	dw_end = timeGetTime();
	DWORD dw_cross = dw_end - dw_begin;
	if(dw_cross > 1000)
	{
		print_message(_T("load_sim_role %u/r/n"), dw_cross);
	}
	
	return simple_role_->dw_error_code;
}

// 获取账号序列号礼包
DWORD db_server::load_serial_reward(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_serial_reward* p_recv = (NET_DB2C_load_serial_reward*)p_msg;

	//char* serial_buff = new char[X_SERIAL_BUFF_SIZE];
	//ZeroMemory(serial_buff, X_SERIAL_BUFF_SIZE);

	db_handler_->lock_tables("serial_reward");

	//db_handler_->load_serial_reward(serial_buff, p_recv->dw_account_id);
	db_handler_->load_serial_reward(p_recv, p_recv->dw_role_id);

	db_handler_->unlock_tables();

	/*NET_DB2S_load_serial_reward_finish send_finish;
	send_finish.dw_account_id = p_recv->dw_role_id;
	p_server_->send_msg(GAMESERVERSESSIONID, &send_finish, send_finish.dw_size);*/

	//SAFE_DELETE_ARRAY(serial_buff);

	return E_Success;
}

// 获取角色完整数据
DWORD db_server::load_one_role_full_data(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_role * p_recv = ( NET_DB2C_load_role * ) p_msg ;  

	ZeroMemory(buffer_, X_ROLE_BUFF_SIZE);
	full_role_->dw_message_id = full_role_->message_id_crc("NET_DB2S_load_role");
	full_role_->dw_account_id = p_recv->dw_account_id;
	full_role_->dw_error_code = E_Success;
	full_role_->dw_role_id = p_recv->dw_role_id;
	if(!db_handler_->load_one_role_full_data(p_recv->dw_account_id, p_recv->dw_role_id, &full_role_->RoleData))
	{
		full_role_->dw_error_code = e_database_load_role_data_failed;
	}

	// 重新计算full_role_的大小
	full_role_->dw_size = sizeof(NET_DB2S_load_role) + full_role_->RoleData.dw_size - sizeof(full_role_->RoleData);

	// 发送到游戏服务器
	p_server_->send_msg(GAMESERVERSESSIONID, full_role_, full_role_->dw_size);
	return full_role_->dw_error_code;
}

// 存储角色
VOID db_server::save_one_role(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_save_role * p_recv = ( NET_DB2C_save_role * ) p_msg ;  

	db_handler_->save_one_role(p_recv->dw_role_id, &p_recv->RoleData);
}
//更新玩家奖励数据信息
VOID db_server::update_one_role_reward_data(DWORD p_msg,DWORD dw_reserve)
{
	NET_DB2C_update_role_reward_data* p_recv = (NET_DB2C_update_role_reward_data*)p_msg;
	db_handler_->save_reward_data(p_recv->dw_role_id,p_recv->by_data,NULL);
}

// 创建角色
DWORD db_server::create_role(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_create_role * p_recv = ( NET_DB2C_create_role * ) p_msg ;  

	create_role_->dw_error_code = E_Success;
	create_role_->dw_account_id = p_recv->dw_account_id;
	memcpy(create_role_->s_sim_role_.szRoleName, 
		p_recv->s_role_info_.sz_role_name, sizeof(create_role_->s_sim_role_.szRoleName));


	DWORD dw_role_name_crc = INVALID_VALUE;
	DWORD dw_new_role_id = INVALID_VALUE;
	if(!db_handler_->create_role(dw_new_role_id, p_recv->dw_account_id, p_recv->s_role_info_, dw_role_name_crc))
	{
		create_role_->dw_error_code = e_database_insert_role_failed;
	}
	// 创角色成功
	else 
	{
		// 创建家族信息
		db_handler_->insert_clan_data(dw_new_role_id);

		// 创建角色名贴
		db_handler_->insert_visiting_card(dw_new_role_id);

		// 初始化角色基本信息
		memcpy(&create_role_->s_sim_role_.Avatar, &p_recv->s_role_info_.avatar, sizeof(create_role_->s_sim_role_.Avatar));
		create_role_->s_sim_role_.dw_role_id = dw_new_role_id;
		create_role_->s_sim_role_.dwMapID = p_recv->s_role_info_.dw_map_id_;
		create_role_->s_sim_role_.byLevel = p_recv->s_role_info_.by_level_;
		create_role_->s_sim_role_.eClassType = p_recv->s_role_info_.e_class_type_;
		create_role_->s_sim_role_.dwLoginTime = GetCurrentDWORDTime();
		create_role_->s_sim_role_.dwDelGuardTime = INVALID_VALUE;
		create_role_->s_sim_role_.dwChangeNameTime	= INVALID_VALUE;

		// 初始化tagRoleInfo
		memcpy(create_role_->s_role_info_.sz_role_name, 
			p_recv->s_role_info_.sz_role_name, sizeof(create_role_->s_role_info_.sz_role_name));
		create_role_->s_role_info_.dw_role_id = dw_new_role_id;
		create_role_->s_role_info_.dw_account_id = p_recv->dw_account_id;
		create_role_->s_role_info_.dw_role_name_crc = dw_role_name_crc;
		create_role_->s_role_info_.dw_map_id_ = p_recv->s_role_info_.dw_map_id_;
		create_role_->s_role_info_.by_level = p_recv->s_role_info_.by_level_;
		create_role_->s_role_info_.by_sex_ = p_recv->s_role_info_.avatar.bySex;
		create_role_->s_role_info_.e_class_type_ = p_recv->s_role_info_.e_class_type_;

	}
	p_server_->send_msg(GAMESERVERSESSIONID, create_role_, create_role_->dw_size);
	return simple_role_->dw_error_code;
}

// 删除角色
DWORD db_server::delete_role(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_role * p_recv = ( NET_DB2C_delete_role * ) p_msg ;  

	// 设置删除标志
	db_handler_->set_role_delete_flag(p_recv->dw_role_id, p_recv->dw_time);

	// 记录玩家相关信息
	db_handler_->register_deleted_role(p_recv->dw_account_id, p_recv->dw_role_id, p_recv->sz_role_name, p_recv->sz_ip);


	// 发送到游戏服务器
	NET_DB2S_delete_role send;
	send.dw_error_code = E_Success;
	send.dw_account_id = p_recv->dw_account_id;
	send.dw_role_id = p_recv->dw_role_id;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return send.dw_error_code;
}

// 恢复删除角色保护时间
DWORD db_server::role_guard_cancel_time(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_cancel_role_del_guard_time* p = (NET_DB2C_cancel_role_del_guard_time*)p_msg;

	db_handler_->role_guard_cancel_time(p->dw_account_id, p->dw_role_id);

	NET_DB2S_cancel_role_del_guard_time send;
	send.dw_error = E_Success;
	send.dw_role_id = p->dw_role_id;
	send.dw_account_id = p->dw_account_id;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return send.dw_error;
}

// 设置删除角色保护时间
DWORD db_server::role_guard_set_time(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_role_guard_time_set* p = (NET_DB2C_delete_role_guard_time_set*)p_msg;

	db_handler_->role_guard_set_time(p->dw_account_id, p->dw_role_id, p->dw_delGuardTime);

	NET_DB2S_delete_role_guard_time_set send;
	send.dw_error_code = E_Success;
	send.dw_role_id = p->dw_role_id;
	send.dw_account_id = p->dw_account_id;
	send.dw_delGuardTime = p->dw_delGuardTime;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return send.dw_error_code;
}

// 重新激活删除的角色
DWORD db_server::active_role(DWORD p_msg, DWORD dw_reserve)
{
	return 0;
}

// 角色改名
DWORD db_server::change_role_name(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_change_role_name * p_recv = ( NET_DB2C_change_role_name * ) p_msg ;  

	NET_DB2S_change_role_name	send;	
	send.dw_account_id = p_recv->dw_account_id;
	send.dw_role_id = p_recv->dw_role_id;
	send.dw_error_code = E_Success;
	send.dw_change_time = p_recv->dw_change_time;
	send.dw_name_crc = p_recv->dw_name_crc;
	memcpy(send.sz_new_role_name, p_recv->sz_new_role_name, sizeof(send.sz_new_role_name));

	if(!db_handler_->change_role_name(p_recv->dw_account_id, p_recv->dw_role_id, 
		p_recv->sz_new_role_name, p_recv->dw_name_crc, p_recv->dw_change_time))
	{
		send.dw_error_code = e_change_role_name_failed;
	}
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	return send.dw_error_code;
}

// 角色更改帮会
DWORD db_server::change_role_guild(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_change_role_guild * p_recv = ( NET_DB2C_change_role_guild * ) p_msg ;  

	if(!db_handler_->change_role_guild(p_recv->dw_role_id, p_recv->dw_guild_id))
	{
		ASSERT(0);
		print_message(_T("\n\nCaution:\n\tRole<id: %u> guildid change failed!\n"), p_recv->dw_role_id);
	}

	return E_Success;
}

// 角色离开帮会时间
DWORD db_server::update_role_leave_guild_time(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_change_role_leave_guild_time * p_recv = ( NET_DB2C_change_role_leave_guild_time * ) p_msg ;  

	if(!db_handler_->update_role_leave_guild_time(p_recv->dw_role_id, p_recv))
	{
		ASSERT(0);
		print_message(_T("\n\nCaution:\n\tRole<id: %u> change leave guild time!\n"), p_recv->dw_role_id);
	}

	return E_Success;
}
// 更新VIP信息
DWORD db_server::update_role_vip_info(DWORD p_msg,DWORD dw_reserver)
{
	NET_DB2C_change_role_VIP_Info * p_recv = ( NET_DB2C_change_role_VIP_Info * ) p_msg ; 
	if(!db_handler_->update_role_vip_info(p_recv->dw_role_id, p_recv))
	{
		ASSERT(0);
		print_message(_T("\n\nCaution:\n\tRole<id: %u> update role vip_info!\n"), p_recv->dw_role_id);
	}
	return E_Success;
}
//情况VIP信息
DWORD db_server::clean_role_vip_info(DWORD p_msg,DWORD dw_reserver)
{
	NET_C2DB_clear_role_VIP_info* p_recv = (NET_C2DB_clear_role_VIP_info*)p_msg;
	db_handler_->clear_role_vip_info(p_recv->dw_role_id,p_recv);
	return E_Success;
}
// 角色穿着模型ID
DWORD db_server::change_role_dress_model_id(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_clear_role_dress_mdid * p_recv = ( NET_DB2C_clear_role_dress_mdid * ) p_msg ;  

	if(!db_handler_->change_role_dress_model_id(p_recv->dw_role_id, p_recv->w_new_val))
	{
		ASSERT(0);
		print_message(_T("\n\nCaution:\n\tRole<id: %u> dressmdid change failed!\n"), p_recv->dw_role_id);
	}

	return E_Success;
}

// 按类型删除副本进度
DWORD db_server::delete_map_process(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_role_map_process* p_recv = (NET_DB2C_delete_role_map_process*)p_msg;

	if(!db_handler_->delete_inst_process_type(p_recv->n_type))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 更新进入限制
DWORD db_server::update_role_map_limit(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_role_map_limit * p_recv = ( NET_DB2C_delete_role_map_limit * ) p_msg ;  

	if(!db_handler_->update_role_map_limit(INVALID_VALUE, p_recv->n_type))
	{
		ASSERT(0);
		print_message(_T("\n\nCaution:\n\tmaplist change failed!\n"));
	}

	return E_Success;
}

// 删除进入限制
DWORD db_server::delete_role_map_limit(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_remove_role_map_limit * p_recv = ( NET_DB2C_remove_role_map_limit * ) p_msg ;  

	if(!db_handler_->delete_role_map_limit(p_recv->dw_role_id))
	{
		ASSERT(0);
		print_message(_T("\n\nCaution:\n\tmaplist del failed!\n"));
	}

	return E_Success;
}

// 新增进入限制
DWORD db_server::insert_role_map_limit(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_insert_role_map_limit * p_recv = ( NET_DB2C_insert_role_map_limit * ) p_msg ;  

	if(!db_handler_->insert_role_map_limit(p_recv->dw_role_id, &p_recv->st_enter_map_limit_))
	{
		ASSERT(0);
		print_message(_T("\n\nCaution:\n\tmaplist add failed!\n"));
	}

	return E_Success;
}

// 新加好友
DWORD db_server::insert_friend(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_insert_friend * p_recv = ( NET_DB2C_insert_friend * ) p_msg ;  

	if(!db_handler_->insert_friend(p_recv->dw_role_id, &p_recv->s_friend_save_))
	{
		return e_database_insert_friend_failed;
	}

	return E_Success;
}

// 更新好友
DWORD db_server::update_friend(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_friend * p_recv = ( NET_DB2C_update_friend * ) p_msg ;  

	if(!db_handler_->update_friend(p_recv->dw_role_id, &p_recv->s_friend_save_))
	{
		return e_database_insert_friend_failed;
	}

	return E_Success;
}

// 删除好友
DWORD db_server::delelte_friend(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_friend * p_recv = ( NET_DB2C_delete_friend * ) p_msg ;  

	if(!db_handler_->delelte_friend(p_recv->dw_role_id, p_recv->dw_friend_id_))
	{
		return e_database_delete_friend_failed;
	}

	return E_Success;
}

// 删除角色在好友表中所有记录
DWORD db_server::delete_all_friend_relation(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_all_friend * p_recv = ( NET_DB2C_delete_all_friend * ) p_msg ;  

	if(!db_handler_->delete_all_friend_relation(p_recv->dw_role_id))
	{
		return e_database_delete_friend_failed;
	}

	return E_Success;
}

// 新增好友度
DWORD db_server::insert_friendship(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_insert_friend_value * p_recv = ( NET_DB2C_insert_friend_value * ) p_msg ;  

	if(!db_handler_->insert_friendship(p_recv->dw_role_id, &p_recv->s_friendship_save_))
	{
		return e_database_insert_friendship_failed;
	}

	return E_Success;
}

// 新增好友度
DWORD db_server::update_friendship(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_friend_value * p_recv = ( NET_DB2C_update_friend_value * ) p_msg ;  

	if(!db_handler_->update_friendship(p_recv->dw_role_id, &p_recv->s_friendship_save_))
	{
		return e_database_insert_friendship_failed;
	}

	return E_Success;
}

// 删除好友度
DWORD db_server::delete_friendship(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_friend * p_recv = ( NET_DB2C_delete_friend * ) p_msg ;  

	if(!db_handler_->delete_friendship(p_recv->dw_role_id, p_recv->dw_friend_id_))
	{
		return e_database_delete_friendship_failed;
	}

	return E_Success;
}

// 删除角色时，删除所有好友度相关
DWORD db_server::delete_all_friendship_relation(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_all_friend_value * p_recv = ( NET_DB2C_delete_all_friend_value * ) p_msg ;  

	if(!db_handler_->delete_all_friendship_relation(p_recv->dw_role_id))
	{
		return e_database_delete_friendship_failed;
	}

	return E_Success;
}

// 新加黑名单
DWORD db_server::insert_black(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_insert_black * p_recv = ( NET_DB2C_insert_black * ) p_msg ;  

	if(!db_handler_->insert_black(p_recv->dw_role_id, p_recv->dw_black_id_))
	{
		return e_database_insert_black_failed;
	}

	return E_Success;
}

// 删除黑名单
DWORD db_server::delete_black(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_black * p_recv = ( NET_DB2C_delete_black * ) p_msg ;  

	if (p_recv->dw_black_id_ == INVALID_VALUE)
	{
		if(!db_handler_->delete_black_all(p_recv->dw_role_id))
		{
			return e_database_delete_black_failed;
		}
	}
	else
	{
		if(!db_handler_->delete_black(p_recv->dw_role_id, p_recv->dw_black_id_))
		{
			return e_database_delete_black_failed;
		}
	}
	return E_Success;
}

// 新加敌人
DWORD db_server::insert_enemy(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_insert_enemy * p_recv = ( NET_DB2C_insert_enemy * ) p_msg ;  

	NET_DB2S_insert_enemy send;
	send.dw_error_code = E_Success;
	send.dw_role_id = p_recv->dw_role_id;
	send.dw_enemy_id_ = p_recv->dw_enemy_id_;

	if(!db_handler_->insert_enemy(p_recv->dw_role_id, p_recv->dw_enemy_id_))
	{
		send.dw_error_code = e_database_insert_enemy_failed;
	}
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	return send.dw_error_code;
}

// 删除敌人
DWORD db_server::delete_enemy(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_enemy * p_recv = ( NET_DB2C_delete_enemy * ) p_msg ;  

	NET_DB2S_delete_enemy send;
	send.dw_error_code = E_Success;
	send.dw_role_id = p_recv->dw_role_id;
	send.dw_enemy_id = p_recv->dw_enemy_id_;
	
	if (p_recv->dw_enemy_id_ == INVALID_VALUE)
	{
		if(!db_handler_->delete_enemy_all(p_recv->dw_role_id))
		{
			send.dw_error_code = e_database_delete_enemy_falied;
		}
	}
	else
	{
		if(!db_handler_->delete_enemy(p_recv->dw_role_id, p_recv->dw_enemy_id_))
		{
			send.dw_error_code = e_database_delete_enemy_falied;
		}
	}
	

	// 发送到游戏服务器
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return send.dw_error_code;
}
// 获取百宝物品
DWORD db_server::get_world_max_min_serial(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_get_item_info send;

	db_handler_->get_world_max_min_serial(send.n64_max_serial, send.n64_min_serial);

	// 发送到游戏服务器
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	return 0;
}

// 获取百宝物品
DWORD db_server::load_item_baibao(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_baibao * p_recv = ( NET_DB2C_load_baibao * ) p_msg ;  

	if(p_recv->n_free_space_size_ <=0 || p_recv->n_free_space_size_ > SPACE_BAIBAO_BAG)
	{
		return INVALID_VALUE;
	}

	load_baibao_->dw_account_id = p_recv->dw_account_id;
	load_baibao_->dw_error_code = E_Success;

	int n_temp_size = 0;

	if(!db_handler_->load_item_baibao(p_recv->dw_account_id, p_recv->n_free_space_size_, p_recv->n64_serial, load_baibao_->n_ret_num_, n_temp_size, load_baibao_->by_data_))
	{
		load_baibao_->dw_error_code = e_database_baibao_failed;
	}

	// 重新计算消息大小
	load_baibao_->dw_size = sizeof(NET_DB2S_load_baibao) + n_temp_size - sizeof(load_baibao_->by_data_);

	// 发送到游戏服务器
	p_server_->send_msg(GAMESERVERSESSIONID, load_baibao_, load_baibao_->dw_size);

	return load_baibao_->dw_error_code;
}

// 读取百宝袋中元宝信息
DWORD db_server::load_baibao_yuanbao(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_baibao_yuanbao * p_recv = ( NET_DB2C_load_baibao_yuanbao * ) p_msg ;  

	db_handler_->lock_table_account_common();
	int nYuanBao;
	if (!db_handler_->load_baibao_yuanbao(p_recv->dw_account_id, nYuanBao))
	{
		db_handler_->unlock_tables();
		return INVALID_VALUE;
	}

	db_handler_->unlock_tables();

	if(nYuanBao >= p_recv->dw_recharge_num)
	{
		NET_DB2S_load_baobao_yuanbao send;
		send.dw_account_id	= p_recv->dw_account_id;
		send.n_baibao_yaunbao_	= p_recv->dw_recharge_num;

		// 发送到游戏服务器
		p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	}
	

	return 0;
}

// 读取网页奖励信息
DWORD db_server::load_web_receive(DWORD p_msg, DWORD dw_reserver)
{
	NET_DB2C_load_web_receive* p_recv = (NET_DB2C_load_web_receive*)p_msg;

	db_handler_->lock_table_account_common();
	DWORD	dw_web_receive = 0;
	if (!db_handler_->load_web_receive(p_recv->dw_account_id, dw_web_receive))
	{
		db_handler_->unlock_tables();
		return INVALID_VALUE;
	}

	db_handler_->unlock_tables();

	NET_DB2S_load_web_receive send;
	send.dw_account_id = p_recv->dw_account_id;
	send.dw_web_receive = dw_web_receive;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return 0;
}

// 更新元宝充值
DWORD db_server::update_yuanbao_recharge(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_yuanbao_recharge* p_recv = (NET_DB2C_update_yuanbao_recharge*)p_msg;

	db_handler_->lock_table_account_common();

	if(!db_handler_->update_yuanbao_recharge(p_recv->dw_account_id, p_recv->n32_yuanbao))
	{
		db_handler_->unlock_tables();
		return INVALID_VALUE;
	}

	db_handler_->unlock_tables();

	return 0;
}

// 更新元宝充值总量
DWORD db_server::update_total_recharge(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_total_recharge* p_recv = (NET_DB2C_update_total_recharge*)p_msg;

	db_handler_->lock_table_account_common();

	if(!db_handler_->update_total_recharge(p_recv->dw_account_id, &p_recv->n32_yuanbao))
	{
		db_handler_->unlock_tables();
		return INVALID_VALUE;
	}

	db_handler_->unlock_tables();

	return 0;
}

// 删除百宝物品
DWORD db_server::delete_baibao_item(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_baobao_item * p_recv = ( NET_DB2C_delete_baobao_item * ) p_msg ;  

	if(!db_handler_->delete_item(p_recv->n64_serial, "item_baibao"))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 从删除百宝装备
DWORD db_server::delete_baibao_equip(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_baibao_equip * p_recv = ( NET_DB2C_delete_baibao_equip * ) p_msg ;  

	if(db_handler_->delete_item(p_recv->n64_serial, "item_baibao"))
	{
		return db_handler_->delete_equip_special(p_recv->n64_serial, "equip_baibao");
	}

	return FALSE;
}

// 获取需log的item类型
DWORD db_server::load_item_need_log(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_load_item_need_log send;
	//send.n_item_num		= 0;
	send.dw_error_code	= E_Success;

	if(!db_handler_->load_item_need_log(send.sNeedLogItem, send.n_item_num))
	{
		send.dw_error_code = e_database_load_item_need_log_failed;
	}

	// 重新计算大小
	send.dw_size += (send.n_item_num - MAX_ITEM_NEEDLOG_NUM) * sizeof(s_need_log_item);

	// 发送到游戏服务器
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return 0;
}


// 新增物品
DWORD db_server::insert_item(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_new_item * p = ( NET_DB2C_new_item * ) p_msg ;  

	if(!db_handler_->insert_item(p->item))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 新增装备
DWORD db_server::insert_equip(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_new_equip * p = ( NET_DB2C_new_equip * ) p_msg ;

	if(!db_handler_->insert_equip(p->equip))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 删除物品
DWORD db_server::delete_item(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_item * p = ( NET_DB2C_delete_item * ) p_msg ;  

	if(!db_handler_->delete_item(p->n64_serial))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 删除装备
DWORD db_server::delete_equip(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_equip * p = ( NET_DB2C_delete_equip * ) p_msg ;  

	if(!db_handler_->delete_equip(p->n64_serial))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 保存装备属性
DWORD db_server::save_equip_special(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_equip_att * p = ( NET_DB2C_update_equip_att * ) p_msg ;  

	if(!db_handler_->save_equip_special(p->equip_spec_update))
	{
		return INVALID_VALUE;
	}

	return 0;
}
// 新增百宝物品
DWORD db_server::insert_baibao_item(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_new_baibao_item * p = ( NET_DB2C_new_baibao_item * ) p_msg ;  

	if(!db_handler_->insert_item(p->item, "item_baibao"))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 新增百宝装备
DWORD db_server::insert_baibao_equip(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_new_baibao_equip * p = ( NET_DB2C_new_baibao_equip * ) p_msg ;  

	if(!db_handler_->insert_equip(p->equip, TRUE))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 新增称号
DWORD db_server::insert_title(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_title_insert * p = ( NET_DB2C_title_insert * ) p_msg ;  

	if(!db_handler_->insert_title(p->dw_role_id, &p->s_title_save_))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 删除称号
DWORD db_server::delete_title(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_title_delete * p = ( NET_DB2C_title_delete * ) p_msg ;  

	if(!db_handler_->delete_title(p->dw_role_id, p->dw_title_id))
	{
		return INVALID_VALUE;
	}

	return 0;
}


// 更新角色名片
DWORD db_server::update_visiting_card(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_vcard_change * p = ( NET_DB2C_vcard_change * ) p_msg ;  

	if (!db_handler_->update_visiting_card(p->dw_role_id, &(p->s_card_data_)))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 获取角色名片
DWORD db_server::load_visiting_card(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_get_off_line_vcard * p_recv = ( NET_DB2C_get_off_line_vcard * ) p_msg ;  

	CREATE_MSG(p_send, 2000, NET_DB2S_get_off_line_vcard);

	p_send->dw_role_id = p_recv->dw_role_id;
	p_send->dw_query_id = p_recv->dw_query_id;
	PVOID pDataLoad = p_send->by_data;
	
	if (db_handler_->load_visiting_card(pDataLoad, p_recv->dw_role_id))
	{
		p_send->dw_error_code = E_VCard_Success;
		DWORD dwVCardSize =(DWORD)( (BYTE*)pDataLoad - p_send->by_data );
		p_send->dw_size = sizeof(NET_DB2S_get_off_line_vcard) - 1 + dwVCardSize;
	}
	else
	{
		p_send->dw_error_code = E_VCard_Failed;
		p_send->dw_size = sizeof(NET_DB2S_get_off_line_vcard);
	}

	p_server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);		
	MDEL_MSG(p_send);

	return 0;
}

// 新增技能
DWORD db_server::insert_skill(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_add_skill * p = ( NET_DB2C_add_skill * ) p_msg ;  

	if(!db_handler_->insert_skill(p->dw_role_id, &p->s_skill_))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 删除技能
DWORD db_server::delete_skill(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_remove_skill * p = ( NET_DB2C_remove_skill * ) p_msg ;  

	if(!db_handler_->delete_skill(p->dw_role_id, p->dw_skill_id))
	{
		return INVALID_VALUE;
	}

	return 0;
}

//更新技能
DWORD db_server::update_skill(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_skill * p = ( NET_DB2C_update_skill * ) p_msg ;  

	if(!db_handler_->update_skill(p->dw_role_id, &p->s_skill_))
		return INVALID_VALUE;

	return 0;
}


// 新增元宝账户
DWORD db_server::insert_yuanbao_account(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_insert_account * p_recv = ( NET_DB2C_insert_account * ) p_msg ;  

	if(!db_handler_->insert_yuanbao_account(p_recv->dw_role_id))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 更新账户元宝
DWORD db_server::update_account_yuanbao(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_syn_account_yanbao * p_recv = ( NET_DB2C_syn_account_yanbao * ) p_msg ;  

	if(!db_handler_->update_account_yuanbao(p_recv->dw_role_id, p_recv->n_yuanbao))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 更新账户金钱
DWORD db_server::update_account_silver(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_syn_account_silver * p_recv = ( NET_DB2C_syn_account_silver * ) p_msg ;  

	if(!db_handler_->update_account_silver(p_recv->dw_role_id, &p_recv->money))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 订单出售
DWORD db_server::update_submit_sell(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_syn_submit_sell * p_recv = ( NET_DB2C_syn_submit_sell * ) p_msg ;  

	if(!db_handler_->update_submit_sell(p_recv->dw_role_id, p_recv->b_sell_order))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 订单收购
DWORD db_server::update_submit_buy(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_syn_submit_buy * p_recv = ( NET_DB2C_syn_submit_buy * ) p_msg ;  

	if (!db_handler_->update_submit_buy(p_recv->dw_role_id, p_recv->b_buy_order))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 新增订单
DWORD db_server::insert_one_order(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_insert_order * p_recv = ( NET_DB2C_insert_order * ) p_msg ;  

	if(!db_handler_->insert_one_order(p_recv->yb_order.dw_role_id, &p_recv->yb_order))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 完成订单
DWORD db_server::complete_one_order(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_complete_order * p_recv = ( NET_DB2C_complete_order * ) p_msg ;  

	if(!db_handler_->complete_one_order(INVALID_VALUE, &p_recv->complete_order))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 刷新订单
DWORD db_server::refresh_one_order(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_refresh_order * p_recv = ( NET_DB2C_refresh_order * ) p_msg ;  

	if(!db_handler_->refresh_one_order(INVALID_VALUE, &p_recv->refresh_order))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 删除订单
DWORD db_server::delete_one_order(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_yuanbao_order * p_recv = ( NET_DB2C_delete_yuanbao_order * ) p_msg ;  

	if(!db_handler_->delete_one_order(INVALID_VALUE, &p_recv->dw_delete_time))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 获取百宝日志
DWORD db_server::load_baibao_log_info(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_baobao_log * p_recv = ( NET_DB2C_load_baobao_log * ) p_msg ;  

	ZeroMemory(buffer_, X_ROLE_BUFF_SIZE);
	load_baibao_log_->dw_message_id = load_baibao_log_->message_id_crc("NET_DB2S_load_baibao_log");
	load_baibao_log_->dw_account_id = p_recv->dw_account_id;
	load_baibao_log_->dw_error_code = E_Success;

	int n_temp_size = 0;

	if (!db_handler_->load_baibao_log_info(p_recv->dw_account_id, load_baibao_log_->n_log_num_, n_temp_size, load_baibao_log_->by_data_))
	{
		load_baibao_log_->dw_error_code = e_database_load_baibao_log_failed;
	}

	// 重新计算消息大小
	load_baibao_log_->dw_size = sizeof(NET_DB2S_load_baibao_log) + n_temp_size - sizeof(load_baibao_log_->by_data_);

	// 发送到游戏服务器
	p_server_->send_msg(GAMESERVERSESSIONID, load_baibao_log_, load_baibao_log_->dw_size);
	return load_baibao_log_->dw_error_code;
}

// 新增百宝日志
DWORD db_server::insert_baibao_log_info(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_add_baibao_log * p_recv = ( NET_DB2C_add_baibao_log * ) p_msg ;  

	return db_handler_->insert_baibao_log_info(p_recv->dw_account_id, (LPVOID)&p_recv->log_info_);
}

// 获取所有团购信息
DWORD db_server::load_all_group_purchase_info(DWORD p_msg, DWORD dw_reserve)
{
	int n_gp_count = db_handler_->get_group_purchase_info_count();
	if(0 == n_gp_count)
	{
		NET_DB2S_get_all_gp_info send;
		send.dw_error_code = E_Success;
		send.n_gp_info_num_ = 0;
		p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
		return E_Success;
	}

	s_gp_info *p_gp_info = (s_gp_info*)(new BYTE[db_handler_->get_group_purchate_info_size()]);
	s_gp_info *p_start = p_gp_info;

	if (!db_handler_->load_all_group_purchase_info(p_gp_info, n_gp_count))
	{
		NET_DB2S_get_all_gp_info send;
		send.dw_error_code = edbl_group_purchase_faild;
		send.n_gp_info_num_ = 0;
		p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);		
		return INVALID_VALUE;
	}

	// 分配空间
	char *p_buffer = new char[MAX_GROUPPURCHASE_CAPACITY_PER_MSG + sizeof(NET_DB2S_get_all_gp_info)];
	ZeroMemory(p_buffer, MAX_GROUPPURCHASE_CAPACITY_PER_MSG + sizeof(NET_DB2S_get_all_gp_info));

	// 初始化消息头
	NET_DB2S_get_all_gp_info load_temp; 
	NET_DB2S_get_all_gp_info *p_load = (NET_DB2S_get_all_gp_info*)p_buffer;
	p_load->dw_message_id = load_temp.dw_message_id;
	p_load->dw_size = load_temp.dw_size - sizeof(s_gp_info);
	p_load->n_gp_info_num_ = 0;

	s_gp_info *p_temp_gp_info = p_start;
	while (n_gp_count > 0)
	{
		p_load->dw_size = load_temp.dw_size - sizeof(s_gp_info);
		p_load->n_gp_info_num_ = 0;

		int nTmpSize = 0;

		while (nTmpSize < MAX_GROUPPURCHASE_CAPACITY_PER_MSG)
		{
			int nCurSize = p_temp_gp_info->size();

			
			ASSERT(nCurSize <= MAX_GROUPPURCHASE_CAPACITY_PER_MSG);

			if (nTmpSize + nCurSize > MAX_GROUPPURCHASE_CAPACITY_PER_MSG)
			{
				break;
			}
			else
			{
				nTmpSize += nCurSize;
				p_temp_gp_info = (s_gp_info*)((BYTE*)p_temp_gp_info + nCurSize);

				p_load->n_gp_info_num_++;

				n_gp_count--;
				if (n_gp_count <= 0)
				{
					break;
				}
			}
		}

		p_load->dw_size += nTmpSize;
		memcpy(p_load->gp_info_data_, p_start, nTmpSize);
		p_start = p_temp_gp_info;
		p_server_->send_msg(GAMESERVERSESSIONID, p_load, p_load->dw_size);
	}

	// 释放内存
	SAFE_DELETE_ARRAY(p_buffer);
	SAFE_DELETE_ARRAY(p_gp_info);

	return E_Success;
}

// 新增团购信息
DWORD db_server::add_group_purchase_info(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_add_gp_info * p_recv = ( NET_DB2C_add_gp_info * ) p_msg ;  

	return db_handler_->add_group_purchase_info(&(p_recv->gp_info));
}

// 删除团购信息
DWORD db_server::delete_group_purchase_info(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_remove_gp_info * p_recv = ( NET_DB2C_remove_gp_info * ) p_msg ;  

	return db_handler_->delete_group_purchase_info(&(p_recv->gp_info_key_));
}

// 删除帮派团购信息
DWORD db_server::delete_guild_group_purchase_info(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_remove_guild_gp_info * p_recv = ( NET_DB2C_remove_guild_gp_info * ) p_msg ;  

	return db_handler_->delete_guild_group_purchase_info(p_recv->dw_guild_id);
}

// 更新团购信息
DWORD db_server::update_group_purchase_info(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_gp_info * p_recv = ( NET_DB2C_update_gp_info * ) p_msg ;  

	return db_handler_->update_group_purchase_info(&(p_recv->gp_info_key_), p_recv->dw_new_participator_);
}

// 更新团购时间
DWORD db_server::update_group_purchase_time(DWORD p_msg, DWORD dw_reserve)
{
	return db_handler_->update_group_purchase_time();
}

// 获取获取VIP摊位
DWORD db_server::get_all_vip_stall_info(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_get_all_vip_stall_info send;

	// 初始化所有VIP摊位数据
	for (int i=0; i<VIP_STALL_MAX_NUM; i++)
	{
		send.vip_stall_info[i].by_stall_index	= INVALID_VALUE;
		send.vip_stall_info[i].dw_owner_id		= INVALID_VALUE;
		send.vip_stall_info[i].n_remain_time	= INVALID_VALUE;
	}

	if (!db_handler_->get_all_vip_stall_info(send.vip_stall_info))
	{
		return INVALID_VALUE;
	}
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	return E_Success;
}

// VIP 摊位申请
DWORD db_server::apply_vip_stall(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_apply_vip_stall * p_recv = ( NET_DB2C_apply_vip_stall * ) p_msg ;  

	return db_handler_->apply_vip_stall(&(p_recv->vip_stall_info));
}

// VIP摊位剩余时间
DWORD db_server::update_vip_stall_time(DWORD p_msg, DWORD dw_reserve)
{
	return db_handler_->update_vip_stall_time();
}

// 读取所有活动数据
DWORD db_server::load_activities(DWORD p_msg, DWORD dw_reserve)
{
	int n_activity_count = db_handler_->get_activity_count();

	if(0==n_activity_count)
	{
		NET_DB2S_load_activity_data send;
		send.n_count = 0;
		p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
		return E_Success;
	}

	s_activity_data *p_activity_data = new s_activity_data[n_activity_count];
	s_activity_data *p_start = p_activity_data;

	int n_count;
	if(!db_handler_->load_activities(p_activity_data, n_count))
	{
		delete []p_activity_data;
		return INVALID_VALUE;
	}

	// 分配空间
	char *p_buffer = new char[MAX_ACTIVITY_NUM_PER_MSG * sizeof(s_activity_data) + sizeof(NET_DB2S_load_activity_data)];

	// 初始化消息头
	NET_DB2S_load_activity_data load_temp;
	NET_DB2S_load_activity_data *p_load = (NET_DB2S_load_activity_data*)p_buffer;
	p_load->dw_message_id = load_temp.dw_message_id;
	p_load->dw_size = load_temp.dw_size;
	p_load->n_count = 0;

	while(n_activity_count > 0)
	{
		if(n_activity_count > MAX_ACTIVITY_NUM_PER_MSG)
		{
			memcpy(p_load->activity, p_start, MAX_ACTIVITY_NUM_PER_MSG * sizeof(s_activity_data));

			// 重新设置消息大小
			p_load->dw_size = (MAX_ACTIVITY_NUM_PER_MSG - 1) 
				* sizeof(s_activity_data) + sizeof(NET_DB2S_load_activity_data);
			p_load->n_count = MAX_ACTIVITY_NUM_PER_MSG;

			n_activity_count -= MAX_ACTIVITY_NUM_PER_MSG;
			p_start = &p_start[MAX_ACTIVITY_NUM_PER_MSG];
		}
		else
		{
			memcpy(p_load->activity, p_start, n_activity_count * sizeof(s_activity_data));

			// 重新设置消息大小
			p_load->dw_size = (n_activity_count - 1) 
				* sizeof(s_activity_data) + sizeof(NET_DB2S_load_activity_data);
			p_load->n_count = n_activity_count;

			n_activity_count = 0;
			p_start = NULL;
		}
		p_server_->send_msg(GAMESERVERSESSIONID, p_load, p_load->dw_size);
	}

	// 释放内存
	delete []p_buffer;
	delete []p_activity_data;

	return E_Success;
}

// 存储某个活动数据
DWORD db_server::save_activity(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_save_activity_data * p_recv = ( NET_DB2C_save_activity_data * ) p_msg ;   ;  

	if(!db_handler_->save_activity(p_recv->activity.dw_id, &p_recv->activity))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 更新宝箱数目
DWORD db_server::update_treasure_count(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_treasure_sum * p_recv = ( NET_DB2C_update_treasure_sum * ) p_msg ;  

	if(!db_handler_->update_treasure_count(p_recv->dw_role_id, p_recv->n_sum_))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 保存邮件
DWORD db_server::save_mail(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_save_mail * p_recv = ( NET_DB2C_save_mail * ) p_msg ;  

	if(!db_handler_->save_mail(p_recv))
	{
		return INVALID_VALUE;
	}
	return E_Success;
}

// 保存邮件物品
DWORD db_server::update_mail_item(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_save_mail_item * p_recv = ( NET_DB2C_save_mail_item * ) p_msg ;  

	if(!db_handler_->update_mail_item(INVALID_VALUE, p_recv->by_buffer_, p_recv->n_count, NULL))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 获取所有邮件
DWORD db_server::load_all_mail(DWORD p_msg, DWORD dw_reserve)
{
	char* p_buffer = new char[MAX_LOAD_BUFFER];
	if(!VALID_POINT(p_buffer))
		return INVALID_VALUE;
	db_handler_->load_all_mail(p_buffer);

	SAFE_DELETE_ARRAY(p_buffer);

	NET_DB2S_load_all_mail_end send;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return E_Success;
}

// 获取邮件内容
DWORD db_server::load_mail_content(DWORD p_msg, DWORD dw_reserve)
{
	char* p_buffer = new char[MAX_LOAD_BUFFER];
	if(!VALID_POINT(p_buffer))
		return INVALID_VALUE;

	db_handler_->load_mail_content(p_buffer);

	SAFE_DELETE_ARRAY(p_buffer);

	NET_DB2S_load_mail_content_end send;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return E_Success;
}

// 获取邮件物品
DWORD db_server::load_mail_item(DWORD p_msg, DWORD dw_reserve)
{
	char* p_buffer = new char[MAX_LOAD_BUFFER];
	if(!VALID_POINT(p_buffer))
		return INVALID_VALUE;

	db_handler_->load_mail_item(p_buffer);

	SAFE_DELETE_ARRAY(p_buffer);

	NET_DB2S_load_mail_item_end send;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return E_Success;
}

// 获取邮件最大编号
DWORD db_server::get_mail_max_serial(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_get_mail_max_serial send;
	send.dw_max_mail_serial_ = 0;

	if(!db_handler_->get_mail_max_serial(send.dw_max_mail_serial_))
	{
		return INVALID_VALUE;
	}
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	return E_Success;
}

// 更新邮件
DWORD db_server::update_mail(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_mail * p_recv = ( NET_DB2C_update_mail * ) p_msg ;  

	if(!db_handler_->update_mail(p_recv))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// 删除邮件
DWORD db_server::delete_mail(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_delete_mail * p_recv = ( NET_DB2C_delete_mail * ) p_msg ;

	if(!db_handler_->delete_mail(p_recv))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// 更新角色发邮件计数
DWORD db_server::update_role_send_mail_count(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_role_send_num * p_recv = ( NET_DB2C_update_role_send_num * ) p_msg ;  

	if(!db_handler_->update_role_send_mail_count(p_recv))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 更新人物金钱
DWORD db_server::change_role_money(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_change_role_money * p_recv = ( NET_DB2C_change_role_money * ) p_msg ;  
	return E_Success;
}
// 读取等级排行榜
DWORD db_server::load_level_rank(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_level_randk * p_recv = ( NET_DB2C_load_level_randk * ) p_msg ;  

	if(!db_handler_->load_level_rank())
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 读取装备排行榜
DWORD db_server::load_equip_rank(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_equip_rank * p_recv = ( NET_DB2C_load_equip_rank * ) p_msg ;  

	NET_DB2S_load_equip_rank send;

	int n_count = 0;

	tagEquip st_equip[100];
	ZeroMemory(st_equip, sizeof(tagEquip)*100);
	LPVOID p_buffer = st_equip;

	if(!db_handler_->load_equip_rank(p_buffer, &n_count))
	{
		return INVALID_VALUE;
	}

	for(int i = 0; i < n_count; i++)
	{
		memcpy(&send.s_equip, &st_equip[i], sizeof(tagEquip));
		p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	}

	return E_Success;
}

// 读取帮会排行榜
DWORD db_server::load_guild_rank(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_guild_rank * p_recv = ( NET_DB2C_load_guild_rank * ) p_msg ;  

	if(!db_handler_->load_guild_rank())
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 读取恶人榜
DWORD db_server::load_kill_rank(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_kill_rank * p_recv = ( NET_DB2C_load_kill_rank * ) p_msg ;  

	if(!db_handler_->load_kill_rank())
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 读取正义榜
DWORD db_server::load_justice_rank(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_justice_rank* p_recv = (NET_DB2C_load_justice_rank*)p_msg;

	if(!db_handler_->load_justice_rank())
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 读取噬魂榜
DWORD db_server::load_shihun_rank(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_shihun_rank* p_recv = (NET_DB2C_load_shihun_rank*)p_msg;

	if(!db_handler_->load_shihun_rank())
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 成就点数榜
DWORD db_server::load_achpoint_rank(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_achievement_point_rank* p_recv = (NET_DB2C_load_achievement_point_rank*)p_msg;
	if(!db_handler_->load_achpoint_rank())
	{
		return INVALID_VALUE;
	}
	return E_Success;
}

// 成就数量榜
DWORD db_server::load_achnumber_rank(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_achievement_number_rank* p_recv = (NET_DB2C_load_achievement_number_rank*)p_msg;
	if(!db_handler_->load_achnumber_rank())
	{
		return INVALID_VALUE;
	}
	return E_Success;
}

// 坐骑榜
DWORD db_server::load_mounts_rank(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_mounts_rank* p_recv = (NET_DB2C_load_mounts_rank*)p_msg;
	if(!db_handler_->load_mounts_rank())
	{
		return INVALID_VALUE;
	}
	return E_Success;
}

// 充值榜
DWORD db_server::load_reach_rank(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_reach_rank* p_recv = (NET_DB2C_load_reach_rank*)p_msg;
	if(!db_handler_->load_reach_rank())
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 读取1v1积分榜
DWORD db_server::load_1v1_rank(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_load_1v1_score_rank* p_recv = (NET_DB2C_load_1v1_score_rank*)p_msg;

	if(!db_handler_->load_1v1_score_rank())
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// 读取师徒榜
DWORD db_server::load_master_rank(DWORD p_msg,DWORD dw_reserve)
{
	NET_DB2C_load_MasterGraduate_randk* p_recv = (NET_DB2C_load_MasterGraduate_randk*)p_msg;
	if (!db_handler_->load_master_rank())
	{
		return INVALID_VALUE;
	}
	return E_Success;
}
