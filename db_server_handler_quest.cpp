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
 *	@file		fp_Quest
 *	@author		mwh
 *	@date		2010/11/03	initial
 *	@version	0.0.1.0
 *	@brief		任务
*/

#include "StdAfx.h"
#include "../common/ServerDefine/quest_server_define.h"
#include "../common/ServerDefine/guerdon_quest_protocol.h"
#include "../common/ServerDefine/TeamShareQuestServerDefine.h"
#include "db_server_handler.h"
#include "db_handler.h"
// 新增任务
DWORD db_server::accept_quest(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p_recv, p_msg, NET_DB2C_accept_quest);
	NET_DB2C_accept_quest * p_recv = ( NET_DB2C_accept_quest * ) p_msg ;  
	if(!db_handler_->insert_quest(p_recv->dw_role_id, &p_recv->accept_quest_))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 新增完成任务
DWORD db_server::complete_quest(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p_recv, p_msg, NET_DB2C_complete_quest);
	NET_DB2C_complete_quest * p_recv = ( NET_DB2C_complete_quest * ) p_msg ;  
	if(!db_handler_->save_quest_done(p_recv->dw_role_id, &p_recv->quest_done_, 1)
		|| !db_handler_->delete_quest(p_recv->dw_role_id, p_recv->quest_done_.u16QuestID))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}

// 删除任务
DWORD db_server::delete_quest(DWORD p_msg, DWORD dw_reserve)
{
	//GET_MESSAGE(p_recv, p_msg, NET_DB2C_discard_quest);
	NET_DB2C_discard_quest * p_recv = ( NET_DB2C_discard_quest * ) p_msg ;  
	if(!db_handler_->delete_quest(p_recv->dw_role_id, p_recv->u16_quest_id_))
	{
		return INVALID_VALUE;
	}

	return E_Success;
}
//删除每日任务
DWORD db_server::delete_all_dayquest(DWORD p_msg,DWORD dw_reserve)
{
	NET_DB2C_clear_day_quest* p_recv = (NET_DB2C_clear_day_quest*)p_msg;
	db_handler_->delte_all_dayquest(1);
	db_handler_->delte_all_dayquest_done(1);
	return E_Success;
}
// 所有悬赏任务
DWORD db_server::load_all_guerdonquest(DWORD p_msg, DWORD dw_reserve)
{
	BYTE* p_buffer = new BYTE[LOADALLGDQUESTBUFFER_MAX];
	if( !VALID_POINT( p_buffer )) return INVALID_VALUE;
	ZeroMemory( p_buffer, LOADALLGDQUESTBUFFER_MAX * sizeof( BYTE ) );

	NET_DB2C_LoadAllGuerdonQuest* p_send = (NET_DB2C_LoadAllGuerdonQuest*)p_buffer;
	NET_DB2C_LoadAllGuerdonQuest tmp;
	p_send->nNumber = 0;
	p_send->dw_message_id = tmp.dw_message_id;
	p_send->dw_size = tmp.dw_size - sizeof(guerdon_quest);

	db_handler_->load_all_guerdonquest(p_send->stData, p_send->nNumber);
	if(VALID_POINT(p_send->nNumber))
		p_send->dw_size += sizeof(guerdon_quest)*p_send->nNumber;

	p_server_->send_msg( GAMESERVERSESSIONID, p_send, p_send->dw_size );

	delete []p_buffer;

	return E_Success;
}
// 所有奖励
DWORD db_server::load_all_guerdonquest_reward(DWORD p_msg, DWORD dw_reserve)
{
	BYTE* p_buffer = new BYTE[LOADALLGDQUESTREWARDBUFFER_MAX];
	ZeroMemory(p_buffer, sizeof(BYTE)*LOADALLGDQUESTREWARDBUFFER_MAX);
	if(!VALID_POINT(p_buffer)) return 0;

	NET_DB2C_LoadAllGuerdonReward* p_send = (NET_DB2C_LoadAllGuerdonReward*)p_buffer;
	NET_DB2C_LoadAllGuerdonReward temp;
	p_send->nNumber = 0;
	p_send->dw_message_id = temp.dw_message_id;
	p_send->dw_size = temp.dw_size - sizeof(BYTE);

	VOID* pData = p_send->byData;
	db_handler_->load_all_gdreward(pData, p_send->nNumber);
	if(p_send->nNumber)
	{
		p_send->dw_size = (char*)pData - (char*)p_send->byData;
	}
	p_server_->send_msg( GAMESERVERSESSIONID, p_send, p_send->dw_size);

	delete []p_buffer;

	return E_Success;
}
// 新增悬赏
DWORD db_server::insert_new_guerdonquest(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_AddNewGuerdonQuest* pProtocol = (NET_DB2S_AddNewGuerdonQuest*)p_msg;
	db_handler_->insert_new_guerdonquest(pProtocol);
	return E_Success;
}

// 更新
DWORD db_server::update_guerdonquest(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_UpdateGuerdonQuest* pProtocol = (NET_DB2S_UpdateGuerdonQuest*)p_msg;
	db_handler_->update_guerdonquest(pProtocol);
	return E_Success;
}
// 删除
DWORD db_server::delete_guerdonquest(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_DelGuerdonQuest* pProtocol = (NET_DB2S_DelGuerdonQuest*)p_msg;
	db_handler_->delete_guerdonquest(pProtocol);
	return E_Success;
}

// 完成
DWORD db_server::update_questcompletedata(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_SaveCreatureKillGD* pProtocol = (NET_DB2S_SaveCreatureKillGD*)p_msg;
	db_handler_->update_questcompletedata(pProtocol);
	return E_Success;
}

// 更新
DWORD db_server::update_questflag(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2S_UpdateRoleQuestFlag* pProtocol = (NET_DB2S_UpdateRoleQuestFlag*)p_msg;
	db_handler_->update_quest_flag(pProtocol);
	return E_Success;
}

DWORD db_server::insert_quest_rewarditem(DWORD p_msg, DWORD dw_reserve)
{	
	NET_DB2S_InsertQuestRewardItem* pProtocol = (NET_DB2S_InsertQuestRewardItem*)p_msg;
	db_handler_->insert_quest_rewarditem(pProtocol);
	return E_Success;
}
DWORD db_server::delete_quest_rewarditem(DWORD p_msg, DWORD dw_reserve)
{	
	NET_DB2S_DeleteQuestRewardItem* pProtocol = (NET_DB2S_DeleteQuestRewardItem*)p_msg;
	db_handler_->delete_quest_rewarditem(pProtocol);
	return E_Success;
}

DWORD db_server::remove_all_teamshare_quest(DWORD p_msg, DWORD dw_reserve)
{
	NET_S2DB_DelAllTeamShareQuest* pProtocol = (NET_S2DB_DelAllTeamShareQuest*)p_msg;
	db_handler_->delete_all_teamshare_quest(pProtocol->nNewFlag);
	return E_Success;
}