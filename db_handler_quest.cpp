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
 *	@file		db_handler_quest
 *	@author		mwh
 *	@date		2010/12/13	initial
 *	@version	0.0.1.0
 *	@brief		悬赏任务
*/

#include "StdAfx.h"
#include "../../common/WorldDefine/container_define.h"
#include "../common/ServerDefine/quest_server_define.h"
#include "../../common/WorldDefine/guerdon_quest_define.h"
#include "../common/ServerDefine/guerdon_quest_protocol.h"
#include "db_handler.h"

// 加载所有
BOOL db_handler::load_all_guerdonquest(OUT LPVOID p_buffer, OUT INT& nNumber)
{
	return common_select( p_buffer,&nNumber, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_guerdonquest), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_all_guerdonquest));
}

VOID db_handler::makesql_load_all_guerdonquest(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("guerdon_quest", "serial,sender,reciver,quest_id,guild_fix,state,yuanbao,end_time");
}
VOID db_handler::procres_load_all_guerdonquest(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	*p_number = p_result->get_row_count();
	guerdon_quest* pData = (guerdon_quest*)p_buffer;
	for(INT n = 0; n < *p_number; ++n)
	{
		pData->n64Serial = (*p_result)[0].get_long( );
		pData->dwSender = (*p_result)[1].get_dword();
		pData->dwReciver = (*p_result)[2].get_dword();
		pData->u16QuestID = (UINT16)(*p_result)[3].get_int( );
		pData->bGuildFix = (BOOL)(*p_result)[4].get_byte( );
		pData->eState = (EGuerdonQuestState)(*p_result)[5].get_byte( );
		pData->u16YuanBao = (UINT16)(*p_result)[6].get_int( );
		pData->dwEndTime = (*p_result)[7].get_dword();
		++pData; p_result->next_row();
	}
	p_buffer = pData;
}

// 加载奖励
BOOL db_handler::load_all_gdreward(OUT LPVOID& p_buffer, OUT INT& nNumber)
{
	return common_select( p_buffer,&nNumber, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_guerdonquest_reward), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_items));
}
VOID db_handler::makesql_load_all_guerdonquest_reward(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->write_string("select ");
	init_item_load_field(p_stream);
	p_stream->write_string(" from item left join equip using(serial)");
	p_stream->where_item();
	p_stream->write_string(" container_type_id=") << EICT_GDQuest;
}
// 新增
VOID db_handler::insert_new_guerdonquest(PVOID p_buffer)
{
	common_update(INVALID_VALUE, p_buffer, 1, sizeof(guerdon_quest),
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_new_guerdonquest));
}

VOID db_handler::makesql_insert_new_guerdonquest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	NET_DB2S_AddNewGuerdonQuest* pProtocol = (NET_DB2S_AddNewGuerdonQuest*)p_buffer;
	p_stream->clear();
	p_stream->insert_item("guerdon_quest");
	p_stream->write_string("serial=") << pProtocol->stData.n64Serial;
	p_stream->write_string(",sender=") << pProtocol->stData.dwSender;
	p_stream->write_string(",reciver=") << pProtocol->stData.dwReciver;
	p_stream->write_string(",quest_id=") << pProtocol->stData.u16QuestID;
	p_stream->write_string(",guild_fix=") << pProtocol->stData.bGuildFix;
	p_stream->write_string(",state=") << pProtocol->stData.eState;
	p_stream->write_string(",yuanbao=") << pProtocol->stData.u16YuanBao;
	p_stream->write_string(",end_time=") << pProtocol->stData.dwEndTime;
}

// 更新
VOID db_handler::update_guerdonquest(PVOID p_buffer)
{
	common_update(INVALID_VALUE, p_buffer, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_update_guerdonquest));
}

VOID db_handler::makesql_update_guerdonquest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	NET_DB2S_UpdateGuerdonQuest* pProtocol = (NET_DB2S_UpdateGuerdonQuest*)p_buffer;
	p_stream->clear();
	p_stream->update_item("guerdon_quest");
	p_stream->write_string("reciver=") << pProtocol->dwReciver;
	p_stream->write_string(",state=") << pProtocol->eState;
	p_stream->where_item();
	p_stream->write_string("serial=") << pProtocol->n64Serial;
}

