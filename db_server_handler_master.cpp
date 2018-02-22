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
 *	@file		db_server_handler_master
 *	@author		mwh
 *	@date		2011/02/12	initial
 *	@version	0.0.1.0
 *	@brief		师徒
*/

#include "StdAfx.h"
#include "db_server_handler.h"
#include "db_handler.h"
#include "../common/ServerDefine/master_apprentice_server_define.h"

DWORD db_server::load_all_master_prentices( DWORD p_msg, DWORD dw_reserve )
{
	BYTE* p_buffer = new BYTE[MASTERPRENTICELOADMAXBUFF];
	if(!VALID_POINT(p_buffer)) return INVALID_VALUE;

	ZeroMemory(p_buffer,sizeof(BYTE)*MASTERPRENTICELOADMAXBUFF);
	NET_DB2S_load_all_master_prentice * p_send = ( NET_DB2S_load_all_master_prentice * ) p_buffer ;  
	NET_DB2S_load_all_master_prentice temp;
	p_send->dw_message_id = temp.dw_message_id;
	p_send->dw_size = temp.dw_size - sizeof(s_master_prentice_load);
	p_send->u32_number = 0;

	int size = 0;
	db_handler_->load_all_master_prentices( (LPVOID&)p_send, size );
	//p_server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size );
	SAFE_DELETE_ARRAY(p_buffer);

	return E_Success;
}
DWORD db_server::load_all_master_recruit( DWORD p_msg, DWORD dw_reserve )
{
	BYTE* p_buffer = new BYTE[MASTERRECRUITBUFFER];
	if(!VALID_POINT(p_buffer)) return INVALID_VALUE;

	ZeroMemory(p_buffer,sizeof(BYTE)*MASTERRECRUITBUFFER);
	NET_DB2S_load_all_master_recruit temp;
	NET_DB2S_load_all_master_recruit* p_send = (NET_DB2S_load_all_master_recruit*)p_buffer;
	p_send->dw_message_id = temp.dw_message_id;
	p_send->u32_number = 0; p_send->dw_size = temp.dw_size - sizeof(DWORD);

	INT size = 0;
	db_handler_->load_all_master_recruit(p_buffer, size);
	//p_server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);
	SAFE_DELETE_ARRAY(p_buffer);
	return E_Success;
}
DWORD db_server::save_master_prentices(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p,p_msg,NET_DB2C_save_master_prentice);
	NET_DB2C_save_master_prentice * p = ( NET_DB2C_save_master_prentice * ) p_msg ;  
	return db_handler_->save_master_prentices( p->master_prentice, INVALID_VALUE, p->u32_number );
}

DWORD db_server::add_new_master_prentice(DWORD p_msg, DWORD dw_reserve )
{
	//GET_MESSAGE(p,p_msg,NET_DB2C_insert_master);
	NET_DB2C_insert_master * p = ( NET_DB2C_insert_master * ) p_msg ;  
	return db_handler_->add_new_master_prentices( p->dw_master );
}

DWORD db_server::update_master_id_and_forbid_time(DWORD p_msg, DWORD dw_reserve )
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_update_master_id_and_forbid_time);
	NET_DB2C_update_master_id_and_forbid_time * p = ( NET_DB2C_update_master_id_and_forbid_time * ) p_msg ;  
	return db_handler_->update_master_id_and_forbid_time( p->dw_role_id, p);
}

DWORD db_server::master_delete_role( DWORD p_msg, DWORD dw_reserve )
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_master_delete_role);
	NET_DB2C_master_delete_role * p = ( NET_DB2C_master_delete_role * ) p_msg ;  
	return db_handler_->master_delete_role( p->dw_role_id, p );
}
DWORD db_server::join_master_recruit( DWORD p_msg, DWORD dw_reserve )
{
	NET_DB2C_join_master_recruit*  p = (NET_DB2C_join_master_recruit*)p_msg;
	return db_handler_->join_master_recruit(p->dw_role_id);
}
DWORD db_server::leave_master_recruit( DWORD p_msg, DWORD dw_reserve )
{
	NET_DB2C_leave_master_recruit* p = (NET_DB2C_leave_master_recruit*)p_msg;
	return db_handler_->leave_master_recruit(p->dw_role_id);
}