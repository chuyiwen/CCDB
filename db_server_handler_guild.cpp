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
#include "../common/ServerDefine/guild_server_define.h"

#include <new>

#define MGuildMemMsgSz(n_count)	((n_count - 1) * sizeof(s_guild_member_load) + sizeof(NET_DB2S_load_all_guild_member))

// ��ȡ���а�������
DWORD db_server::load_all_guild(DWORD p_msg, DWORD dw_reserve)
{
	// ��������ڴ�
	char *p_buffer = new char[MAX_GUILD_BUFF_SIZE];
	ZeroMemory(p_buffer, sizeof(char) * MAX_GUILD_BUFF_SIZE);
	
	int n_guild_number = 0;
	if(db_handler_->load_all_guild(p_buffer, n_guild_number))
	{
		M_trans_pointer(p_guil_load, p_buffer, s_guild_load);
		NET_DB2S_load_all_guild send;
		for(int i=0; i<n_guild_number; ++i)
		{
			memcpy(&send.s_guild_load_, p_guil_load + i, sizeof(s_guild_load));
			p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
		}
	}

	SAFE_DELETE_ARRAY(p_buffer);

	return E_Success;
}

// ���ɳ�ʼ�����
DWORD db_server::check_guild_init_ok(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_guild_init_ok send;
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	return E_Success;
}

// ��������
DWORD db_server::create_guild(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_create_guild);
	
	NET_DB2C_create_guild * p = ( NET_DB2C_create_guild * ) p_msg ;  

	if(!db_handler_->insert_guild(&p->create_guild_info_))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// ��ɢ����
DWORD db_server::dismiss_guild(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_dismiss_guild);
	NET_DB2C_dismiss_guild * p = ( NET_DB2C_dismiss_guild * ) p_msg ;  
	if(!db_handler_->delete_guild(p->dw_guild_id))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

DWORD db_server::resetSignUpAttack(DWORD p_msg, DWORD dw_reserve)
{
	ET_DB2C_reset_signUpAttact* p = (ET_DB2C_reset_signUpAttact*)p_msg;
	if (!db_handler_->resetSignUpAttack())
	{
		return INVALID_VALUE;
	}
	return E_Success;
}
// �޸İ��ɽ���
DWORD db_server::change_guild_tenet(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_change_guild_tenet);
	NET_DB2C_change_guild_tenet * p = ( NET_DB2C_change_guild_tenet * ) p_msg ;  

	if(!db_handler_->update_guild_tenet(p->dw_guild_id, p->sz_new_tenet))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// �޸İ���ͼ��
DWORD db_server::change_guild_symbol(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_change_guild_symbol);
	NET_DB2C_change_guild_symbol * p = ( NET_DB2C_change_guild_symbol * ) p_msg ;  
	if(!db_handler_->update_guild_symbol(p->dw_guild_id, p->by_guild_symbol))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ���������������
DWORD db_server::save_guild_all(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_save_all_guild);
	NET_DB2C_save_all_guild * p = ( NET_DB2C_save_all_guild * ) p_msg ;  
	if(!db_handler_->update_guild_all(p->guild_base, p->n_guild_num))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

DWORD db_server::save_guild_all_plant(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_save_all_guild_plant* p = (NET_DB2C_save_all_guild_plant*)p_msg;
	if(!db_handler_->update_guild_all_plant(p->guild_plant, p->n_guild_num))
	{
		return INVALID_VALUE;
	}
	return E_Success;
}
// �����������
DWORD db_server::save_guild_attribute(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_save_guild_att);
	NET_DB2C_save_guild_att * p = ( NET_DB2C_save_guild_att * ) p_msg ;  
	if(!db_handler_->update_guild_attribute(&p->p_guild_base))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