// 删除
VOID db_handler::delete_guerdonquest(PVOID p_buffer)
{
	common_update(INVALID_VALUE, p_buffer, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_guerdonquest));
}
VOID db_handler::makesql_delete_guerdonquest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	NET_DB2S_DelGuerdonQuest* pProtocol = (NET_DB2S_DelGuerdonQuest*)p_buffer;
	p_stream->clear();
	p_stream->delete_item("guerdon_quest");
	p_stream->where_item();
	p_stream->write_string("serial=") << pProtocol->n64Serial;
}

// 更新
VOID db_handler::update_questcompletedata(PVOID p_buffer)
{
	common_update(INVALID_VALUE, p_buffer, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_update_questcompletedata));
}

VOID db_handler::makesql_update_questcompletedata(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	NET_DB2S_SaveCreatureKillGD* pProtocol = (NET_DB2S_SaveCreatureKillGD*)p_buffer;
	p_stream->clear();
	p_stream->update_item("quest");
	p_stream->write_string("monster0_num=") << pProtocol->n16MonsterNum[0];
	p_stream->write_string(",monster1_num=") << pProtocol->n16MonsterNum[1];
	p_stream->write_string(",monster2_num=") << pProtocol->n16MonsterNum[2];
	p_stream->write_string(",monster3_num=") << pProtocol->n16MonsterNum[3];
	p_stream->write_string(",monster4_num=") << pProtocol->n16MonsterNum[4];
	p_stream->write_string(",monster5_num=") << pProtocol->n16MonsterNum[5];
	p_stream->where_item();
	p_stream->write_string("role_id=") << pProtocol->dwRoleID;
	p_stream->write_string(" AND quest_id=") << pProtocol->u16QuestID;
}

// 更新
VOID db_handler::update_quest_flag(PVOID p_buffer)
{
	common_update(INVALID_VALUE, p_buffer, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_update_quest_flag));
}

// 更新任务标志
VOID db_handler::makesql_update_quest_flag(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	NET_DB2S_UpdateRoleQuestFlag* pProtocol = (NET_DB2S_UpdateRoleQuestFlag*)p_buffer;
	p_stream->clear();
	p_stream->update_item("quest");
	p_stream->write_string("quest_flag = quest_flag & ") << pProtocol->dwFlagEx;
	p_stream->write_string(",quest_flag = quest_flag | ") << pProtocol->dwFlag;
	p_stream->where_item();
	p_stream->write_string("role_id=") << pProtocol->dwRoleID;
	p_stream->write_string(" AND quest_id=") << pProtocol->u16QuestID;
}

// 更新奖励物品
VOID db_handler::insert_quest_rewarditem(PVOID p_buffer)
{
	common_update(INVALID_VALUE, p_buffer, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_quest_rewarditem));
}

// 更新奖励物品
VOID db_handler::makesql_insert_quest_rewarditem(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	NET_DB2S_InsertQuestRewardItem* pProtocol = (NET_DB2S_InsertQuestRewardItem*)p_buffer;
	p_stream->clear();
	p_stream->update_item("item");
	p_stream->write_string("owner_id=") << pProtocol->dwSender;
	p_stream->write_string(",account_id=") << pProtocol->u16QuestID;
	p_stream->write_string(",container_type_id=") << pProtocol->byContainerType;
	p_stream->where_item();
	p_stream->write_string("owner_id=") << pProtocol->dwSender;
	p_stream->write_string(" AND serial=") << pProtocol->n64Serial;
}

// 删除奖励物品
VOID db_handler::delete_quest_rewarditem(PVOID p_buffer)
{
	common_update(INVALID_VALUE, p_buffer, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_quest_rewarditem));
}

// 删除奖励物品
VOID db_handler::makesql_delete_quest_rewarditem(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	NET_DB2S_DeleteQuestRewardItem* pProtocol = (NET_DB2S_DeleteQuestRewardItem*)p_buffer;
	p_stream->clear();
	p_stream->delete_item("item");
	p_stream->where_item();
	p_stream->write_string("serial=") << pProtocol->n64Serial;
	p_stream->write_string(" AND owner_id=") << pProtocol->dwOwner;
	p_stream->write_string(" AND account_id=") << pProtocol->dwAccountID;
	p_stream->write_string(" AND container_type_id=") << pProtocol->byContainerType;
}
