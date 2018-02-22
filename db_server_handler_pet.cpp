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
#include "../common/ServerDefine/pet_server_define.h"
#include "db_server_handler.h"
#include "db_handler.h"

DWORD db_server::create_new_pet(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p_recv, p_msg, NET_DB2C_create_pet_soul);
	NET_DB2C_create_pet_soul * p_recv = ( NET_DB2C_create_pet_soul * ) p_msg ;  
	DWORD dw_pet_id = INVALID_VALUE;

	// 创建宠物
	BOOL b_ret = db_handler_->insert_pet_soul(&p_recv->create_data, dw_pet_id);

	LPVOID p_buffer = g_mem_pool_safe.alloc(1024*10);
	NET_DB2S_create_pet_soul* p_send = (NET_DB2S_create_pet_soul*)p_buffer;
	p_send->dw_message_id = get_tool()->crc32("NET_DB2S_create_pet_soul");

	if (b_ret)
	{
		ASSERT(IS_PET(dw_pet_id));
		// 加载宠物
		PVOID p_load = (PVOID)&p_send->pet_data_load;
		db_handler_->load_one_pet(p_load, dw_pet_id);

		p_send->dw_error_code = E_Success;
	}
	else
	{
		// 返回错误代码
		p_send->dw_error_code = E_FAIL;
	}

	// 重新计算大小
	p_send->dw_size = sizeof(NET_DB2S_create_pet_soul) - sizeof(s_db_pet) + p_send->pet_data_load.get_size();

	p_server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

	g_mem_pool_safe._free(p_buffer);
	return E_Success;
}
DWORD db_server::delete_pet(DWORD p_msg, DWORD dw_reserve)
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_delete_pet_soul);
	for (int i=0; i<p_recv->n_pet_soul_num; ++i)
	{
		DWORD dwPetID = p_recv->dw_pet_id[i];
		if (IS_PET(dwPetID))
		{
			db_handler_->delete_pet_skill(INVALID_VALUE, dwPetID);
			db_handler_->delete_pet_equip(dwPetID);
			db_handler_->delete_pet_soul(dwPetID);
		}
	}
	return E_Success;
}

DWORD db_server::update_pet_attribute(DWORD p_msg, DWORD dw_reserve)
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_update_pet_soul);

	return db_handler_->update_pet_attribute(&p_recv->pet_att, p_recv->pet_att.dw_pet_id_);
}
DWORD db_server::insert_pet_skill(DWORD p_msg, DWORD dw_reserve)
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_insert_pet_skill);
	db_handler_->insert_pet_skill(&p_recv->pet_skill_, p_recv->dw_pet_id_);
	return E_Success;
}
DWORD db_server::delete_pet_skill(DWORD p_msg, DWORD dw_reserve)
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_delete_pet_skill);
	db_handler_->delete_pet_skill(p_recv->dw_pet_skill_type_id, p_recv->dw_pet_id_);
	return E_Success;
}

DWORD db_server::insert_pet_sns_info(DWORD p_msg, DWORD dw_reserve)
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_insert_pet_sns);
	db_handler_->insert_pet_sns_info(&p_recv->pet_sns_info);
	return E_Success;
}

DWORD db_server::delete_pet_sns_info(DWORD p_msg, DWORD dw_reserve)
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_delete_pet_sns);
	db_handler_->delete_pet_sns_info(p_recv->dw_pet_id);
	return E_Success;
}

DWORD db_server::load_pet_sns_info(DWORD p_msg, DWORD dw_reserve)
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_load_pet_sns);
	INT nInfoNum = 0;
	db_handler_->load_pet_sns_info(NULL, nInfoNum);
	if (nInfoNum == 0)
		return E_Success;

	
	DWORD dw_size = sizeof(NET_DB2S_load_pet_sns) + ((nInfoNum > 0) ? (nInfoNum - 1)*sizeof(tagPetSNSInfo) : 0);

	CREATE_MSG(pSend, dw_size, NET_DB2S_load_pet_sns);
	
	if (db_handler_->load_pet_sns_info(pSend->pet_sns_info, nInfoNum))
	{
		pSend->n_count = nInfoNum; 
	}

	p_server_->send_msg(GAMESERVERSESSIONID, pSend, pSend->dw_size);
	
	MDEL_MSG(pSend);

	return E_Success;
}