DWORD db_server::save_guild_plant(DWORD p_msg, DWORD dw_reserve)
{	
	NET_DB2C_save_plant * p = ( NET_DB2C_save_plant * ) p_msg ;  
	if(!db_handler_->update_guild_plant(&p->p_guild_plant))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

DWORD db_server::load_sbk_guild(DWORD p_msg, DWORD dw_reserve)
{
	// ��������ڴ�
	DWORD dwGuildID = INVALID_VALUE;

	int n_number = 0;
	if(db_handler_->load_sbk_guild(&dwGuildID, n_number))
	{

		NET_DB2C_load_sbk send;
		send.dw_guild_id = dwGuildID;
		p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	}


	return E_Success;
}

DWORD db_server::save_sbk_guild(DWORD p_msg, DWORD dw_reserve)
{
	NET_C2DB_save_sbk* p = (NET_C2DB_save_sbk*)p_msg;
	if (!db_handler_->save_guild_sbk(p->dw_guild_id))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// �޸İ���ְλ����
DWORD db_server::change_guild_pos_name(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_change_guild_pos_name);
	NET_DB2C_change_guild_pos_name * p = ( NET_DB2C_change_guild_pos_name * ) p_msg ;  
	if(!db_handler_->update_guild_pos_name(p->dw_guild_id, p))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// �޸İ���ְλȨ��
DWORD db_server::update_guild_pos_power(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_change_guild_pos_power);
	NET_DB2C_change_guild_pos_power * p = ( NET_DB2C_change_guild_pos_power * ) p_msg ;  
	if(!db_handler_->update_guild_pos_power(p->dw_guild_id, p))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// ��������ֲ��Ϣ
DWORD db_server::load_guild_plant_data(DWORD p_msg, DWORD dw_reserve)
{
	// ��������ڴ�
	char *p_buffer = new char[MAX_GUILD_BUFF_SIZE];
	ZeroMemory(p_buffer, sizeof(char) * MAX_GUILD_BUFF_SIZE);

	int n_number = 0;
	if(db_handler_->load_all_guild_plant_data(p_buffer, n_number))
	{
		M_trans_pointer(p_guil_plant, p_buffer, s_guild_plant);
		NET_DB2C_load_guild_plant_data send;
		for(int i=0; i<n_number; ++i)
		{
			memcpy(&send.s_plant_data, p_guil_plant + i, sizeof(s_guild_plant));
			p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
		}
	}

	SAFE_DELETE_ARRAY(p_buffer);

	return E_Success;
}
// �½�����PVP����
DWORD db_server::insert_guild_pvp_data(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_insert_pvp_data);
	NET_DB2C_insert_pvp_data * p = ( NET_DB2C_insert_pvp_data * ) p_msg ;  

	if(!db_handler_->insert_guild_pvp_data(p->n_act_id, p))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ȡ�ð���PVP����
DWORD db_server::load_guild_pvp_data(DWORD p_msg, DWORD dw_reserve)
{
	// Ԥ�ȷ���һ���㹻��Ŀռ�
	char *p_buffer_load = new char[TEMP_GUILD_BUFF_SIZE];
	ZeroMemory(p_buffer_load, sizeof(char) * TEMP_GUILD_BUFF_SIZE);

	int nDataNum = 0;

	if(db_handler_->load_guild_pvp_data(p_buffer_load, nDataNum))
	{
		if(nDataNum > 0)
		{
			M_trans_pointer(p_buffer, p_buffer_load, s_load_guild_pvp_data);
			int nMsgSize = sizeof(NET_DB2S_load_pvp_data) + (nDataNum - 1)*sizeof(s_load_guild_pvp_data);
			CREATE_MSG(pSend, nMsgSize, NET_DB2S_load_pvp_data);

			memcpy(pSend->st_load_guild_pvp_data, p_buffer, nDataNum*sizeof(s_load_guild_pvp_data));
			pSend->n_count = nDataNum;
			p_server_->send_msg(GAMESERVERSESSIONID, pSend, pSend->dw_size);
			MDEL_MSG(pSend);
		}
	}

	SAFE_DELETE_ARRAY(p_buffer_load);

	return E_Success;
}
// ���°���PVP����
DWORD db_server::update_pvp_data(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_up_pvp_data);
	NET_DB2C_up_pvp_data * p = ( NET_DB2C_up_pvp_data * ) p_msg ;  
	if(!db_handler_->update_pvp_data(p->n_act_id, p))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ��ȡ�������г�Ա����
DWORD db_server::load_all_guild_member(DWORD p_msg, DWORD dw_reserve)
{
	// ��������ڴ�
	char *p_buffer_load = new char[MAX_GUILD_BUFF_SIZE];
	ZeroMemory(p_buffer_load, sizeof(char) * MAX_GUILD_BUFF_SIZE);

	int n_guild_member_number = 0;
	if(db_handler_->load_all_guild_member(p_buffer_load, n_guild_member_number))
	{
		char *p_buffer_send = new char[MGuildMemMsgSz(MAX_GUILDMEM_NUM_PER_MSG)];

		NET_DB2S_load_all_guild_member *p_send = (NET_DB2S_load_all_guild_member*)(p_buffer_send);
		ZeroMemory(p_buffer_send, MGuildMemMsgSz(MAX_GUILDMEM_NUM_PER_MSG));
		M_msg_init(p_send, NET_DB2S_load_all_guild_member);
				
		M_trans_pointer(p_start, p_buffer_load, s_guild_member_load);
		int n_send_left = n_guild_member_number;
		int n_send_number	= 0;
		while(n_send_left > 0)
		{
			p_start = &p_start[n_send_number];
			
			if(n_send_left > MAX_GUILDMEM_NUM_PER_MSG)
			{
				n_send_number = MAX_GUILDMEM_NUM_PER_MSG;
			}
			else
			{
				n_send_number = n_send_left;
			}
			
			memcpy(p_send->guild_member_load_, p_start, n_send_number * sizeof(s_guild_member_load));
			p_send->n_all_guild_num	= n_send_number;
			p_send->dw_size		= MGuildMemMsgSz(n_send_number);
			p_server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);
			n_send_left -= n_send_number;
		}

		SAFE_DELETE_ARRAY(p_buffer_send);
	}

	SAFE_DELETE_ARRAY(p_buffer_load);

	return E_Success;
}

// ��ȡ��ļ����Ϣ
DWORD db_server::load_guild_recruit(DWORD p_msg, DWORD dw_reserve)
{
	char *p_buffer_load = new char[MAX_GUILD_BUFF_SIZE];
	ZeroMemory(p_buffer_load, sizeof(char) * MAX_GUILD_BUFF_SIZE);

	db_handler_->load_guild_recruit(p_buffer_load);

	SAFE_DELETE_ARRAY(p_buffer_load);

	return E_Success;
}

// ��Ӱ����ļ��Ϣ
DWORD db_server::insert_guild_recruit(DWORD p_msg, DWORD dw_reserve)
{
	NET_C2DB_insert_guild_recruit* p_cmd = (NET_C2DB_insert_guild_recruit*)p_msg;

	if(!db_handler_->insert_guild_recruit(p_cmd->dw_role_id))
	{
		return INVALID_VALUE;
	}
	return E_Success;
}

// ɾ�������ļ��Ϣ
DWORD  db_server::delete_guild_recruit(DWORD p_msg, DWORD dw_reserve)
{
	NET_C2DB_delete_guild_recruit* p_cmd = (NET_C2DB_delete_guild_recruit*)p_msg;

	if(!db_handler_->delete_guild_recruit(p_cmd->dw_role_id))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// ��������ļ��
DWORD db_server::clean_guild_recruit(DWORD p_msg,	DWORD dw_reserve)
{
	if(!db_handler_->clean_guild_recruit())
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// ��ȡ���boss��ɱ
DWORD db_server::load_guild_skill_boss(DWORD p_msg, DWORD dw_reserve)
{
	if(!db_handler_->load_guild_boss_kill(NULL))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// ������boss��ɱ
DWORD db_server::insert_guild_kill_boss(DWORD p_msg, DWORD dw_reserve)
{
	NET_C2DB_insert_guild_skill_boss* p_recv = (NET_C2DB_insert_guild_skill_boss*)p_msg;

	if(!db_handler_->insert_guild_kill_boss(&p_recv->st_guild_kill_boss))
	{
		return INVALID_VALUE;
	}
	return E_Success;
}

// ��ȡ���ս��
DWORD db_server::load_guild_war_history(DWORD p_msg, DWORD dw_reserve)
{
	NET_C2DB_load_guild_war_history* p_recv = (NET_C2DB_load_guild_war_history*)p_msg;

	if(!db_handler_->load_guild_war_history(NULL))
	{
		return INVALID_VALUE;
	}
	return E_Success;
}

// ������ս��
DWORD db_server::insert_guild_war_history(DWORD p_msg, DWORD dw_reserve)
{
	NET_C2DB_insert_guild_war_history* p_recv = (NET_C2DB_insert_guild_war_history*)p_msg;

	if(!db_handler_->insert_guild_war_history(&p_recv->st_guild_war_history))
	{
		return INVALID_VALUE;
	}
	return E_Success;
}

DWORD db_server::delete_guild_war_history(DWORD p_msg, DWORD dw_reserver)
{
	NET_C2DB_delete_guild_war_history* p_recv = (NET_C2DB_delete_guild_war_history*)p_msg;

	if (!db_handler_->delete_guild_war_history(&p_recv->st_guild_war_history))
	{
		return INVALID_VALUE;
	}
	return E_Success;
}
// �������
DWORD db_server::join_guild(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_join_guild);
	NET_DB2C_join_guild * p = ( NET_DB2C_join_guild * ) p_msg ;  
	if(!db_handler_->insert_guild_member(p->guild_member_info))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// �˳�����
DWORD db_server::leave_guild(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_leave_guild);
	NET_DB2C_leave_guild * p = ( NET_DB2C_leave_guild * ) p_msg ;  
	if(!db_handler_->delete_guild_member(p->dw_role_id))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ���°��ɳ�Աְλ
DWORD db_server::change_guild_member_pos(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_change_guild_pos);
	NET_DB2C_change_guild_pos * p = ( NET_DB2C_change_guild_pos * ) p_msg ;  
	if(!db_handler_->update_guild_member_pos(p->dw_role_id, p->n_new_pos))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ���ð��ɳ�Ա���ʲֿ�Ȩ��
DWORD db_server::guild_ware_privilege(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_guild_ware_pri);
	NET_DB2C_guild_ware_pri * p = ( NET_DB2C_guild_ware_pri * ) p_msg ;  
	if (!db_handler_->guild_ware_privilege(p->dw_role_id, p->b_enable))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ���İ��ɳ�Ա���׶�
DWORD db_server::change_member_contribution(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_change_contrib);
	NET_DB2C_change_contrib * p = ( NET_DB2C_change_contrib * ) p_msg ;  
	if (!db_handler_->change_member_contribution((LPVOID)p))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ���ð��ɳ�ԱͶƱȨ��
DWORD db_server::change_ballot(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_change_ballot);
	NET_DB2C_change_ballot * p = ( NET_DB2C_change_ballot * ) p_msg ;  
	if(!db_handler_->change_ballot((LPVOID)p))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// �Ƿ��ܲμӰ��ս
//DWORD db_server::change_war(DWORD p_msg, DWORD dw_reserve)
//{
//	NET_DB2C_change_can_war* p = (NET_DB2C_change_can_war*)p_msg;
//
//	if(!db_handler_->change_war((LPVOID)p))
//	{
//		return INVALID_VALUE;
//	}
//
//	return E_Success;
//}
// ���°��ɳ�ԱDKP
DWORD db_server::change_DKP(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_change_dkp);
	NET_DB2C_change_dkp * p = ( NET_DB2C_change_dkp * ) p_msg ;  
	if(!db_handler_->change_DKP((LPVOID)p))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ��ȡ���ɲֿ����е���
DWORD db_server::load_guild_ware_all_items(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_load_guild_ware_items);
	NET_DB2C_load_guild_ware_items * p = ( NET_DB2C_load_guild_ware_items * ) p_msg ;  
	// �����㹻����ڴ�
	BYTE* p_buffer_load = new BYTE[TEMP_GUILD_BUFF_SIZE];
	if (!VALID_POINT(p_buffer_load))
	{
		return INVALID_VALUE;
	}
	ZeroMemory(p_buffer_load, sizeof(BYTE) * TEMP_GUILD_BUFF_SIZE);

	NET_DB2S_load_guild_ware_items* p_send = (NET_DB2S_load_guild_ware_items*)p_buffer_load;

	//��ʼ����Ϣͷ
	NET_DB2S_load_guild_ware_items temp;
	p_send->dw_message_id			= temp.dw_message_id;
	p_send->dw_size		= temp.dw_size;
	p_send->dw_guild_id	= p->dw_guild_id;
	p_send->n_item_num		= INVALID_VALUE;

	LPVOID pTemp = (LPVOID)(p_send->by_data);
	db_handler_->load_guild_ware_all_items(pTemp, p->dw_guild_id, p_send->n_item_num);

	if (VALID_VALUE(p_send->n_item_num))
	{
		// ����������Ϣ��С
		p_send->dw_size		= sizeof(NET_DB2S_load_guild_ware_items) + ((BYTE*)pTemp - p_send->by_data - 1) * sizeof(BYTE);
		p_server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);
	}

	// �ͷ�
	SAFE_DELETE_ARRAY(p_buffer_load);

	return E_Success;
}

// ����װ������
DWORD db_server::save_guild_ware_items(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_save_guild_ware);
	NET_DB2C_save_guild_ware * p = ( NET_DB2C_save_guild_ware * ) p_msg ;  
	db_handler_->save_item_attribute(INVALID_VALUE, p->by_data, p->n_item_num, NULL);

	return E_Success;
}
// ��ȡ����������ʩ
DWORD db_server::load_guild_facilities(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_load_facilities_info);
	NET_DB2C_load_facilities_info * p = ( NET_DB2C_load_facilities_info * ) p_msg ;  
	// ��������ڴ�
	BYTE* p_buffer_load = new BYTE[TEMP_GUILD_BUFF_SIZE];
	if (!VALID_POINT(p_buffer_load))
	{
		return INVALID_VALUE;
	}
	ZeroMemory(p_buffer_load, sizeof(BYTE) * TEMP_GUILD_BUFF_SIZE);

	NET_DB2S_load_facilities_info* pSend = (NET_DB2S_load_facilities_info*)p_buffer_load;

	//����Ϣͷ
	NET_DB2S_load_facilities_info temp;
	pSend->dw_message_id			= temp.dw_message_id;
	pSend->dw_size		= temp.dw_size;
	pSend->dw_guild_id	= p->dw_guild_id;

	if (!db_handler_->load_guild_facilities((LPVOID)pSend->s_facilities_info_, pSend->n_info_num, p->dw_guild_id))
	{
		SAFE_DELETE_ARRAY(p_buffer_load);
		return INVALID_VALUE;
	}

	// ���¼�����Ϣ��С
	pSend->dw_size = sizeof(NET_DB2S_load_facilities_info) + (pSend->n_info_num - 1) * sizeof(s_facilities_load);
	p_server_->send_msg(GAMESERVERSESSIONID, pSend, pSend->dw_size);

	// �ͷ��ڴ�
	SAFE_DELETE_ARRAY(p_buffer_load);

	return E_Success;
}

