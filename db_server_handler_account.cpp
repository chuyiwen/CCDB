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
#include "../common/ServerDefine/common_server_define.h"
// 更新安全码
DWORD db_server::set_safe_code(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_safe_code_set * p = ( NET_DB2C_safe_code_set * ) p_msg ;   ;  

	if(!db_handler_->update_safe_code(p->dw_account_id, p->dw_safe_code_crc))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 设置安全码重置时间
DWORD db_server::reset_safe_code(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_safe_code_reset * p = ( NET_DB2C_safe_code_reset * ) p_msg ;   ;  

	if(!db_handler_->update_safe_code_reset_time(p->dw_account_id, p->dw_reset_time))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 设置安全码重置时间
DWORD db_server::cancel_safe_code_reset(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_safe_code_reset_cancel * p = ( NET_DB2C_safe_code_reset_cancel * ) p_msg ;   ;  

	if(!db_handler_->update_safe_code_reset_time(p->dw_account_id, INVALID_VALUE))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 更新包裹密码
DWORD db_server::change_bag_password(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_change_bag_password * p = ( NET_DB2C_change_bag_password * ) p_msg ;   ;  

	if(!db_handler_->update_bag_password(p->dw_account_id, p->dw_bag_password))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 跟新角色仓库上限
DWORD db_server::update_ware_size(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_ware_size_update * p = ( NET_DB2C_ware_size_update * ) p_msg ;   ;  

	if(!db_handler_->update_ware_size(p->dw_account_id, &p))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 更新仓库货币
DWORD db_server::update_ware_money(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_ware_money_update * p = ( NET_DB2C_ware_money_update * ) p_msg ;   ;  

	if(!db_handler_->update_ware_money(p->dw_account_id, p->n64_ware_silver))
	{
		return INVALID_VALUE;
	}

	return 0;
}

// 更新包裹非绑定币
DWORD db_server::bag_meony_update(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_bag_money_update* p = (NET_DB2C_bag_money_update*)p_msg;

	if(!db_handler_->update_bag_money(p->dw_role_id, p))
	{
		return INVALID_VALUE;
	}
	
	return 0;
}

// 更新元宝
DWORD db_server::update_baibao_yuanbao(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_baibao_yuanbao_update * p = ( NET_DB2C_baibao_yuanbao_update * ) p_msg ;   ;  

	db_handler_->lock_table_account_common();

	if(!db_handler_->update_baibao_yuanbao(p->dw_account_id, &p->nBaiBaoYuanBao))
	{
		db_handler_->unlock_tables();
		return INVALID_VALUE;
	}

	db_handler_->unlock_tables();

	return 0;
}

// 更新积分
DWORD db_server::update_exchange_volume(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_exchange_volume_update* p = (NET_DB2C_exchange_volume_update*) p_msg;

	db_handler_->lock_table_account_common();

	if(!db_handler_->update_exchang_volume(p->dw_account_id, &p->n_volume))
	{
		db_handler_->unlock_tables();
		return INVALID_VALUE;
	}

	db_handler_->unlock_tables();

	return 0;
}

// 更新领取标志
DWORD db_server::update_account_receive(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_receive* p_recv = (NET_DB2C_update_receive*)p_msg;

	db_handler_->lock_table_account_common();

	if(!db_handler_->update_receive(p_recv->dw_account_id, NULL))
	{
		db_handler_->unlock_tables();

		return INVALID_VALUE;
	}

	db_handler_->unlock_tables();

	return 0;
}

// 更新领取标志
DWORD db_server::update_account_receive_ex(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_receive_ex* p_recv = (NET_DB2C_update_receive_ex*)p_msg;

	db_handler_->lock_table_account_common();

	if(!db_handler_->update_receive_ex(p_recv->dw_account_id, &p_recv->dw_receive_type))
	{
		db_handler_->unlock_tables();

		return INVALID_VALUE;
	}

	db_handler_->unlock_tables();

	return 0;
}

// 更新网页领奖标志
DWORD db_server::update_web_recieve(DWORD p_msg,	DWORD dw_reserve)
{
	NET_DB2C_update_web_recieve* p_recv = (NET_DB2C_update_web_recieve*)p_msg;

	db_handler_->lock_table_account_common();

	if(!db_handler_->update_web_receive(p_recv->dw_account_id, NULL))
	{
		db_handler_->unlock_tables();
		return INVALID_VALUE;
	}

	db_handler_->unlock_tables();

	return 0;
}

// 删除序列号礼包
DWORD db_server::delete_serial_reward(DWORD p_msg,	DWORD dw_reserve)
{
	NET_DB2C_delete_serial_reward* p_recv = (NET_DB2C_delete_serial_reward*)p_msg;

	db_handler_->lock_tables("serial_reward");

	if(!db_handler_->delete_serial_reward(p_recv->dw_account_id, p_recv->sz_serial))
	{
		db_handler_->unlock_tables();
		return INVALID_VALUE;
	}

	db_handler_->unlock_tables();

	return 0;
}