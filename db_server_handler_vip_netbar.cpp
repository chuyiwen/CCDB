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
#include "../common/ServerDefine/vip_netbar_server_define.h"
#include "db_server_handler.h"
#include "db_handler.h"

DWORD db_server::update_vip_net_bar_player( DWORD p_msg, DWORD dw_reserve )
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_update_vnb_player);
	db_handler_->update_vip_net_bar_player(p_recv->dw_account_id, p_recv->dw_login_time);

	return E_Success;
}

DWORD db_server::load_vip_net_bar( DWORD p_msg, DWORD dw_reserve )
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_get_vnb_data);

	char buffer[1024 * 10] = {0};
	NET_DB2S_get_vnb_data* p_send = (NET_DB2S_get_vnb_data*)buffer;
	NET_DB2S_get_vnb_data send;
	*p_send = send;

	db_handler_->load_vip_net_bar(&p_send->players, p_recv->dw_date);
	int n_total_number = p_send->players.n_his_players + p_send->players.n_todays_players;
	p_send->dw_size = n_total_number == 0 ? sizeof(NET_DB2S_get_vnb_data) : sizeof(NET_DB2S_get_vnb_data)  - 1 + n_total_number * sizeof(DWORD);
	p_server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

	return E_Success;
}