// ����������ʩ
DWORD db_server::insert_guild_facilities(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_create_facilities);
	NET_DB2C_create_facilities * p = ( NET_DB2C_create_facilities * ) p_msg ;  
	if (!db_handler_->insert_guild_facilities(&p->s_facilities_info_))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ���°�����ʩ
DWORD db_server::update_guild_facilities(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_facilities_upgrade);
	NET_DB2C_facilities_upgrade * p = ( NET_DB2C_facilities_upgrade * ) p_msg ;  
	if (!db_handler_->update_guild_facilities(&p->s_facilities_info_))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ɾ��������ʩ
DWORD db_server::delete_guild_facilities(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_remove_all_facilities);
	NET_DB2C_remove_all_facilities * p = ( NET_DB2C_remove_all_facilities * ) p_msg ;  
	if (!db_handler_->delete_guild_facilities(p->dw_guild_id))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// �������ɵ���
DWORD db_server::insert_guild_delate(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_create_guild_delate);
	NET_DB2C_create_guild_delate * p = ( NET_DB2C_create_guild_delate * ) p_msg ;  
	if(!db_handler_->insert_guild_delate(p->dw_guild_id))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ��ȡ���ɵ���
DWORD db_server::load_guild_delate(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_load_guild_delate);
	NET_DB2C_load_guild_delate * p = ( NET_DB2C_load_guild_delate * ) p_msg ;  
	NET_DB2S_load_guild_delate send;

	if(!db_handler_->load_guild_delate(&send.s_guild_delate_load_, p->dw_guild_id))
	{
		return INVALID_VALUE;
	}

	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	return E_Success;
}
// ɾ�����ɵ���
DWORD db_server::delete_guild_delate(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_remove_guild_delate);
	NET_DB2C_remove_guild_delate * p = ( NET_DB2C_remove_guild_delate * ) p_msg ;  
	if(!db_handler_->delete_guild_delate(p->dw_guild_id))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ���°��ɵ���
