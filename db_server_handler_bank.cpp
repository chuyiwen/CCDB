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
#include "db_server_handler.h"
#include "db_handler.h"
#include "../common/ServerDefine/bank_server_define.h"

// 获取钱庄最大编号
DWORD	db_server::get_bank_max_id(DWORD p_msg, DWORD dw_reserver)
{
	NET_DB2S_get_max_bank_id send;
	send.dw_max_id = 0;

	if(!db_handler_->get_bank_max_id(send.dw_max_id))
	{
		return INVALID_VALUE;
	}

	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	return E_Success;
}

// 保存钱庄信息
DWORD	db_server::insert_bank(DWORD p_msg, DWORD dw_reserver)
{
	NET_S2DB_insert_bank* p_recv = (NET_S2DB_insert_bank*)p_msg;

	if(!db_handler_->insert_bank(p_recv))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 删除钱庄信息
DWORD	db_server::delete_bank(DWORD p_msg, DWORD dw_reserver)
{
	NET_S2DB_delete_bank* p_recv = (NET_S2DB_delete_bank*)p_msg;

	if(!db_handler_->delete_bank(p_recv))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 更新钱庄信息
DWORD	db_server::update_bank(DWORD p_msg, DWORD dw_reserver)
{
	NET_S2DB_update_bank* p_recv = (NET_S2DB_update_bank*)p_msg;

	if(!db_handler_->update_bank(p_recv))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 读取所有钱庄信息
DWORD	db_server::load_all_bank(DWORD p_msg, DWORD dw_reserver)
{
	char* p_buffer = new char[MAX_LOAD_BANK_BUFFER];
	if(!VALID_POINT(p_buffer))
		return INVALID_VALUE;

	if(!db_handler_->load_all_bank(p_buffer))
	{
		return INVALID_VALUE;
	}

	SAFE_DELETE_ARRAY(p_buffer);
	return E_Success;
}