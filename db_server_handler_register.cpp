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
 *	@file		db_server_handler_register
 *	@author		mwh
 *	@date		2011/02/12	initial
 *	@version	0.0.1.0
 *	@brief		网络消息处理注册
*/


#include "StdAfx.h"

#include "db_server_handler.h"

// 注册包处理
VOID db_server::register_command()
{
	REGISTER_NET_MSG("NET_retification",		db_server,				game_server_init_ok,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_world_shutdown",	db_server,		game_server_showdown,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_world_init_ok",	db_server,				game_server_ok,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_all_role_info",	db_server,				load_all_role_simple,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_sim_role",		db_server,			load_one_role_simple,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_role",			db_server,			load_one_role_full_data,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_serial_reward", db_server,			load_serial_reward,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_create_role",			db_server,			create_role,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_role",			db_server,			delete_role,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_cancel_role_del_guard_time",			db_server,			role_guard_cancel_time,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_role_guard_time_set",			db_server,			role_guard_set_time,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_resume_role",			db_server,			active_role,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_save_role",			db_server,			save_one_role,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_role_reward_data",			db_server,			update_one_role_reward_data,				_T(""));//
	REGISTER_NET_MSG("NET_DB2C_change_role_name",		db_server,			change_role_name,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_role_guild",	db_server,			change_role_guild,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_role_leave_guild_time",	db_server,	update_role_leave_guild_time,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_role_VIP_Info",	db_server,	update_role_vip_info,	_T(""));//gx add
	REGISTER_NET_MSG("NET_DB2C_clear_role_dress_mdid",		db_server,		change_role_dress_model_id,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_role_map_process",	db_server,		delete_map_process,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_role_map_limit",		db_server,		update_role_map_limit,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_remove_role_map_limit",		db_server,		delete_role_map_limit,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_insert_role_map_limit",		db_server,		insert_role_map_limit,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_clean_role_hang_num",		db_server,		clean_role_hang_number,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_clean_role_yuanbao_exchange_num",	db_server,	clean_role_exchange_num,	_T(""));
	REGISTER_NET_MSG("NET_C2DB_clean_role_day",			db_server,		clean_role_day_clear,	_T(""));
	REGISTER_NET_MSG("NET_C2DB_reset_role_value_at_0",			db_server,		reset_role_at_0,	_T(""));
	REGISTER_NET_MSG("NET_C2DB_CleanRolePaiMaiNum",		db_server,		clean_role_paimai_number,	_T(""));
	REGISTER_NET_MSG("NET_C2DB_clean_role_spouse_id",		db_server,	clean_role_spouse_id,	_T(""));
	REGISTER_NET_MSG("NET_C2DB_clear_role_VIP_info",		db_server,	clean_role_vip_info,	_T(""));//gx add
	REGISTER_NET_MSG("NET_C2DB_clean_role_sign_data",	db_server,		clean_role_sign_data,		_T(""));
	REGISTER_NET_MSG("NET_C2DB_clean_role_bank_num",	db_server,		clean_role_bank_number,		_T(""));
	REGISTER_NET_MSG("NET_C2DB_clean_role_exploits_num", db_server,		clean_role_exploits_num,	_T(""));
	REGISTER_NET_MSG("NET_C2DB_clean_role_active_data",	db_server,		clean_role_active_data,		_T(""));
	REGISTER_NET_MSG("NET_C2DB_update_day_1v1_score",	db_server,		update_day_1v1_score,		_T(""));
	REGISTER_NET_MSG("NET_C2DB_update_week_1v1_score",	db_server,		update_week_1v1_score,		_T(""));
	REGISTER_NET_MSG("NET_C2DB_update_1v1_award",		db_server,		update_1v1_award,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_insert_pet_sns",			db_server,		insert_pet_sns_info,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_pet_sns",			db_server,		delete_pet_sns_info,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_pet_sns",			db_server,		load_pet_sns_info,		_T(""));
	REGISTER_NET_MSG("NET_C2DB_clean_role_luck",		db_server,		clean_role_luck,		_T(""));
	REGISTER_NET_MSG("NET_C2DB_update_shihun",			db_server,		update_shihun,			_T(""));
	REGISTER_NET_MSG("NET_C2DB_UpdateCircleRefreshNumberAndQuestNum",		db_server,	inc_circle_refreshnumber_and_questnum,		_T(""));
	REGISTER_NET_MSG("NET_C2DB_clean_role_destroy_equip_count", db_server, clear_destory_equip_count, _T(""));
	REGISTER_NET_MSG("NET_DB2C_bag_money_update",		db_server,		bag_meony_update,		_T(""));

	// 帐号属性
	REGISTER_NET_MSG("NET_DB2C_safe_code_set",		db_server,			set_safe_code,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_safe_code_reset",		db_server,			reset_safe_code,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_safe_code_reset_cancel",	db_server,		cancel_safe_code_reset,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_ware_size_update",			db_server,		update_ware_size,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_ware_money_update",		db_server,		update_ware_money,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_baibao_yuanbao_update",			db_server,		update_baibao_yuanbao,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_exchange_volume_update",		db_server,		update_exchange_volume,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_receive",		db_server,		update_account_receive,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_receive_ex",	db_server,		update_account_receive_ex,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_web_recieve", db_server,		update_web_recieve,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_serial_reward",	db_server,	delete_serial_reward,		_T(""));


	REGISTER_NET_MSG("NET_DB2C_change_bag_password",			db_server,		change_bag_password,			_T(""));

	// 好友
	REGISTER_NET_MSG("NET_DB2C_insert_friend",		db_server,			insert_friend,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_friend",		db_server,			update_friend,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_friend",		db_server,			delelte_friend,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_all_friend",		db_server,			delete_all_friend_relation,		_T(""));

	REGISTER_NET_MSG("NET_DB2C_insert_friend_value",		db_server,			insert_friendship,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_friend_value",		db_server,			update_friendship,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_friend",		db_server,			delete_friendship,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_all_friend_value",	db_server,			delete_all_friendship_relation,		_T(""));

	// 黑名单
	REGISTER_NET_MSG("NET_DB2C_insert_black",		db_server,			insert_black,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_black",		db_server,			delete_black,			_T(""));

	REGISTER_NET_MSG("NET_DB2C_insert_enemy",		db_server,			insert_enemy,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_enemy",		db_server,			delete_enemy,			_T(""));

	REGISTER_NET_MSG("NET_DB2C_get_item_info",		db_server,			get_world_max_min_serial,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_item_need_log",	db_server,			load_item_need_log,		_T(""));

	// 任务
	REGISTER_NET_MSG("NET_DB2C_accept_quest",		db_server,			accept_quest,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_complete_quest",		db_server,			complete_quest,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_discard_quest",		db_server,			delete_quest,			_T(""));
	REGISTER_NET_MSG("NET_S2DB_DelAllTeamShareQuest",		db_server,			remove_all_teamshare_quest,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_clear_day_quest",		db_server,			delete_all_dayquest,			_T(""));//gx add

	// 技能
	REGISTER_NET_MSG("NET_DB2C_add_skill",			db_server,			insert_skill,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_remove_skill",		db_server,			delete_skill,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_skill",		db_server,			update_skill,			_T(""));

	// 物品装备
	REGISTER_NET_MSG("NET_DB2C_new_item",			db_server,			insert_item,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_new_equip",			db_server,			insert_equip,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_item",			db_server,			delete_item,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_equip",			db_server,			delete_equip,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_equip_att",		db_server,			save_equip_special,		_T(""));

	// item_baibao表中物品
	REGISTER_NET_MSG("NET_DB2C_load_baibao",			db_server,			load_item_baibao,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_baibao_yuanbao",	db_server,			load_baibao_yuanbao,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_new_baibao_item",		db_server,			insert_baibao_item,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_new_baibao_equip",		db_server,			insert_baibao_equip,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_baobao_item",		db_server,			delete_baibao_item,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_baibao_equip",		db_server,			delete_baibao_equip,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_yuanbao_recharge",	db_server,		update_yuanbao_recharge,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_total_recharge",		db_server,		update_total_recharge,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_web_receive",		db_server,			load_web_receive,				_T(""));

	// 百宝袋Log信息
	REGISTER_NET_MSG("NET_DB2C_add_baibao_log",		db_server,			insert_baibao_log_info,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_baobao_log",		db_server,			load_baibao_log_info,		_T(""));

	// 称号
	REGISTER_NET_MSG("NET_DB2C_title_insert",		db_server,			insert_title,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_title_delete",		db_server,			delete_title,			_T(""));

	// 角色名贴
	REGISTER_NET_MSG("NET_DB2C_vcard_change",			db_server,			update_visiting_card,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_get_off_line_vcard",	db_server,			load_visiting_card,				_T(""));
	

	// 名人堂
	REGISTER_NET_MSG("NET_DB2C_fame_hall_init_start",	db_server,			fame_hall_initial,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_get_rep_rank_list",		db_server,			load_reputation_rank,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_insert_fame_hall_snap",	db_server,			insert_fame_hall_enter_snap, _T(""));
	REGISTER_NET_MSG("NET_DB2C_insert_act_clan_treasure",	db_server,		insert_act_treasure,_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_reprst_time",		db_server,		update_rep_reset_time,	_T(""));

	// 元宝交易
	REGISTER_NET_MSG("NET_DB2C_load_all_yuanbao_account",	db_server,			load_all_yuanbao_account,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_all_order",		db_server,			load_all_yuanbao_order,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_insert_account",		db_server,			insert_yuanbao_account,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_syn_account_yanbao",		db_server,			update_account_yuanbao,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_syn_account_silver",	db_server,			update_account_silver, _T(""));
	REGISTER_NET_MSG("NET_DB2C_syn_submit_sell",		db_server,			update_submit_sell,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_syn_submit_buy",		db_server,			update_submit_buy,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_insert_order",		db_server,			insert_one_order,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_complete_order",		db_server,			complete_one_order,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_refresh_order",		db_server,			refresh_one_order,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_get_role_yuanbao_order",		db_server,			load_one_role_all_yuanbao_order,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_yuanbao_order",		db_server,			delete_one_order,		_T(""));

	// 帮派属性
	REGISTER_NET_MSG("NET_DB2C_load_all_guild",		db_server,			load_all_guild,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_guild_init_ok",		db_server,			check_guild_init_ok,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_create_guild",		db_server,			create_guild,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_dismiss_guild",		db_server,			dismiss_guild,		_T(""));
	REGISTER_NET_MSG("ET_DB2C_reset_signUpAttact",  db_server,			resetSignUpAttack,  _T(""));
	REGISTER_NET_MSG("NET_DB2C_change_guild_tenet",	db_server,			change_guild_tenet,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_guild_symbol",	db_server,			change_guild_symbol,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_save_all_guild",		db_server,			save_guild_all,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_save_guild_att",		db_server,			save_guild_attribute,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_guild_pos_name",	db_server,			change_guild_pos_name,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_guild_pos_power",	db_server,		update_guild_pos_power,	_T(""));
	REGISTER_NET_MSG("NET_C2DB_load_guild_plant_data",	db_server,		load_guild_plant_data,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_save_plant",		db_server,				save_guild_plant,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_save_all_guild_plant",		db_server,	save_guild_all_plant,		_T(""));
	REGISTER_NET_MSG("NET_C2DB_load_sbk",			db_server,			load_sbk_guild,		_T(""));
	REGISTER_NET_MSG("NET_C2DB_save_sbk",		db_server,				save_sbk_guild,		_T(""));

	// 帮会pvp
	REGISTER_NET_MSG("NET_DB2C_insert_pvp_data",		db_server,			insert_guild_pvp_data,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_pvp_data",		db_server,			load_guild_pvp_data,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_up_pvp_data",			db_server,			update_pvp_data,			_T(""));

	// 帮会招募
	REGISTER_NET_MSG("NET_C2DB_load_guild_recruit",	db_server,			load_guild_recruit,				_T(""));
	REGISTER_NET_MSG("NET_C2DB_insert_guild_recruit",	db_server,		insert_guild_recruit,			_T(""));
	REGISTER_NET_MSG("NET_C2DB_delete_guild_recruit", db_server,		delete_guild_recruit,			_T(""));
	REGISTER_NET_MSG("NET_C2DB_clean_guild_recruit",	db_server,		clean_guild_recruit,			_T(""));

	// 帮会boss击杀
	REGISTER_NET_MSG("NET_C2DB_load_guild_skill_boss",	db_server,		load_guild_skill_boss,			_T(""));
	REGISTER_NET_MSG("NET_C2DB_insert_guild_skill_boss", db_server,		insert_guild_kill_boss,			_T(""));

	// 帮派成员属性
	REGISTER_NET_MSG("NET_DB2C_load_all_guild_member",		db_server,		load_all_guild_member,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_join_guild",			db_server,			join_guild,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_leave_guild",			db_server,			leave_guild,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_guild_pos",		db_server,			change_guild_member_pos,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_guild_ware_pri",		db_server,			guild_ware_privilege,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_contrib",		db_server,			change_member_contribution,_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_ballot",		db_server,			change_ballot,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_dkp",			db_server,			change_DKP,				_T(""));
	REGISTER_NET_MSG("NET_C2DB_load_guild_war_history",			db_server,				load_guild_war_history,				_T(""));	
	REGISTER_NET_MSG("NET_C2DB_insert_guild_war_history",			db_server,			insert_guild_war_history,				_T(""));	
	REGISTER_NET_MSG("NET_C2DB_delete_guild_war_history",			db_server,			delete_guild_war_history,				_T(""));	

	//REGISTER_NET_MSG("NET_DB2C_change_can_war",		db_server,			change_war,			_T(""));

	// 帮派仓库
	REGISTER_NET_MSG("NET_DB2C_load_guild_ware_items",		db_server,		load_guild_ware_all_items,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_save_guild_ware",			db_server,		save_guild_ware_items,		_T(""));

	// 帮派设施
	REGISTER_NET_MSG("NET_DB2C_load_facilities_info",		db_server,		load_guild_facilities,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_create_facilities",		db_server,		insert_guild_facilities,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_facilities_upgrade",		db_server,		update_guild_facilities,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_remove_all_facilities",	db_server,		delete_guild_facilities,		_T(""));

	// 帮派技能
	REGISTER_NET_MSG("NET_DB2C_load_guild_skill_info",		db_server,		load_guild_skill_info,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_create_guild_skill",		db_server,		insert_guild_skill_info,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_save_guild_skill",			db_server,		save_guild_skill_info,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_research_skill",	db_server,		change_research_guild_skill,_T(""));
	REGISTER_NET_MSG("NET_DB2C_remove_guild_skill",		db_server,		delete_guild_skill_info,	_T(""));

	// 帮派弹劾
	REGISTER_NET_MSG("NET_DB2C_create_guild_delate",		db_server,		insert_guild_delate,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_guild_delate",		db_server,		load_guild_delate,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_remove_guild_delate",		db_server,		delete_guild_delate,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_guild_dalate",		db_server,		update_guild_delate,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_guild_delate_content",	db_server,	update_guild_delate_content, _T(""));

	// 帮派跑商
	REGISTER_NET_MSG("NET_DB2C_load_commodity",		db_server,			load_guild_commerce_info,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_commerce_rank",	db_server,			load_commerce_rank_info,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_create_commodity",	db_server,			insert_commodity_info,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_tael",			db_server,			update_commodity_tael,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_save_commodity",		db_server,			save_commodity_info,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_remove_commodity",	db_server,			delete_commodity_info,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_set_commendation",	db_server,			set_guild_commendation,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_commerce_rank",	db_server,			update_commerce_rank_info,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_remove_commerce_rank",	db_server,			delete_commerce_rank_info,	_T(""));

	// 宠物
	REGISTER_NET_MSG("NET_DB2C_create_pet_soul",		db_server,			create_new_pet,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_pet_soul",			db_server,			delete_pet,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_pet_soul",		db_server,			update_pet_attribute,			_T(""));

	REGISTER_NET_MSG("NET_DB2C_insert_pet_skill",		db_server,			insert_pet_skill,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_pet_skill",		db_server,			delete_pet_skill,			_T(""));

	// 商城
	REGISTER_NET_MSG("NET_DB2C_get_all_gp_info",		db_server,			load_all_group_purchase_info,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_add_gp_info",			db_server,			add_group_purchase_info,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_remove_gp_info",		db_server,			delete_group_purchase_info,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_remove_guild_gp_info",	db_server,			delete_guild_group_purchase_info,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_gp_info",		db_server,			update_group_purchase_info,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_gp_time",		db_server,			update_group_purchase_time,		_T(""));

	// VIP摊位
	REGISTER_NET_MSG("NET_DB2C_get_all_vip_stall_info",		db_server,		get_all_vip_stall_info,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_apply_vip_stall",		db_server,			apply_vip_stall,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_vip_stall",		db_server,			update_vip_stall_time,		_T(""));

	REGISTER_NET_MSG("NET_DB2C_update_vnb_player",	db_server,			update_vip_net_bar_player,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_get_vnb_data",			db_server,			load_vip_net_bar,			_T(""));

	//	开启宝箱
	REGISTER_NET_MSG("NET_DB2C_update_treasure_sum",	db_server,			update_treasure_count, _T(""));

	// 固定活动
	REGISTER_NET_MSG("NET_DB2C_load_activity_data",	db_server,			load_activities,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_save_activity_data",	db_server,			save_activity,	_T(""));

	// 日志
	REGISTER_NET_MSG("NET_DB2C_log_silver",		db_server,			insert_money_log,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_yuanbao",	db_server,			insert_yuanbao_log,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_exvolume",	db_server,			insert_exvolume_log,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_exploits",	db_server,			insert_log_exploits,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_time_stat",	db_server,			insert_time_stat_log,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_item",		db_server,			insert_item_log,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_mall_sell",	db_server,			insert_mall_sell_log,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_mall_sell_pack",	db_server,			insert_mall_sell_pack_log,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_item_times",	db_server,			insert_item_times_log,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_fund",			db_server,			insert_fund_log,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_material",		db_server,			insert_material_log,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_reputation",		db_server,			insert_reputation_log,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_bank",			db_server,			insert_bank_log,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_paimai",			db_server,			insert_paimai_log,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_leave_practice", db_server,			insert_leave_pracitice_log, _T(""));
	REGISTER_NET_MSG("NET_DB2C_log_serial_reward",	db_server,			insert_serial_reward_log,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_log_kick",			db_server,			insert_kick_log,		_T(""));

	// 在线人数
	REGISTER_NET_MSG("NET_DB2C_update_online_num",	db_server,			update_online_num,		_T(""));

	// 离线留言
	REGISTER_NET_MSG("NET_DB2C_load_left_message",		db_server,			load_leave_msg,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_insert_left_message",		db_server,			insert_leave_msg,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_clear_left_message",		db_server,			clear_leave_msg,		_T(""));

	// 保存玩家经验和点
	REGISTER_NET_MSG("NET_DB2C_update_role_att",		db_server,			update_role_attribute,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_role_att_point",		db_server,		update_role_attribute_point,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_role_talent_point",	db_server,		update_role_talent_point,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_clean_role_leave_exp",		db_server,		clear_role_leave_exp,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_clean_role_leave_brother",	db_server,		clear_role_leave_brotherhood,	_T(""));

	// 压测专用
	REGISTER_NET_MSG("NET_DB2C_test_load_role",		db_server,			force_test_load_role,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_test_load_role_item",	db_server,			force_test_load_item,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_test_save_item",		db_server,			force_test_save_item,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_test_save_equip",		db_server,			force_test_save_equip,		_T(""));

	// 邮件相关
	REGISTER_NET_MSG("NET_DB2C_get_mail_max_serial",	db_server,			get_mail_max_serial,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_save_mail_item",		db_server,			update_mail_item,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_save_mail",			db_server,			save_mail,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_all_mail",		db_server,			load_all_mail,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_mail_content",	db_server,			load_mail_content,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_mail_item",		db_server,			load_mail_item,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_mail",			db_server,			update_mail,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_delete_mail",			db_server,			delete_mail,				_T(""));
	REGISTER_NET_MSG("NET_DB2C_change_role_money",	db_server,			change_role_money,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_role_send_num",	db_server,			update_role_send_mail_count,	_T(""));

	// 拍卖相关
	REGISTER_NET_MSG("NET_S2DB_load_paimai_max_id",	db_server,			get_paimai_max_id,		_T(""));
	REGISTER_NET_MSG("NET_S2DB_update_paimai_item",	db_server,			update_paimai_item,		_T(""));
	REGISTER_NET_MSG("NET_S2DB_insert_paimai",		db_server,			insert_paimai,			_T(""));
	REGISTER_NET_MSG("NET_S2DB_load_all_paimai",	db_server,			load_all_paimai_info,	_T(""));
	REGISTER_NET_MSG("NET_S2DB_load_paimai_item",	db_server,			load_paimai_item,		_T(""));
	REGISTER_NET_MSG("NET_S2DB_delete_paimai",		db_server,			delete_paimai,			_T(""));
	REGISTER_NET_MSG("NET_S2DB_update_paimai",		db_server,			update_paimai,			_T(""));

	// 自动拍卖
	REGISTER_NET_MSG("NET_S2DB_load_auto_paimai",	db_server,			load_all_auto_paimai,	_T(""));
	REGISTER_NET_MSG("NET_S2DB_check_is_paimai",	db_server,			check_is_paimai,		_T(""));
	REGISTER_NET_MSG("NET_S2DB_auto_paimai_init_ok",	db_server,		auto_paimai_init_ok,	_T(""));
	REGISTER_NET_MSG("NET_S2DB_update_auto_paimai",	db_server,			update_auto_paimai,		_T(""));

	// 钱庄相关
	REGISTER_NET_MSG("NET_S2DB_get_max_bank_id",	db_server,			get_bank_max_id,		_T(""));
	REGISTER_NET_MSG("NET_S2DB_insert_bank",		db_server,			insert_bank,			_T(""));
	REGISTER_NET_MSG("NET_S2DB_delete_bank",		db_server,			delete_bank,			_T(""));
	REGISTER_NET_MSG("NET_S2DB_update_bank",		db_server,			update_bank,			_T(""));
	REGISTER_NET_MSG("NET_S2DB_load_all_bank",		db_server,			load_all_bank,			_T(""));

	// Gm命令
	REGISTER_NET_MSG("NET_DB2C_log_gm_cmd",			db_server,			insert_gm_cmd_log,			_T(""));

	//师徒 gx modify 2013.11.15
	REGISTER_NET_MSG("NET_DB2C_load_all_master_prentice",		db_server,	load_all_master_prentices,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_save_master_prentice",		db_server,		save_master_prentices,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_insert_master",			db_server,		add_new_master_prentice,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_master_delete_role",		db_server,		master_delete_role,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_master_id_and_forbid_time",db_server,	update_master_id_and_forbid_time,			_T(""));
	//REGISTER_NET_MSG("NET_DB2C_load_all_master_recruit",		db_server,		load_all_master_recruit,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_join_master_recruit",		db_server,		join_master_recruit,			_T(""));
	REGISTER_NET_MSG("NET_DB2C_leave_master_recruit",		db_server,		leave_master_recruit,			_T(""));

	//排行榜
	REGISTER_NET_MSG("NET_DB2C_load_level_randk",		db_server,			load_level_rank,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_1v1_score_rank",	db_server,			load_1v1_rank,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_equip_rank",		db_server,			load_equip_rank,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_guild_rank",		db_server,			load_guild_rank,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_kill_rank",		db_server,			load_kill_rank,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_shihun_rank",	db_server,			load_shihun_rank,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_justice_rank",	db_server,			load_justice_rank,		_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_achievement_point_rank",	db_server,	load_achpoint_rank,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_achievement_number_rank",	db_server,	load_achnumber_rank,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_MasterGraduate_randk",   db_server,	load_master_rank,	_T(""));//gx add 2013.12.06
	REGISTER_NET_MSG("NET_DB2C_load_mounts_rank",	db_server,	load_mounts_rank,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_load_reach_rank",	db_server,	load_reach_rank,	_T(""));

	// 悬赏任务
	REGISTER_NET_MSG("NET_DB2S_LoadAllGuerdonQuest", db_server,	load_all_guerdonquest, _T(""));
	REGISTER_NET_MSG("NET_DB2S_LoadAllGuerdonReward", db_server,	load_all_guerdonquest_reward, _T(""));
	REGISTER_NET_MSG("NET_DB2S_AddNewGuerdonQuest", db_server,	insert_new_guerdonquest, _T(""));
	REGISTER_NET_MSG("NET_DB2S_UpdateGuerdonQuest", db_server,	update_guerdonquest, _T(""));
	REGISTER_NET_MSG("NET_DB2S_DelGuerdonQuest", db_server,	delete_guerdonquest, _T(""));
	REGISTER_NET_MSG("NET_DB2S_SaveCreatureKillGD", db_server,	update_questcompletedata, _T(""));
	REGISTER_NET_MSG("NET_DB2S_UpdateRoleQuestFlag", db_server,	update_questflag, _T(""));
	REGISTER_NET_MSG("NET_DB2S_InsertQuestRewardItem", db_server,	insert_quest_rewarditem, _T(""));
	REGISTER_NET_MSG("NET_DB2S_DeleteQuestRewardItem", db_server,	delete_quest_rewarditem, _T(""));
	// 重置元气值
	REGISTER_NET_MSG("NET_DB2C_reset_vigour",		db_server,			reset_all_role_vigour,		_T(""));
	//REGISTER_NET_MSG("NDBC_ReplaceItemCDTime", db_server, ReplaceItemCDTime), _T(""));

	// 1v1积分竞技场
	REGISTER_NET_MSG("NET_DB2C_update_role_1v1_score", db_server,	update_role_1v1_score,	_T(""));
	REGISTER_NET_MSG("NET_DB2C_update_noline_role_1v1_score",	db_server, update_noline_1v1_score, _T(""));
	
	// 坐骑
	REGISTER_NET_MSG("NET_DB2C_update_mounts",	db_server, updata_mounts, _T(""));
}

// 注销包处理
VOID db_server::unregister_command()
{
	UNREGISTER_NET_MSG("NET_retification",		db_server,				game_server_init_ok);
	UNREGISTER_NET_MSG("NET_DB2C_world_shutdown",	db_server,		game_server_showdown);
	UNREGISTER_NET_MSG("NET_DB2C_world_init_ok",	db_server,				game_server_ok);
	UNREGISTER_NET_MSG("NET_DB2C_load_all_role_info",	db_server,				load_all_role_simple);
	UNREGISTER_NET_MSG("NET_DB2C_load_sim_role",		db_server,			load_one_role_simple);
	UNREGISTER_NET_MSG("NET_DB2C_load_role",			db_server,			load_one_role_full_data);
	UNREGISTER_NET_MSG("NET_DB2C_load_serial_reward", db_server,			load_serial_reward);
	UNREGISTER_NET_MSG("NET_DB2C_create_role",			db_server,			create_role);
	UNREGISTER_NET_MSG("NET_DB2C_delete_role",			db_server,			delete_role);
	UNREGISTER_NET_MSG("NET_DB2C_cancel_role_del_guard_time",	db_server,	role_guard_cancel_time);
	UNREGISTER_NET_MSG("NET_DB2C_delete_role_guard_time_set",	db_server,	role_guard_set_time);
	UNREGISTER_NET_MSG("NET_DB2C_resume_role",			db_server,			active_role);
	UNREGISTER_NET_MSG("NET_DB2C_save_role",			db_server,			save_one_role);
	UNREGISTER_NET_MSG("NET_DB2C_update_role_reward_data",			db_server,			update_one_role_reward_data);////
	UNREGISTER_NET_MSG("NET_DB2C_change_role_name",		db_server,			change_role_name);
	UNREGISTER_NET_MSG("NET_DB2C_change_role_guild",	db_server,			change_role_guild);
	UNREGISTER_NET_MSG("NET_DB2C_change_role_leave_guild_time",	db_server,	update_role_leave_guild_time);
	UNREGISTER_NET_MSG("NET_DB2C_change_role_VIP_Info",	db_server,	update_role_vip_info,	_T(""));//gx add
	UNREGISTER_NET_MSG("NET_DB2C_clear_role_dress_mdid",		db_server,		change_role_dress_model_id);
	UNREGISTER_NET_MSG("NET_DB2C_delete_role_map_limit",		db_server,		update_role_map_limit);
	UNREGISTER_NET_MSG("NET_DB2C_remove_role_map_limit",		db_server,		delete_role_map_limit);
	UNREGISTER_NET_MSG("NET_DB2C_insert_role_map_limit",		db_server,		insert_role_map_limit);
	UNREGISTER_NET_MSG("NET_DB2C_clean_role_hang_num",		db_server,		clean_role_hang_number);
	UNREGISTER_NET_MSG("NET_C2DB_clean_role_day",			db_server,		clean_role_day_clear);
	UNREGISTER_NET_MSG("NET_C2DB_reset_role_value_at_0",			db_server,		reset_role_at_0);
	UNREGISTER_NET_MSG("NET_DB2C_clean_role_yuanbao_exchange_num",	db_server,	clean_role_exchange_num);
	UNREGISTER_NET_MSG("NET_C2DB_CleanRolePaiMaiNum",		db_server,		clean_role_paimai_number);
	UNREGISTER_NET_MSG("NET_C2DB_clean_role_spouse_id",		db_server,	clean_role_spouse_id,	_T(""));
	UNREGISTER_NET_MSG("NET_C2DB_clear_role_VIP_info",		db_server,	clean_role_vip_info,	_T(""));//gx add
	UNREGISTER_NET_MSG("NET_DB2C_insert_pet_sns",			db_server,		insert_pet_sns_info);
	UNREGISTER_NET_MSG("NET_DB2C_delete_pet_sns",			db_server,		delete_pet_sns_info);	
	UNREGISTER_NET_MSG("NET_DB2C_load_pet_sns",				db_server,		load_pet_sns_info);	
	UNREGISTER_NET_MSG("NET_DB2C_bag_money_update",		db_server,		bag_meony_update);

	// 帐号通用属性相关
	UNREGISTER_NET_MSG("NET_DB2C_safe_code_set",		db_server,			set_safe_code);
	UNREGISTER_NET_MSG("NET_DB2C_safe_code_reset",		db_server,			reset_safe_code);
	UNREGISTER_NET_MSG("NET_DB2C_safe_code_reset_cancel",	db_server,		cancel_safe_code_reset);
	UNREGISTER_NET_MSG("NET_DB2C_ware_size_update",			db_server,		update_ware_size);
	UNREGISTER_NET_MSG("NET_DB2C_ware_money_update",		db_server,		update_ware_money);
	UNREGISTER_NET_MSG("NET_DB2C_baibao_yuanbao_update",			db_server,		update_baibao_yuanbao);
	UNREGISTER_NET_MSG("NET_DB2C_exchange_volume_update",		db_server,		update_exchange_volume);
	UNREGISTER_NET_MSG("NET_DB2C_update_receive",		db_server,		update_account_receive);
	UNREGISTER_NET_MSG("NET_DB2C_update_receive_ex",	db_server,		update_account_receive_ex);
	UNREGISTER_NET_MSG("NET_DB2C_update_web_recieve", db_server,		update_web_recieve);
	UNREGISTER_NET_MSG("NET_DB2C_delete_serial_reward",	db_server,	delete_serial_reward);

	UNREGISTER_NET_MSG("NET_DB2C_change_bag_password",			db_server,		change_bag_password);

	// 好友
	UNREGISTER_NET_MSG("NET_DB2C_insert_friend",		db_server,			insert_friend);
	UNREGISTER_NET_MSG("NET_DB2C_update_friend",		db_server,			update_friend);
	UNREGISTER_NET_MSG("NET_DB2C_delete_friend",		db_server,			delelte_friend);
	UNREGISTER_NET_MSG("NET_DB2C_delete_all_friend",		db_server,			delete_all_friend_relation);

	UNREGISTER_NET_MSG("NET_DB2C_insert_friend_value",		db_server,			insert_friendship);
	UNREGISTER_NET_MSG("NET_DB2C_update_friend_value",		db_server,			update_friendship);
	UNREGISTER_NET_MSG("NET_DB2C_delete_friend",		db_server,			delete_friendship);
	UNREGISTER_NET_MSG("NET_DB2C_delete_all_friend_value",	db_server,			delete_all_friendship_relation);

	// 黑名单
	UNREGISTER_NET_MSG("NET_DB2C_insert_black",		db_server,			insert_black);
	UNREGISTER_NET_MSG("NET_DB2C_delete_black",		db_server,			delete_black);

	UNREGISTER_NET_MSG("NET_DB2C_insert_enemy",		db_server,			insert_enemy);
	UNREGISTER_NET_MSG("NET_DB2C_delete_enemy",		db_server,			delete_enemy);

	UNREGISTER_NET_MSG("NET_DB2C_get_item_info",		db_server,			get_world_max_min_serial);
	UNREGISTER_NET_MSG("NET_DB2C_load_item_need_log",	db_server,			load_item_need_log);

	// 任务
	UNREGISTER_NET_MSG("NET_DB2C_accept_quest",		db_server,			accept_quest);
	UNREGISTER_NET_MSG("NET_DB2C_complete_quest",		db_server,			complete_quest);
	UNREGISTER_NET_MSG("NET_DB2C_discard_quest",		db_server,			delete_quest);
	UNREGISTER_NET_MSG("NET_S2DB_DelAllTeamShareQuest",		db_server,			remove_all_teamshare_quest);
	UNREGISTER_NET_MSG("NET_DB2C_clear_day_quest",		db_server,			delete_all_dayquest,			_T(""));//gx add

	// 技能
	UNREGISTER_NET_MSG("NET_DB2C_add_skill",			db_server,			insert_skill);
	UNREGISTER_NET_MSG("NET_DB2C_remove_skill",		db_server,			delete_skill);
	UNREGISTER_NET_MSG("NET_DB2C_update_skill",		db_server,			update_skill);

	// 物品装备
	UNREGISTER_NET_MSG("NET_DB2C_new_item",			db_server,			insert_item);
	UNREGISTER_NET_MSG("NET_DB2C_new_equip",			db_server,			insert_equip);
	UNREGISTER_NET_MSG("NET_DB2C_delete_item",			db_server,			delete_item);
	UNREGISTER_NET_MSG("NET_DB2C_delete_equip",			db_server,			delete_equip);
	UNREGISTER_NET_MSG("NET_DB2C_update_equip_att",		db_server,			save_equip_special);

	// 百宝袋(item_baibao表)中物品
	UNREGISTER_NET_MSG("NET_DB2C_load_baibao",			db_server,			load_item_baibao);
	UNREGISTER_NET_MSG("NET_DB2C_load_baibao_yuanbao",	db_server,			load_baibao_yuanbao);
	UNREGISTER_NET_MSG("NET_DB2C_new_baibao_item",		db_server,			insert_baibao_item);
	UNREGISTER_NET_MSG("NET_DB2C_new_baibao_equip",		db_server,			insert_baibao_equip);
	UNREGISTER_NET_MSG("NET_DB2C_delete_baobao_item",		db_server,			delete_baibao_item);
	UNREGISTER_NET_MSG("NET_DB2C_delete_baibao_equip",		db_server,			delete_baibao_equip);
	UNREGISTER_NET_MSG("NET_DB2C_update_yuanbao_recharge",	db_server,		update_yuanbao_recharge);
	UNREGISTER_NET_MSG("NET_DB2C_update_total_recharge",		db_server,		update_total_recharge);
	UNREGISTER_NET_MSG("NET_DB2C_load_web_receive",		db_server,			load_web_receive);

	// 百宝袋Log信息
	UNREGISTER_NET_MSG("NET_DB2C_add_baibao_log",		db_server,			insert_baibao_log_info);
	UNREGISTER_NET_MSG("NET_DB2C_load_baobao_log",		db_server,			load_baibao_log_info);

	// 称号
	UNREGISTER_NET_MSG("NET_DB2C_title_insert",		db_server,			insert_title);
	UNREGISTER_NET_MSG("NET_DB2C_title_delete",		db_server,			delete_title);	
	// 角色名贴
	UNREGISTER_NET_MSG("NET_DB2C_vcard_change",			db_server,			update_visiting_card);
	UNREGISTER_NET_MSG("NET_DB2C_get_off_line_vcard",	db_server,			load_visiting_card);


	// 名人堂
	UNREGISTER_NET_MSG("NET_DB2C_fame_hall_init_start",	db_server,			fame_hall_initial);
	UNREGISTER_NET_MSG("NET_DB2C_get_rep_rank_list",		db_server,			load_reputation_rank);
	UNREGISTER_NET_MSG("NET_DB2C_insert_fame_hall_snap",	db_server,			insert_fame_hall_enter_snap);
	UNREGISTER_NET_MSG("NET_DB2C_insert_act_clan_treasure",	db_server,		insert_act_treasure);
	UNREGISTER_NET_MSG("NET_DB2C_update_reprst_time",		db_server,		update_rep_reset_time);

	// 元宝交易
	UNREGISTER_NET_MSG("NET_DB2C_load_all_yuanbao_account",	db_server,			load_all_yuanbao_account);
	UNREGISTER_NET_MSG("NET_DB2C_load_all_order",		db_server,			load_all_yuanbao_order);
	UNREGISTER_NET_MSG("NET_DB2C_insert_account",		db_server,			insert_yuanbao_account);
	UNREGISTER_NET_MSG("NET_DB2C_syn_account_yanbao",		db_server,			update_account_yuanbao);
	UNREGISTER_NET_MSG("NET_DB2C_syn_account_silver",	db_server,			update_account_silver);
	UNREGISTER_NET_MSG("NET_DB2C_syn_submit_sell",		db_server,			update_submit_sell);
	UNREGISTER_NET_MSG("NET_DB2C_syn_submit_buy",		db_server,			update_submit_buy);
	UNREGISTER_NET_MSG("NET_DB2C_insert_order",		db_server,			insert_one_order);
	UNREGISTER_NET_MSG("NET_DB2C_complete_order",		db_server,			complete_one_order);
	UNREGISTER_NET_MSG("NET_DB2C_refresh_order",		db_server,			refresh_one_order);
	UNREGISTER_NET_MSG("NET_DB2C_get_role_yuanbao_order",		db_server,			load_one_role_all_yuanbao_order);
	UNREGISTER_NET_MSG("NET_DB2C_delete_yuanbao_order",		db_server,			delete_one_order);

	// 帮派属性相关
	UNREGISTER_NET_MSG("NET_DB2C_load_all_guild",		db_server,			load_all_guild);
	UNREGISTER_NET_MSG("NET_DB2C_guild_init_ok",		db_server,			check_guild_init_ok);
	UNREGISTER_NET_MSG("NET_DB2C_create_guild",		db_server,			create_guild);
	UNREGISTER_NET_MSG("NET_DB2C_dismiss_guild",		db_server,			dismiss_guild);
	UNREGISTER_NET_MSG("NET_DB2C_change_guild_tenet",	db_server,			change_guild_tenet);
	UNREGISTER_NET_MSG("NET_DB2C_change_guild_symbol",	db_server,			change_guild_symbol);
	UNREGISTER_NET_MSG("NET_DB2C_save_all_guild",		db_server,			save_guild_all);
	UNREGISTER_NET_MSG("NET_DB2C_save_guild_att",		db_server,			save_guild_attribute);
	UNREGISTER_NET_MSG("NET_DB2C_change_guild_pos_name",	db_server,			change_guild_pos_name);
	UNREGISTER_NET_MSG("NET_DB2C_change_guild_pos_power",	db_server,		update_guild_pos_power);
	UNREGISTER_NET_MSG("NET_C2DB_load_guild_plant_data",	db_server,		load_guild_plant_data);
	UNREGISTER_NET_MSG("NET_DB2C_save_plant",		db_server,				save_guild_plant);
	UNREGISTER_NET_MSG("NET_DB2C_save_all_guild_plant",		db_server,			save_guild_all_plant);

	// 帮会pvp相关
	UNREGISTER_NET_MSG("NET_DB2C_insert_pvp_data",		db_server,			insert_guild_pvp_data);
	UNREGISTER_NET_MSG("NET_DB2C_load_pvp_data",		db_server,			load_guild_pvp_data);
	UNREGISTER_NET_MSG("NET_DB2C_up_pvp_data",			db_server,			update_pvp_data);

	// 帮派成员属性相关
	UNREGISTER_NET_MSG("NET_DB2C_load_all_guild_member",		db_server,		load_all_guild_member);
	UNREGISTER_NET_MSG("NET_DB2C_join_guild",			db_server,			join_guild);
	UNREGISTER_NET_MSG("NET_DB2C_leave_guild",			db_server,			leave_guild);
	UNREGISTER_NET_MSG("NET_DB2C_change_guild_pos",		db_server,			change_guild_member_pos);
	UNREGISTER_NET_MSG("NET_DB2C_guild_ware_pri",		db_server,			guild_ware_privilege);
	UNREGISTER_NET_MSG("NET_DB2C_change_contrib",		db_server,			change_member_contribution);
	UNREGISTER_NET_MSG("NET_DB2C_change_ballot",		db_server,			change_ballot);
	UNREGISTER_NET_MSG("NET_DB2C_change_dkp",			db_server,			change_DKP);
	UNREGISTER_NET_MSG("NET_C2DB_load_guild_war_history",			db_server,			load_guild_war_history);
	UNREGISTER_NET_MSG("NET_C2DB_insert_guild_war_history",			db_server,			insert_guild_war_history);
	UNREGISTER_NET_MSG("NET_C2DB_insert_guild_war_history",			db_server,			delete_guild_war_history);

	// 帮派仓库相关
	UNREGISTER_NET_MSG("NET_DB2C_load_guild_ware_items",		db_server,		load_guild_ware_all_items);
	UNREGISTER_NET_MSG("NET_DB2C_save_guild_ware",			db_server,		save_guild_ware_items);

	// 帮派设施相关
	UNREGISTER_NET_MSG("NET_DB2C_load_facilities_info",		db_server,		load_guild_facilities);
	UNREGISTER_NET_MSG("NET_DB2C_create_facilities",		db_server,		insert_guild_facilities);
	UNREGISTER_NET_MSG("NET_DB2C_facilities_upgrade",		db_server,		update_guild_facilities);
	UNREGISTER_NET_MSG("NET_DB2C_remove_all_facilities",	db_server,		delete_guild_facilities);

	// 帮会招募榜
	UNREGISTER_NET_MSG("NET_C2DB_load_guild_recruit",	db_server,			load_guild_recruit);
	UNREGISTER_NET_MSG("NET_C2DB_insert_guild_recruit",	db_server,			insert_guild_recruit);
	UNREGISTER_NET_MSG("NET_C2DB_delete_guild_recruit", db_server,			delete_guild_recruit);
	UNREGISTER_NET_MSG("NET_C2DB_clean_guild_recruit", db_server,			clean_guild_recruit);

	// 帮会boss击杀
	UNREGISTER_NET_MSG("NET_C2DB_load_guild_skill_boss",	db_server,		load_guild_skill_boss);
	UNREGISTER_NET_MSG("NET_C2DB_insert_guild_skill_boss", db_server,		insert_guild_kill_boss);

	// 帮派技能相关
	UNREGISTER_NET_MSG("NET_DB2C_load_guild_skill_info",		db_server,		load_guild_skill_info);
	UNREGISTER_NET_MSG("NET_DB2C_create_guild_skill",		db_server,		insert_guild_skill_info);
	UNREGISTER_NET_MSG("NET_DB2C_save_guild_skill",			db_server,		save_guild_skill_info);
	UNREGISTER_NET_MSG("NET_DB2C_change_research_skill",	db_server,		change_research_guild_skill);
	UNREGISTER_NET_MSG("NET_DB2C_remove_guild_skill",		db_server,		delete_guild_skill_info);

	// 帮派弹劾相关
	UNREGISTER_NET_MSG("NET_DB2C_create_guild_delate",		db_server,		insert_guild_delate);
	UNREGISTER_NET_MSG("NET_DB2C_load_guild_delate",		db_server,		load_guild_delate);
	UNREGISTER_NET_MSG("NET_DB2C_remove_guild_delate",		db_server,		delete_guild_delate);
	UNREGISTER_NET_MSG("NET_DB2C_update_guild_dalate",		db_server,		update_guild_delate);
	UNREGISTER_NET_MSG("NET_DB2C_update_guild_delate_content",	db_server,	update_guild_delate_content);

	// 帮派跑商相关
	UNREGISTER_NET_MSG("NET_DB2C_load_commodity",		db_server,			load_guild_commerce_info);
	UNREGISTER_NET_MSG("NET_DB2C_load_commerce_rank",	db_server,			load_commerce_rank_info);
	UNREGISTER_NET_MSG("NET_DB2C_create_commodity",	db_server,			insert_commodity_info);
	UNREGISTER_NET_MSG("NET_DB2C_change_tael",			db_server,			update_commodity_tael);
	UNREGISTER_NET_MSG("NET_DB2C_save_commodity",		db_server,			save_commodity_info);
	UNREGISTER_NET_MSG("NET_DB2C_remove_commodity",	db_server,			delete_commodity_info);
	UNREGISTER_NET_MSG("NET_DB2C_set_commendation",	db_server,			set_guild_commendation);
	UNREGISTER_NET_MSG("NET_DB2C_update_commerce_rank",	db_server,			update_commerce_rank_info);
	UNREGISTER_NET_MSG("NET_DB2C_remove_commerce_rank",	db_server,			delete_commerce_rank_info);

	// 宠物相关
	UNREGISTER_NET_MSG("NET_DB2C_create_pet_soul",		db_server,			create_new_pet);
	UNREGISTER_NET_MSG("NET_DB2C_delete_pet_soul",			db_server,			delete_pet);
	UNREGISTER_NET_MSG("NET_DB2C_update_pet_soul",		db_server,			update_pet_attribute);

	UNREGISTER_NET_MSG("NET_DB2C_insert_pet_skill",		db_server,			insert_pet_skill);
	UNREGISTER_NET_MSG("NET_DB2C_delete_pet_skill",		db_server,			delete_pet_skill);

	// 商城相关
	UNREGISTER_NET_MSG("NET_DB2C_get_all_gp_info",		db_server,			load_all_group_purchase_info);
	UNREGISTER_NET_MSG("NET_DB2C_add_gp_info",			db_server,			add_group_purchase_info);
	UNREGISTER_NET_MSG("NET_DB2C_remove_gp_info",		db_server,			delete_group_purchase_info);
	UNREGISTER_NET_MSG("NET_DB2C_remove_guild_gp_info",	db_server,			delete_guild_group_purchase_info);
	UNREGISTER_NET_MSG("NET_DB2C_update_gp_info",		db_server,			update_group_purchase_info);
	UNREGISTER_NET_MSG("NET_DB2C_update_gp_time",		db_server,			update_group_purchase_time);

	// VIP摊位相关
	UNREGISTER_NET_MSG("NET_DB2C_get_all_vip_stall_info",		db_server,		get_all_vip_stall_info);
	UNREGISTER_NET_MSG("NET_DB2C_apply_vip_stall",		db_server,			apply_vip_stall);
	UNREGISTER_NET_MSG("NET_DB2C_update_vip_stall",		db_server,			update_vip_stall_time);

	UNREGISTER_NET_MSG("NET_DB2C_update_vnb_player",	db_server,			update_vip_net_bar_player);
	UNREGISTER_NET_MSG("NET_DB2C_get_vnb_data",			db_server,			load_vip_net_bar);
	//	开启宝箱相关
	UNREGISTER_NET_MSG("NET_DB2C_update_treasure_sum",	db_server,			update_treasure_count);

	// 固定活动
	UNREGISTER_NET_MSG("NET_DB2C_load_activity_data",	db_server,			load_activities);
	UNREGISTER_NET_MSG("NET_DB2C_save_activity_data",	db_server,			load_activities);

	// log相关
	UNREGISTER_NET_MSG("NET_DB2C_log_silver",	db_server,			insert_money_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_yuanbao",	db_server,			insert_yuanbao_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_exvolume",	db_server,			insert_exvolume_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_exploits",	db_server,			insert_log_exploits);
	UNREGISTER_NET_MSG("NET_DB2C_log_time_stat",db_server,			insert_time_stat_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_item",		db_server,			insert_item_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_mall_sell",db_server,			insert_mall_sell_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_mall_sell_pack",	db_server,			insert_mall_sell_pack_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_item_times",db_server,			insert_item_times_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_fund",			db_server,			insert_fund_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_material",		db_server,			insert_material_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_reputation",	db_server,			insert_reputation_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_bank",			db_server,			insert_bank_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_paimai",			db_server,			insert_paimai_log);
	UNREGISTER_NET_MSG("NET_DB2C_log_kick",			db_server,			insert_kick_log);
	
	UNREGISTER_NET_MSG("NET_DB2C_update_online_num", db_server,			update_online_num);
	// 留言相关
	UNREGISTER_NET_MSG("NET_DB2C_load_left_message",		db_server,			load_leave_msg);
	UNREGISTER_NET_MSG("NET_DB2C_insert_left_message",		db_server,			insert_leave_msg);
	UNREGISTER_NET_MSG("NET_DB2C_clear_left_message",		db_server,			clear_leave_msg);

	// 保存玩家经验和点
	UNREGISTER_NET_MSG("NET_DB2C_update_role_att",		db_server,			update_role_attribute);
	UNREGISTER_NET_MSG("NET_DB2C_update_role_att_point",		db_server,		update_role_attribute_point);
	UNREGISTER_NET_MSG("NET_DB2C_update_role_talent_point",	db_server,		update_role_talent_point);
	UNREGISTER_NET_MSG("NET_DB2C_clean_role_leave_exp",		db_server,		clear_role_leave_exp);
	UNREGISTER_NET_MSG("NET_DB2C_clean_role_leave_brother",	db_server,		clear_role_leave_brotherhood);

	// 压力测试专用
	UNREGISTER_NET_MSG("NET_DB2C_test_load_role",		db_server,			force_test_load_role);
	UNREGISTER_NET_MSG("NET_DB2C_test_load_role_item",	db_server,			force_test_load_item);
	UNREGISTER_NET_MSG("NET_DB2C_test_save_item",		db_server,			force_test_save_item);
	UNREGISTER_NET_MSG("NET_DB2C_test_save_equip",		db_server,			force_test_save_equip);

	// 邮件相关
	UNREGISTER_NET_MSG("NET_DB2C_get_mail_max_serial",	db_server,			get_mail_max_serial);
	UNREGISTER_NET_MSG("NET_DB2C_save_mail_item",		db_server,			update_mail_item);
	UNREGISTER_NET_MSG("NET_DB2C_save_mail",			db_server,			save_mail);
	UNREGISTER_NET_MSG("NET_DB2C_load_all_mail",		db_server,			load_all_mail);
	UNREGISTER_NET_MSG("NET_DB2C_load_mail_content",	db_server,			load_mail_content);
	UNREGISTER_NET_MSG("NET_DB2C_load_mail_item",		db_server,			load_mail_item);
	UNREGISTER_NET_MSG("NET_DB2C_update_mail",			db_server,			update_mail);
	UNREGISTER_NET_MSG("NET_DB2C_delete_mail",			db_server,			delete_mail);
	UNREGISTER_NET_MSG("NET_DB2C_change_role_money",	db_server,			change_role_money);
	UNREGISTER_NET_MSG("NET_DB2C_update_role_send_num",	db_server,			update_role_send_mail_count);

	// Gm命令
	UNREGISTER_NET_MSG("NET_DB2C_log_gm_cmd",			db_server,			insert_gm_cmd_log);

	//师徒
	UNREGISTER_NET_MSG("NET_DB2C_load_all_master_prentice",		db_server,	load_all_master_prentices);
	UNREGISTER_NET_MSG("NET_DB2C_save_master_prentice",		db_server,		save_master_prentices);
	UNREGISTER_NET_MSG("NET_DB2C_insert_master",			db_server,		add_new_master_prentice);
	UNREGISTER_NET_MSG("NET_DB2C_master_delete_role",		db_server,		master_delete_role);
	UNREGISTER_NET_MSG("NET_DB2C_update_master_id_and_forbid_time",db_server,	update_master_id_and_forbid_time);

	//排行榜
	UNREGISTER_NET_MSG("NET_DB2C_load_level_randk",		db_server,			load_level_rank);
	UNREGISTER_NET_MSG("NET_DB2C_load_1v1_score_rank",	db_server,			load_1v1_rank);
	UNREGISTER_NET_MSG("NET_DB2C_load_equip_rank",		db_server,			load_equip_rank);
	UNREGISTER_NET_MSG("NET_DB2C_load_guild_rank",		db_server,			load_guild_rank);
	UNREGISTER_NET_MSG("NET_DB2C_load_kill_rank",		db_server,			load_kill_rank);
	UNREGISTER_NET_MSG("NET_DB2C_load_MasterGraduate_randk",     db_server,	        load_master_rank);//gx add 2013.12.06
	UNREGISTER_NET_MSG("NET_DB2C_load_shihun_rank",		db_server,			load_shihun_rank);
	UNREGISTER_NET_MSG("NET_DB2C_load_justice_rank",		db_server,			load_justice_rank);
	UNREGISTER_NET_MSG("NET_DB2C_load_achievement_point_rank",		db_server,	load_achpoint_rank);
	UNREGISTER_NET_MSG("NET_DB2C_load_achievement_number_rank",		db_server,	load_achnumber_rank);
	UNREGISTER_NET_MSG("NET_DB2C_load_mounts_rank",		db_server,			load_mounts_rank);
	UNREGISTER_NET_MSG("NET_DB2C_load_reach_rank",		db_server,			load_reach_rank);

	// 拍卖相关
	UNREGISTER_NET_MSG("NET_S2DB_load_paimai_max_id",	db_server,			get_paimai_max_id);
	UNREGISTER_NET_MSG("NET_S2DB_update_paimai_item",	db_server,			update_paimai_item);
	UNREGISTER_NET_MSG("NET_S2DB_insert_paimai",		db_server,			insert_paimai);
	UNREGISTER_NET_MSG("NET_S2DB_load_all_paimai",	db_server,			load_all_paimai_info);
	UNREGISTER_NET_MSG("NET_S2DB_load_paimai_item",	db_server,			load_paimai_item);
	UNREGISTER_NET_MSG("NET_S2DB_delete_paimai",		db_server,			delete_paimai);
	UNREGISTER_NET_MSG("NET_S2DB_update_paimai",		db_server,			update_paimai);

	// 重置元气值
	UNREGISTER_NET_MSG("NET_DB2C_reset_vigour",		db_server,			reset_all_role_vigour);

	// 悬赏任务
	UNREGISTER_NET_MSG("NET_DB2S_LoadAllGuerdonQuest", db_server,	load_all_guerdonquest);
	UNREGISTER_NET_MSG("NET_DB2S_AddNewGuerdonQuest", db_server,	insert_new_guerdonquest);

	// 钱庄相关
	UNREGISTER_NET_MSG("NET_S2DB_get_max_bank_id",	db_server,			get_bank_max_id);
	UNREGISTER_NET_MSG("NET_S2DB_insert_bank",		db_server,			insert_bank);
	UNREGISTER_NET_MSG("NET_S2DB_delete_bank",		db_server,			delete_bank);
	UNREGISTER_NET_MSG("NET_S2DB_update_bank",		db_server,			update_bank);
	UNREGISTER_NET_MSG("NET_S2DB_load_all_bank",	db_server,			load_all_bank);
	UNREGISTER_NET_MSG("NET_C2DB_clean_role_bank_num",	db_server,		clean_role_bank_number);
	UNREGISTER_NET_MSG("NET_C2DB_update_day_1v1_score",	db_server,		update_day_1v1_score);
	UNREGISTER_NET_MSG("NET_C2DB_update_week_1v1_score",	db_server,		update_week_1v1_score);
	UNREGISTER_NET_MSG("NET_C2DB_update_1v1_award",		db_server,		update_1v1_award);

	// 1v1积分竞技场
	UNREGISTER_NET_MSG("NET_DB2C_update_role_1v1_score", db_server,	update_role_1v1_score);
	UNREGISTER_NET_MSG("NET_DB2C_update_noline_role_1v1_score",	db_server, update_noline_1v1_score);

	UNREGISTER_NET_MSG("NET_DB2C_update_mounts",	db_server, updata_mounts);
}