DWORD db_server::update_guild_delate(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_update_guild_dalate);
	NET_DB2C_update_guild_dalate * p = ( NET_DB2C_update_guild_dalate * ) p_msg ;  
	if(!db_handler_->update_guild_delate(&p->s_guild_delate_base_, p->s_guild_delate_base_.dwGuildID))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ���°��ɵ�������
DWORD db_server::update_guild_delate_content(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_update_guild_delate_content);
	NET_DB2C_update_guild_delate_content * p = ( NET_DB2C_update_guild_delate_content * ) p_msg ;  
	if(!db_handler_->update_guild_delate_content(p->dw_guild_id, p->sz_content))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ��ȡ�������м���
DWORD db_server::load_guild_skill_info(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_load_guild_skill_info);
	NET_DB2C_load_guild_skill_info * p = ( NET_DB2C_load_guild_skill_info * ) p_msg ;  
	// �����㹻��Ŀռ�
	char *p_buffer_load = new char[TEMP_GUILD_BUFF_SIZE];
	ZeroMemory(p_buffer_load, sizeof(char) * TEMP_GUILD_BUFF_SIZE);

	NET_DB2S_load_guild_skill_info* p_send = (NET_DB2S_load_guild_skill_info*)p_buffer_load;
	
	// ��ʼ����Ϣͷ
	NET_DB2S_load_guild_skill_info temp;
	p_send->dw_message_id			= temp.dw_message_id;
	p_send->dw_size		= temp.dw_size;
	p_send->dw_guild_id	= p->dw_guild_id;
	
	if (!db_handler_->load_guild_skill_info(p_send->s_guild_skill_info_, p->dw_guild_id, p_send->n_info_num_))
	{
		SAFE_DELETE_ARRAY(p_buffer_load);
		return INVALID_VALUE;
	}

	// ���¼�����Ϣ��С
	p_send->dw_size = sizeof(NET_DB2S_load_guild_skill_info) + (p_send->n_info_num_ - 1) * sizeof(s_guild_skill_info);

	// ���͸���Ϸ������
	p_server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

	// �ͷ��ڴ�
	SAFE_DELETE_ARRAY(p_buffer_load);

	return E_Success;
}
// �������ɼ���
DWORD db_server::insert_guild_skill_info(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_create_guild_skill);
	NET_DB2C_create_guild_skill * p = ( NET_DB2C_create_guild_skill * ) p_msg ;  
	if (!db_handler_->insert_guild_skill_info(p->dw_guild_id, &p->s_skill_info))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// ���°��ɼ���
