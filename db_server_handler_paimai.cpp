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
#include "db_server_handler.h"

#include "../common/ServerDefine/paimai_server_define.h"

// 获取拍卖最大编号
DWORD db_server::get_paimai_max_id(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_load_paimai_max_id send;
	send.dw_max_id = 0;

	if(!db_handler_->get_paimai_max_id(send.dw_max_id))
	{
		return INVALID_VALUE;
	}

	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return E_Success;
}

// 更新拍卖物品
DWORD db_server::update_paimai_item(DWORD p_msg, DWORD dw_reserver)
{
	NET_S2DB_update_paimai_item* p_recv = (NET_S2DB_update_paimai_item*)p_msg;

	if(!db_handler_->update_paimai_item(INVALID_VALUE, &p_recv->st_item, 1, NULL))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 保存拍卖信息
DWORD db_server::insert_paimai(DWORD p_msg, DWORD dw_reserver)
{
	NET_S2DB_insert_paimai* p_recv = (NET_S2DB_insert_paimai*)p_msg;

	if(!db_handler_->insert_paimai(p_recv))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 读取所有拍卖数据
DWORD db_server::load_all_paimai_info(DWORD p_msg, DWORD dw_reserver)
{
	char* p_buffer = new char[MAX_LOAD_PAI_BUFFER];
	if(!VALID_POINT(p_buffer))
		return INVALID_VALUE;

	db_handler_->load_all_paimai(p_buffer);

	SAFE_DELETE_ARRAY(p_buffer);

	NET_DB2S_load_all_paimai_end send;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return E_Success;
}

// 读取所有自动拍卖数据
DWORD db_server::load_all_auto_paimai(DWORD p_msg, DWORD dw_reserver)
{
	char* p_buffer = new char[MAX_LOAD_PAI_BUFFER];
	if(!VALID_POINT(p_buffer))
		return INVALID_VALUE;

	db_handler_->load_all_auto_paimai(p_buffer);

	SAFE_DELETE_ARRAY(p_buffer);

	NET_DB2S_load_auto_paimai_end send;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return E_Success;
}

// 检测是否已经拍卖
DWORD db_server::check_is_paimai(DWORD p_msg, DWORD dw_reserver)
{
	NET_S2DB_check_is_paimai* p_recv = (NET_S2DB_check_is_paimai*)p_msg;

	db_handler_->check_is_paimai(p_recv->dw_auto_paimai_id);
	return E_Success;
}

// 自动拍卖初始化完成
DWORD db_server::auto_paimai_init_ok(DWORD p_msg, DWORD dw_reserver)
{
	NET_DB2S_auto_paimai_init_ok send;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	return E_Success;
}

// 更新自动拍卖
DWORD db_server::update_auto_paimai(DWORD p_msg, DWORD dw_reserver)
{
	NET_S2DB_update_auto_paimai* p_recv = (NET_S2DB_update_auto_paimai*)p_msg;

	if(!db_handler_->update_auto_paimai(p_recv))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 读取拍卖物品
DWORD db_server::load_paimai_item(DWORD p_msg,	DWORD dw_reserver)
{
	char* p_buffer = new char[MAX_LOAD_PAI_BUFFER];
	if(!VALID_POINT(p_buffer))
		return INVALID_VALUE;

	db_handler_->load_paimai_item(p_buffer);

	SAFE_DELETE_ARRAY(p_buffer);

	NET_DB2S_load_paimai_item_end send;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	return E_Success;
}

// 删除拍卖
DWORD db_server::delete_paimai(DWORD p_msg, DWORD dw_reserver)
{
	NET_S2DB_delete_paimai* p_recv = (NET_S2DB_delete_paimai*)p_msg;

	if(!db_handler_->delete_paimai(p_recv))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 更新拍卖
DWORD db_server::update_paimai(DWORD p_msg, DWORD dw_reserver)
{
	NET_S2DB_update_paimai* p_recv = (NET_S2DB_update_paimai*)p_msg;

	if(!db_handler_->update_paimai(p_recv))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}