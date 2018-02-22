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
#include "../common/ServerDefine/item_server_define.h"
#include "../common/ServerDefine/role_data_server_define.h"

// Ð´Èë²âÊÔ
DWORD db_server::force_test_load_role(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_test_load_role);
	NET_DB2C_test_load_role * p = ( NET_DB2C_test_load_role * ) p_msg ;  
	s_role_data_load tmp;
	LPVOID p_buffer = &tmp;
	if(!db_handler_->force_test_load_role(p_buffer, p->dw_account_id, p->dw_role_id))
	{
		return INVALID_VALUE;
	}

	return 0;
}


DWORD db_server::force_test_load_item(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_test_load_role_item);
	NET_DB2C_test_load_role_item * p = ( NET_DB2C_test_load_role_item * ) p_msg ;  
	static BYTE s_buffer[1024 * 1024];
	LPVOID p_buffer = (LPVOID)s_buffer;
	int n_count;

	if(!db_handler_->force_test_load_item(p_buffer, n_count, p->dw_account_id, p->dw_role_id))
	{
		return INVALID_VALUE;
	}

	return 0;
}


// Ð´Èë²âÊÔ
DWORD db_server::force_test_save_item(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_test_save_item);
	NET_DB2C_test_save_item * p = ( NET_DB2C_test_save_item * ) p_msg ;  
	if(!db_handler_->save_item(p->item))
	{
		return INVALID_VALUE;
	}

	return 0;
}
DWORD db_server::force_test_save_equip(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_test_save_equip);
	NET_DB2C_test_save_equip * p = ( NET_DB2C_test_save_equip * ) p_msg ;  
	if(!db_handler_->save_equip(p->equip))
	{
		return INVALID_VALUE;
	}

	return 0;
}