DWORD db_server::save_guild_skill_info(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_save_guild_skill);
	NET_DB2C_save_guild_skill * p = ( NET_DB2C_save_guild_skill * ) p_msg ;  
	if (!db_handler_->update_guild_skill_info(p->dw_guild_id, &p->s_skill_info))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ���İ����о�����
DWORD db_server::change_research_guild_skill(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_change_research_skill);
	NET_DB2C_change_research_skill * p = ( NET_DB2C_change_research_skill * ) p_msg ;  
	if (!db_handler_->change_research_guild_skill(p->dw_guild_id, p->dw_new_skill_id))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ɾ�����ɼ���
DWORD db_server::delete_guild_skill_info(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_remove_guild_skill);
	NET_DB2C_remove_guild_skill * p = ( NET_DB2C_remove_guild_skill * ) p_msg ;  
	if (!db_handler_->delete_guild_skill_info(p->dw_guild_id))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ��ȡ�����̻�����
DWORD db_server::load_guild_commerce_info(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_load_commodity);
	NET_DB2C_load_commodity * p = ( NET_DB2C_load_commodity * ) p_msg ;  
	// �����㹻��Ŀռ�
	char *p_buffer_load = new char[TEMP_GUILD_BUFF_SIZE];
	ZeroMemory(p_buffer_load, sizeof(char) * TEMP_GUILD_BUFF_SIZE);

	NET_DB2S_load_commodity* p_send = (NET_DB2S_load_commodity*)p_buffer_load;

	// ����Ϣͷ
	NET_DB2S_load_commodity temp;
	p_send->dw_message_id			= temp.dw_message_id;
	p_send->dw_size		= temp.dw_size;
	p_send->dw_guild_id	= p->dw_guild_id;

	db_handler_->load_guild_commerce_info(p_send->s_commerce_info_, p->dw_guild_id, p_send->n_commodity_num);

	// ���¼�����Ϣ��С
	p_send->dw_size = sizeof(NET_DB2S_load_commodity) + (p_send->n_commodity_num - 1) * sizeof(s_guild_commerce_info);

	// ���͸���Ϸ������
	p_server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

	// �ͷ��ڴ�
	SAFE_DELETE_ARRAY(p_buffer_load);

	return E_Success;
}

