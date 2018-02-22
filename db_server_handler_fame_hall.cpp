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
#include "../common/ServerDefine/role_data_server_define.h"
#include "../common/ServerDefine/famehall_server_define.h"
// 声望排名榜
DWORD db_server::load_reputation_rank(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_get_rep_rank_list * p_recv = ( NET_DB2C_get_rep_rank_list * ) p_msg ;   ;  

	NET_DB2S_get_rep_rank_list send;
	send.dw_error_code = E_Success;
	send.byClanType = p_recv->byClanType;

	int n_total = 0;
	if(!db_handler_->load_rep_rank(p_recv->byClanType, reinterpret_cast<PVOID>(send.repRank), n_total))
	{
		send.dw_error_code = e_database_load_rep_rankdata_failed;
	}
	send.nRecNum = n_total;

	// 重新计算大小
	send.dw_size += (send.nRecNum - MAX_REP_RANK_NUM) * sizeof(tagRepRankData);

	// 发送到游戏服务器
	p_server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
	return 0;
}

// 名人堂初始化
DWORD db_server::fame_hall_initial(DWORD p_msg, DWORD dw_reserve)
{
	PVOID	p_buffer = NULL;
	int		n_total = 0;

	NET_DB2S_get_act_treasure_list		act_treasure_lists;
	p_buffer = reinterpret_cast<PVOID>(&act_treasure_lists);
	db_handler_->load_act_treasures(p_buffer, n_total);
	act_treasure_lists.dw_size += (n_total - MAX_ACT_TREASURE_NUM) * sizeof(tagTreasureData);
	p_server_->send_msg(GAMESERVERSESSIONID, &act_treasure_lists, act_treasure_lists.dw_size);

	NET_DB2S_get_fame_hall_enter_snap	enter_snaps;
	p_buffer = reinterpret_cast<PVOID>(&enter_snaps);
	db_handler_->load_fame_hall_snaps(p_buffer, n_total);
	enter_snaps.dw_size += (n_total - MAX_ENTER_FAMEHALL_SNAP_NUM * ECLT_NUM) * sizeof(tagFameHallEnterSnap);
	p_server_->send_msg(GAMESERVERSESSIONID, &enter_snaps, enter_snaps.dw_size);

	for (int clan_type = ECLT_BEGIN; clan_type < ECLT_END; ++clan_type)
	{	
		NET_DB2S_get_rep_rank_list			rep_rank_list;
		p_buffer = reinterpret_cast<PVOID>(&rep_rank_list.repRank);

		db_handler_->load_rep_rank(clan_type, p_buffer, n_total);
		rep_rank_list.nRecNum = n_total;
		rep_rank_list.byClanType	= clan_type;
		rep_rank_list.dw_size += (rep_rank_list.nRecNum - MAX_REP_RANK_NUM) * sizeof(tagRepRankData);
		p_server_->send_msg(GAMESERVERSESSIONID, &rep_rank_list, rep_rank_list.dw_size);
	}

	NET_DB2S_get_rep_reset_times_tamp	time_stamps;
	p_buffer = reinterpret_cast<PVOID>(&time_stamps);
	db_handler_->load_rep_reset_timestamp(p_buffer);
	p_server_->send_msg(GAMESERVERSESSIONID, &time_stamps, time_stamps.dw_size);
	return 0;
}

// 新增家族珍宝
DWORD db_server::insert_act_treasure(DWORD p_msg, DWORD dw_reserve)
{
	return db_handler_->insert_act_treasure(reinterpret_cast<PVOID>(p_msg));
}

// 新增进入名人堂记录
DWORD db_server::insert_fame_hall_enter_snap(DWORD p_msg, DWORD dw_reserve)
{
	return db_handler_->insert_fame_hall_enter_snap(reinterpret_cast<PVOID>(p_msg));
}

// 更新声望重置时间
DWORD db_server::update_rep_reset_time(DWORD p_msg, DWORD dw_reserve)
{
	M_trans_pointer(p, p_msg, NET_DB2C_update_reprst_time);
	return db_handler_->update_rep_reset_time(p->byClanType, p->dwRepRstTimeStamp);
}