// ��ȡ�����̻����а�
DWORD db_server::load_commerce_rank_info(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_load_commerce_rank);
	NET_DB2C_load_commerce_rank * p = ( NET_DB2C_load_commerce_rank * ) p_msg ;  
	// �����㹻��Ŀռ�
	char *p_buffer_load = new char[TEMP_GUILD_BUFF_SIZE];
	ZeroMemory(p_buffer_load, sizeof(char) * TEMP_GUILD_BUFF_SIZE);

	NET_DB2S_load_commerce_rank* p_send = (NET_DB2S_load_commerce_rank*)p_buffer_load;

	// ����Ϣͷ
	NET_DB2S_load_commerce_rank temp;
	p_send->dw_message_id			= temp.dw_message_id;
	p_send->dw_size		= temp.dw_size;
	p_send->dw_guild_id	= p->dw_guild_id;

	if (!db_handler_->load_commerce_rank_info(p_send->s_rank_info, p->dw_guild_id, p_send->n_rank_num))
	{
		SAFE_DELETE_ARRAY(p_buffer_load);
		return INVALID_VALUE;
	}

	// ���¼�����Ϣ��С
	p_send->dw_size = sizeof(NET_DB2S_load_commerce_rank) + (p_send->n_rank_num - 1) * sizeof(tagCommerceRank);

	// ���͸���Ϸ������
	p_server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

	// �ͷ��ڴ�
	SAFE_DELETE_ARRAY(p_buffer_load);

	return E_Success;
}

// ������Ʒ��Ϣ
DWORD db_server::insert_commodity_info(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_create_commodity);
	NET_DB2C_create_commodity * p = ( NET_DB2C_create_commodity * ) p_msg ;  
	if (!db_handler_->insert_commodity_info((LPVOID)p))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ������Ʒ��Ǯ
DWORD db_server::update_commodity_tael(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_change_tael);
	NET_DB2C_change_tael * p = ( NET_DB2C_change_tael * ) p_msg ;  
	if (!db_handler_->update_commodity_money(p->dw_role_id, p->n_tael))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ������Ʒ��Ϣ
DWORD db_server::save_commodity_info(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_save_commodity);
	NET_DB2C_save_commodity * p = ( NET_DB2C_save_commodity * ) p_msg ;  
	if (!db_handler_->save_commodity_info(p->s_good_info, p->dw_role_id))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ɾ����Ʒ��Ϣ
DWORD db_server::delete_commodity_info(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_remove_commodity);
	NET_DB2C_remove_commodity * p = ( NET_DB2C_remove_commodity * ) p_msg ;  
	if (!db_handler_->delete_commodity_info(p->dw_role_id))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ���ð��ɱ���
DWORD db_server::set_guild_commendation(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_set_commendation);
	NET_DB2C_set_commendation * p = ( NET_DB2C_set_commendation * ) p_msg ;  
	if (!db_handler_->set_guild_citation(p->dw_guild_id, p->b_commend))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// �����̻�����
DWORD db_server::update_commerce_rank_info(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_update_commerce_rank);
	NET_DB2C_update_commerce_rank * p = ( NET_DB2C_update_commerce_rank * ) p_msg ;  
	if (!db_handler_->update_commerce_rank_info(p->dw_guild_id, &p->s_rank_info))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
// ɾ���̻�����
DWORD db_server::delete_commerce_rank_info(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p, p_msg, NET_DB2C_remove_commerce_rank);
	NET_DB2C_remove_commerce_rank * p = ( NET_DB2C_remove_commerce_rank * ) p_msg ;  
	if (!db_handler_->delete_commerce_rank_info(p->dw_guild_id, p->dw_role_id))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
