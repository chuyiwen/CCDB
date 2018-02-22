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
#include "../../common/WorldDefine/QuestDef.h"
#include "../../common/WorldDefine/SocialDef.h"
#include "../../common/WorldDefine/ItemDefine.h"
#include "../../common/WorldDefine/RoleDefine.h"
#include "../../common/WorldDefine/famehall_define.h"
#include "../../common/WorldDefine/mall_define.h"
#include "../../common/WorldDefine/Sign_define.h"
#include "../../common/WorldDefine/LianHun_define.h"
#include "../../common/WorldDefine/reward_define.h"
#include "../../common/WorldDefine/Raid_define.h"

#include "../common/ServerDefine/role_data_server_define.h"
#include "../common/ServerDefine/vcard_server_define.h"
#include "../common/ServerDefine/mall_server_define.h"
#include "../common/ServerDefine/vipstall_server_define.h"
#include "../common/ServerDefine/activity_server_define.h"
#include "../common/ServerDefine/mail_server_define.h"
#include "../common/ServerDefine/mail_server_define.h"
#include "../common/ServerDefine/rank_server_define.h"
#include "../common/ServerDefine/title_server_define.h"
#include "../common/ServerDefine/achievement_server_define.h"

#include "db_server_handler.h"

#include "db_handler.h"

// 获取角色完整数据
BOOL db_handler::load_one_role_full_data(DWORD dw_account_id, DWORD dw_role_id, s_role_data_load *p_role_data)
{
	LPVOID p_buffer = p_role_data;
	// 调用接口读数据
	ZeroMemory(p_role_data, sizeof(s_role_data_load));
	if(!load_one_role_data(p_buffer, dw_account_id, dw_role_id))
	{
		p_role_data->dw_size = 0;
		return FALSE;
	}
	//************************************************
	// 1.最好和保存玩家时顺序一致。
	// 2.注意每次都必须更改指针可读位置。
	// 3.新增数据最好加在尾部，不要从中间插入
	// 4.完成所有数据后需要重设包的大小
	// 此处采用如下顺序：
	// 技能—>状态—>称号—>物品—>装备—>好友—>敌人—>任务—>完成任务->
	// 物品CD—>友好度—>黑名单—>角色名片—>家族—>宠物—>地图进入限制
	//************************************************

	DWORD dw_indexes[50] = {0}; int n_msg_number = 0;
	load_leave_msg_indexes(dw_indexes, dw_role_id, n_msg_number);
	p_role_data->role_data_save_.h_has_left_msg_ = n_msg_number > 0 ? TRUE : FALSE;

	s_role_data_save *p_data_save = &p_role_data->role_data_save_;

	// 可读开始位置
	p_buffer = p_data_save->by_data; 

	// 1.技能
	load_one_role_skill(p_buffer, p_data_save->n_skill_num_, dw_role_id);

	// 2.状态
	load_one_role_buff(p_buffer, p_data_save->n_buff_num_, dw_role_id);

	// 4.称号
	load_one_role_title(p_buffer, p_data_save->n_title_num_, dw_role_id);

	// 5.物品(6.装备)
	load_one_role_items(p_buffer, p_data_save->n_item_num_, dw_account_id, dw_role_id);

	// 7.好友
	load_one_role_friends(p_buffer, p_data_save->n_friend_num_, dw_role_id);

	// 8.敌人
	load_one_role_enemies(p_buffer, p_data_save->n_enemy_num_, dw_role_id);

	// 9.任务
	load_one_role_quest(p_buffer, p_data_save->n_quest_num_, dw_role_id);

	// 10.已完成任务
	load_one_role_quest_done(p_buffer, p_data_save->n_quest_done_num, dw_role_id);

	// 11.物品CD
	load_one_role_item_cd(p_buffer, p_data_save->n_item_cd_time_num_, dw_role_id);

	// 12.友好度
	load_one_role_friendship(p_buffer, p_data_save->n_friendship_num_, dw_role_id);

	int nFriendNumEx = 0;
	load_one_role_friendship_ex(p_buffer, nFriendNumEx, dw_role_id);
	p_data_save->n_friendship_num_ += nFriendNumEx;

	// 13.黑名单
	load_one_role_black(p_buffer, p_data_save->n_black_num_, dw_role_id);

	// 14.角色名片
	load_visiting_card(p_buffer, dw_role_id);

	// 15.家族
	/*if(!load_clan_data(p_buffer,dw_role_id))
	{
		p_role_data->dw_size = 0;
		return FALSE;
	}*/

	// 16.宠物
	//if (!load_pets(p_buffer, dw_role_id, p_data_save->n_pets_num_))
	//{
	//	p_role_data->dw_size = 0;
	//	return FALSE;
	//}

	// 17.地图进入限制
	load_one_role_map_limit(p_buffer, p_data_save->n_map_limit_num_, dw_role_id);
	
	// 18.完成成就
	//load_complate_achievement(p_buffer, p_data_save->n_achievement_num_, dw_role_id);

	// 19.成就进度
	//load_progress_achievement(p_buffer, p_data_save->n_achievement_criteria_num_, dw_role_id);

	// 20.副本进度
	load_process_instance(p_buffer, p_data_save->n_inst_process_num, dw_role_id);

	// 21.签到数据
	if(!load_sign_data(p_buffer, dw_role_id))
	{
		p_role_data->dw_size = 0;
		return FALSE;
	}
	// 22.魂精数据
	//if (!load_huenjing_data(p_buffer, dw_role_id))
	//{
	//	p_role_data->dw_size = 0;
	//	return FALSE;
	//}
	
	// 23.奖励数据
	if (!load_reward_data(p_buffer, dw_role_id))
	{
		p_role_data->dw_size = 0;
		return FALSE;
	}

	// 24.坐骑数据
	if (!load_mounts_data(p_buffer, dw_role_id))
	{
		p_role_data->dw_size = 0;
		return FALSE;
	}

	// 重设包大小
	p_role_data->dw_size = (sizeof(s_role_data_load) - 1) + ((BYTE*)p_buffer - p_data_save->by_data);

	return TRUE;
}
// 获取所有玩家基本数据
BOOL db_handler::load_all_role_simple(LPVOID p_buffer, int &n_role_count)
{
	return common_select(p_buffer, &n_role_count, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_role_simple), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_all_role_simple));
}
VOID db_handler::makesql_load_all_role_simple(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("role_data", "role_name,account_id,role_id,role_name_crc,sex,map_id,level,logout_time,class");
	p_stream->where_item();
	p_stream->write_string("remove_flag=") << 0;
}
VOID db_handler::procres_load_all_role_simple(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_role_info, p_buffer, s_role_info);
	s_role_info * p_role_info = ( s_role_info * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	// RoleID,RoleName,RoleNameCrc,Sex,MapID,Level
	for(int i=0; i<*p_number; ++i)
	{
		(*p_result)[0].get_tchar(p_role_info[i].sz_role_name, X_SHORT_NAME);
		p_role_info[i].dw_account_id = (*p_result)[1].get_dword();
		p_role_info[i].dw_role_id = (*p_result)[2].get_dword();
		p_role_info[i].dw_role_name_crc = (*p_result)[3].get_dword();
		p_role_info[i].by_sex_ = (*p_result)[4].get_byte();
		p_role_info[i].dw_map_id_ = (*p_result)[5].get_dword();
		p_role_info[i].by_level = (*p_result)[6].get_byte();
		DataTime2DwordTime(p_role_info[i].dw_time_last_logout_, (*p_result)[7].get_string(), (*p_result)[7].get_length());
		p_role_info[i].e_class_type_ = (EClassType) (*p_result)[8].get_byte();

		p_result->next_row();
	}
}

// 获取角色简单数据
BOOL db_handler::load_one_role_simple(DWORD dw_account_id, int &n_role_count, LPVOID p_sample_role)
{
	return common_select(p_sample_role, &n_role_count, dw_account_id, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_simple), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_simple));
}
VOID db_handler::makesql_load_one_role_simple(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("role_data", 
		"role_name,role_id,map_id,level,"
		"hair_model_id,hair_color_id,face_model_id,face_detail_id,"
		"dress_model_id,sex,avatar_equip,display_set,"
		"class,login_time,visualizeid,last_change_name_time,delete_role_guard_time");
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
	p_stream->write_string(" and remove_flag=") << 0;
}

VOID db_handler::procres_load_one_role_simple(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_simple_role, p_buffer, tagSimRole);
	tagSimRole * p_simple_role = ( tagSimRole * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	if(*p_number > MAX_ROLENUM_ONEACCOUNT)
	{
		// 应考虑容错处理
		ASSERT(*p_number <= MAX_ROLENUM_ONEACCOUNT);
		*p_number = MAX_ROLENUM_ONEACCOUNT;
	}

	// RoleName,RoleID,MapID,SafeCodeCrc,Level,
	// Status,HairModelID,HairColorID,FaceModelID,FaceDetailID,DressModelID
	for(int i=0; i<*p_number; ++i)
	{
		(*p_result)[0].get_tchar(p_simple_role[i].szRoleName, X_SHORT_NAME);
		p_simple_role[i].dw_role_id = (*p_result)[1].get_dword();

		p_simple_role[i].dwMapID = (*p_result)[2].get_dword();
		p_simple_role[i].byLevel = (*p_result)[3].get_byte();

		p_simple_role[i].Avatar.wHairMdlID = (*p_result)[4].get_short();
		p_simple_role[i].Avatar.wHairTexID = (*p_result)[5].get_short();
		p_simple_role[i].Avatar.wFaceMdlID = (*p_result)[6].get_short();
		p_simple_role[i].Avatar.wFaceDetailTexID = (*p_result)[7].get_short();
		p_simple_role[i].Avatar.wDressMdlID = (*p_result)[8].get_short();
		p_simple_role[i].Avatar.bySex = (BYTE)(*p_result)[9].get_short();
		p_simple_role[i].sDisplaySet = (*p_result)[11].get_byte();
		p_simple_role[i].eClassType = (EClassType)(*p_result)[12].get_int();
		p_simple_role[i].Avatar.byClass = (BYTE)p_simple_role[i].eClassType;
		DataTime2DwordTime(p_simple_role[i].dwLoginTime, (*p_result)[13].get_string(), (*p_result)[13].get_length());
		p_simple_role[i].Avatar.byVisualize = (*p_result)[14].get_byte();
		p_simple_role[i].dwChangeNameTime = (*p_result)[15].get_dword();
		p_simple_role[i].dwDelGuardTime = (*p_result)[16].get_dword();

		ZeroMemory(&p_simple_role[i].AvatarEquip, SIZE_AVATAR_EQUIP);
		if(!(*p_result)[10].is_null())
			(*p_result)[10].get_blob(&p_simple_role[i].AvatarEquip, min((*p_result)[10].get_length(), SIZE_AVATAR_EQUIP));


		p_result->next_row();
	}
}

// 获取角色基础数据
BOOL db_handler::load_one_role_data(LPVOID &p_buffer, DWORD dw_account_id, DWORD dw_role_id)
{
	return common_select(p_buffer, NULL, dw_account_id, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_data), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_data));
}

VOID db_handler::makesql_load_one_role_data(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("role_data",		
		"role_name," "role_name_crc," "display_set,"	"sex," "hair_model_id,"
		"hair_color_id," "face_model_id,"	"face_detail_id,"	"dress_model_id,"	"map_id,"	
		"x,"			"y,"			"z,"			"face_x,"		"face_y,"	
		"face_z,"		"class,"		"classex,"		"level,"		"exp_cur_level,"	
		"hp,"			"mp,"			"att_avail,"	"talent_avail," "physique_added,"	
		"strength_added," "pneuma_added," "innerforce_added," "technique_added," "agility_added,"	
		"injury,"		"vitality,"		"knowledge,"	"morality,"		"culture,"	
		"credit,"		"identity,"		"vip_point,"		"bag_size,"		"bag_gold,"	
		"bag_silver,"	"bag_yuanbao,"	"guild_id,"			"0,"			"0,"	
		"0,"			"create_time,"	"login_time,"	"logout_time,"	"online_time,"	
		"cur_online_time," "rage,"			"endurance,"	"safe_guard_flag,"		"close_safe_guard_time,"
		"talent_type1,"	"talent_type2,"	"talent_type3,"	"talent_type4,"	"talent_val1,"
		"talent_val2,"	"talent_val3,"	"talent_val4,"	"reborn_map_id,"	"total_tax,"	
		"morale,"		"get_mall_free_time,""remote_open_set," "cur_title_id,"	"exchange_volume,"
		"script_data,"	"treasure_sum,"	"stall_level,"	"stall_daily_exp," "stall_cur_exp,"	
		"stall_last_time," "pk_value,"		"team_id,"		"leave_guild_time," "send_mail_num," 
		"master_id," "masterprentice_forbid_time," "map_limit_num,"	 "own_instance_id,"	"own_instance_map_id,"	
		"instance_create_time," "hang_num," "is_exp,"		"is_brotherhood,"	"leave_exp,"		
		"leave_brotherhood,"	  "vigour," "bag_copper,"	"pet_packet_num,"	"role_help,"			
		"role_talk,"			  "key_info," "kill_num,"		"gift_step,"			"gift_id,"		
		"gift_leaving_time,"	  "gift_get," "gift_group_id," "total_mastermoral,"	"visualizeid,"		
		"bag_bind_gold,"		  "bag_bind_silver," "bag_bind_copper,"	"role_camp,"		"paimailimit,"  
		"banklimit," "today_online_tick," "history_vigour_cost," "exbagstep,"		"exwarestep,"	
		"ware_size,"	"ware_gold,"	"ware_silver,"		"ware_copper," "signature_name,"
		"circle_quest,"	"yuanbao_exchange_num," "achievemetn_point," "forbid_talk_start," "forbid_talk_end," "graduate_num," "destory_equip_count,"
		"cur_1v1_score," "day_1v1_score," "day_1v1_num," "score_1v1_award," "exploits," "circle_quest_refresh,"	"exploitslimit,"
		"active_num,"	"active_data,"	"active_receive,"	"justice," "purpuredec," "circle_quest_perdaynumber," "day_clear," "cooldownrevive_cd,"
		"circle_quest_refresh_daymax," "perday_hang_getexp_timems," "pet_xiulian_size," "guild_active_num," "guild_active_data," "guild_active_receive," "perday_vigour_get_total,"
		"god_level," "cur_title_id2," "cur_title_id3," "instance_pass," "shaodang_begin_time," "shaodang_index," "spouse_id," "vip_level," "vip_deadline," "shihun");

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and account_id=") << dw_account_id;
	p_stream->write_string(" and remove_flag=") << 0;
}

VOID db_handler::procres_load_one_role_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(NULL == p_number);

	//M_trans_pointer(p_role_data, p_buffer, s_role_data_load);
	s_role_data_load * p_role_data = ( s_role_data_load * ) p_buffer ;  

	s_role_data_const	*p_role_data_const = &p_role_data->role_data_const_;
	s_role_data_save		*p_role_data_save	= &p_role_data->role_data_save_;

	if(p_result->get_row_count() != 1)
	{
		// 应考虑容错处理
		ASSERT(p_result->get_row_count() == 1);
	}

	(*p_result)[0].get_tchar(p_role_data_const->sz_role_name, X_SHORT_NAME);
	p_role_data_const->dw_role_name_crc				= (*p_result)[1].get_dword();
	p_role_data_const->avatar.bySex				= (*p_result)[3].get_byte();
	p_role_data_const->avatar.wHairMdlID			= (*p_result)[4].get_short();
	p_role_data_const->avatar.wHairTexID			= (*p_result)[5].get_short();
	p_role_data_const->avatar.wFaceMdlID			= (*p_result)[6].get_short();
	p_role_data_const->avatar.wFaceDetailTexID		= (*p_result)[7].get_short();
	p_role_data_const->avatar.wDressMdlID			= (*p_result)[8].get_short();

	p_role_data_save->display_set_					= (*p_result)[2].get_byte();
	p_role_data_save->dw_map_id_						= (*p_result)[9].get_dword();
	p_role_data_save->f_coordinate_[0]				= (*p_result)[10].get_float();
	p_role_data_save->f_coordinate_[1]				= (*p_result)[11].get_float();
	p_role_data_save->f_coordinate_[2]				= (*p_result)[12].get_float();
	p_role_data_save->f_face_[0]						= (*p_result)[13].get_float();
	p_role_data_save->f_face_[1]						= (*p_result)[14].get_float();
	p_role_data_save->f_face_[2]						= (*p_result)[15].get_float();

	p_role_data_save->e_class_						= (EClassType)(*p_result)[16].get_int();
	p_role_data_save->e_class_ex_						= (EClassTypeEx)(*p_result)[17].get_int();
	p_role_data_save->n_level_						= (*p_result)[18].get_short();
	p_role_data_save->n_cur_exp_						= (*p_result)[19].get_int();
	p_role_data_save->n_hp_							= (*p_result)[20].get_int();
	p_role_data_save->n_mp_							= (*p_result)[21].get_int();

	p_role_data_save->n_att_point_					= (*p_result)[22].get_int();
	p_role_data_save->n_talent_point_					= (*p_result)[23].get_int();

	p_role_data_save->n_att_point_add_[ERA_Physique]	= (*p_result)[24].get_int();
	p_role_data_save->n_att_point_add_[ERA_Strength]	= (*p_result)[25].get_int();
	p_role_data_save->n_att_point_add_[ERA_Pneuma]		= (*p_result)[26].get_int();
	p_role_data_save->n_att_point_add_[ERA_InnerForce]	= (*p_result)[27].get_int();
	p_role_data_save->n_att_point_add_[ERA_Technique]	= (*p_result)[28].get_int();
	p_role_data_save->n_att_point_add_[ERA_Agility]	= (*p_result)[29].get_int();

	p_role_data_save->n_injury_						= (*p_result)[30].get_int();
	p_role_data_save->n_brotherhood_					= (*p_result)[31].get_int();
	p_role_data_save->n_knowledge_					= (*p_result)[32].get_int();
	p_role_data_save->n_morality_					= (*p_result)[33].get_int();
	p_role_data_save->n_luck_						= (*p_result)[34].get_int();
	p_role_data_save->n_credit_						= (*p_result)[35].get_int();
	p_role_data_save->n_identity_					= (*p_result)[36].get_int();
	p_role_data_save->n_vip_point_					= (*p_result)[37].get_int();

	p_role_data_save->n16_bag_size_					= (*p_result)[38].get_short();
	p_role_data_save->n_bag_gold_						= (*p_result)[39].get_int();
	p_role_data_save->n_bag_silver_					= (*p_result)[40].get_int();
	p_role_data_save->n_bag_tuanbao_					= (*p_result)[41].get_int();

	p_role_data_save->dw_guild_id					= (*p_result)[42].get_dword();
	//p_role_data_save->nWareGold					= (*p_result)[43].GetInt();
	//p_role_data_save->nWareSilver					= (*p_result)[44].GetInt();
	//p_role_data_save->nWareYuanBao					= (*p_result)[45].GetInt();

	if(!DataTime2DwordTime(p_role_data_const->create_time_,	(*p_result)[46].get_string(), (*p_result)[46].get_length()))
	{
		print_message(_T("file:%s line:%d  DataTime2DwordTime() error RoleNameCRC is  %u  !!!!!\r\n"), 
				_TFILE, _TLINE, p_role_data_const->dw_role_name_crc);
		ASSERT(0);
	}

	DataTime2DwordTime(p_role_data_save->login_time_,	(*p_result)[47].get_string(), (*p_result)[47].get_length());
	DataTime2DwordTime(p_role_data_save->logout_time_,	(*p_result)[48].get_string(), (*p_result)[48].get_length());

	p_role_data_save->n_online_time_					= (*p_result)[49].get_dword();
	p_role_data_save->n_cur_online_time_				= (*p_result)[50].get_dword();

	p_role_data_save->n_rage_						= (*p_result)[51].get_int();
	p_role_data_save->n_endurance_					= (*p_result)[52].get_int();
	p_role_data_save->b_safe_guard_					= (*p_result)[53].get_int();

	//p_role_data_save->CloseSafeGuardTime			= (*p_result)[54].GetDword();
	DataTime2DwordTime(p_role_data_save->close_safe_guard_time_,	(*p_result)[54].get_string(), (*p_result)[54].get_length());

	p_role_data_save->talent_[0].eType				= (ETalentType)(*p_result)[55].get_int();
	p_role_data_save->talent_[1].eType				= (ETalentType)(*p_result)[56].get_int();
	p_role_data_save->talent_[2].eType				= (ETalentType)(*p_result)[57].get_int();
	p_role_data_save->talent_[3].eType				= (ETalentType)(*p_result)[58].get_int();
	p_role_data_save->talent_[0].nPoint				= (*p_result)[59].get_int();
	p_role_data_save->talent_[1].nPoint				= (*p_result)[60].get_int();
	p_role_data_save->talent_[2].nPoint				= (*p_result)[61].get_int();
	p_role_data_save->talent_[3].nPoint				= (*p_result)[62].get_int();

	p_role_data_save->dw_reborn_map_id_				= (*p_result)[63].get_dword();
	p_role_data_save->n_total_tax_					= (*p_result)[64].get_int();
	p_role_data_save->n_morale_						= (*p_result)[65].get_int();

	DataTime2DwordTime(*(tagDWORDTime*)&p_role_data_save->dw_time_get_mall_free_, (*p_result)[66].get_string(), (*p_result)[66].get_length());
	p_role_data_save->s_remote_open_set_				= (*p_result)[67].get_dword();
	p_role_data_save->n_ex_volume_					= (*p_result)[69].get_int();
	(*p_result)[70].get_blob(p_role_data_save->data_.dwData, (*p_result)[70].get_length());
	p_role_data_save->n_treasure_sum_					= (*p_result)[71].get_int();

	p_role_data_save->by_stall_level_					= (*p_result)[72].get_byte();
	p_role_data_save->n_stall_daily_exp_				= (*p_result)[73].get_int();
	p_role_data_save->n_stall_cur_exp_					= (*p_result)[74].get_int();
	p_role_data_save->dw_last_stall_time_				= (*p_result)[75].get_dword();
	p_role_data_save->n_pk_value_						= (*p_result)[76].get_int();
	p_role_data_save->dw_team_id_						= (*p_result)[77].get_dword();
	DataTime2DwordTime(p_role_data_save->leave_guild_time_,	(*p_result)[78].get_string(), (*p_result)[78].get_length());
	p_role_data_save->n_send_mail_num_					= (*p_result)[79].get_int();
	p_role_data_save->dw_master_id_ = (*p_result)[80].get_dword();
	p_role_data_save->dw_master_prentice_forbid_time_ = (*p_result)[81].get_dword();
	p_role_data_save->n_map_limit_num_ = (*p_result)[82].get_int();
	p_role_data_save->dw_own_instance_id_ = (*p_result)[83].get_dword();
	p_role_data_save->dw_own_instance_map_id_ = (*p_result)[84].get_dword();
	DataTime2DwordTime(p_role_data_save->dw_instance_create_time_, (*p_result)[85].get_string(), (*p_result)[85].get_length());
	p_role_data_save->n16_hang_num_ = (*p_result)[86].get_int();
	p_role_data_save->b_exp_ = (*p_result)[87].get_int();
	p_role_data_save->b_brotherhood_ = (*p_result)[88].get_int();
	p_role_data_save->n_leave_exp_ = (*p_result)[89].get_int();
	p_role_data_save->n_leave_brotherhood_ = (*p_result)[90].get_int();
	p_role_data_save->n_vigour_ = (*p_result)[91].get_int();
	p_role_data_save->n_bag_copper_ = (*p_result)[92].get_int();
	p_role_data_save->n16_pet_packet_size_ = (*p_result)[93].get_int();
	(*p_result)[94].get_blob(p_role_data_save->by_role_help_, (*p_result)[94].get_length());
	(*p_result)[95].get_blob(p_role_data_save->by_talk_data_, (*p_result)[95].get_length());
	(*p_result)[96].get_blob(&p_role_data_save->st_key_info_, (*p_result)[96].get_length());
	p_role_data_save->n_kill_num_ = (*p_result)[97].get_int();
	p_role_data_save->n_gift_step_ = (*p_result)[98].get_short();
	p_role_data_save->dw_gift_id_ = (*p_result)[99].get_dword();
	p_role_data_save->dw_gift_leave_time_ = (*p_result)[100].get_dword();
	p_role_data_save->b_gift_get_ = (BOOL)(*p_result)[101].get_short();
	p_role_data_save->n_gift_id_ = (*p_result)[102].get_short();
	p_role_data_save->n_total_mater_moral_ = (*p_result)[103].get_int();
	p_role_data_const->avatar.byVisualize = (*p_result)[104].get_byte();
	p_role_data_save->n_bag_bind_gold_ = (*p_result)[105].get_int();
	p_role_data_save->n_bag_bind_silver_ = (*p_result)[106].get_int();
	p_role_data_save->n_bag_bind_copper_ = (*p_result)[107].get_int();
	p_role_data_save->e_role_camp = (ECamp)(*p_result)[108].get_short();
	p_role_data_save->n_paimai_limit = (*p_result)[109].get_int();
	p_role_data_save->n_bank_limit = (*p_result)[110].get_int();
	p_role_data_save->dw_today_online_tick_ = (*p_result)[111].get_dword();
	p_role_data_save->dw_history_vigour_cost_ = (*p_result)[112].get_dword();
	p_role_data_save->n16_exbag_step = (*p_result)[113].get_short();
	p_role_data_save->n16_exware_step = (*p_result)[114].get_short();
	p_role_data_save->n16WareSize = (*p_result)[115].get_int();
	p_role_data_save->nWareGold = (*p_result)[116].get_int();
	p_role_data_save->nWareSilver = (*p_result)[117].get_int();
	p_role_data_save->nWareCopper = (*p_result)[118].get_int();
	
	(*p_result)[119].get_tchar(p_role_data_save->sz_signature_name, X_SHORT_NAME);
	
	// 环随机任务
	if((*p_result)[120].get_length()) 
		(*p_result)[120].get_blob(&p_role_data_save->circle_quest_man_, (*p_result)[120].get_length());

	p_role_data_save->n_yuanbao_exchange_num = (*p_result)[121].get_int();
	
	p_role_data_save->n_achievement_point = (*p_result)[122].get_int();

	p_role_data_save->dw_forbid_talk_start_ = (*p_result)[123].get_dword();
	p_role_data_save->dw_forbid_talk_end_ = (*p_result)[124].get_dword();
	p_role_data_save->n_graduates_num = (*p_result)[125].get_int();

	p_role_data_save->dw_destory_equip_count = (*p_result)[126].get_dword();

	p_role_data_save->st_1v1_score.n_cur_score = (*p_result)[127].get_int();
	p_role_data_save->st_1v1_score.n_day_max_score = (*p_result)[128].get_int();
	p_role_data_save->st_1v1_score.n_day_scroe_num = (*p_result)[129].get_int();
	p_role_data_save->st_1v1_score.n16_score_award = (*p_result)[130].get_short();

	p_role_data_save->n32_exploits = (*p_result)[131].get_int();
	p_role_data_save->n_circle_quest_fresh = (*p_result)[132].get_int();

	p_role_data_save->n_shop_exploits_limit = (*p_result)[133].get_int();

	p_role_data_save->n32_active_num = (*p_result)[134].get_int();
	if((*p_result)[135].get_length())
		(*p_result)[135].get_blob(&p_role_data_save->n32_active_data, (*p_result)[135].get_length());
	if((*p_result)[136].get_length())
		(*p_result)[136].get_blob(&p_role_data_save->b_active_receive, (*p_result)[136].get_length());

	p_role_data_save->n32_rating = (*p_result)[137].get_int();
	p_role_data_save->b_PurpureDec = (*p_result)[138].get_bool();
	p_role_data_save->n_circle_quest_perdaynum = (*p_result)[139].get_int();
	
	(*p_result)[140].get_blob(p_role_data_save->by_role_day_clear, (*p_result)[140].get_length());

	p_role_data_save->n_CoolDownReviveCD = (*p_result)[141].get_int();
	p_role_data_save->n_circle_quest_freshMax = (*p_result)[142].get_int();
	p_role_data_save->n_PerDayHangGetExpTimeMS = (*p_result)[143].get_int();
	
	p_role_data_save->n16_pet_xiulian_size= (*p_result)[144].get_int();
	p_role_data_save->dw_perday_get_vigour_total = (*p_result)[148].get_dword();
	p_role_data_save->n_god_level_				 = (*p_result)[149].get_short();

	p_role_data_save->n32_guild_active_num = (*p_result)[145].get_int();
	if((*p_result)[146].get_length())
		(*p_result)[146].get_blob(&p_role_data_save->n32_guild_active_data, (*p_result)[146].get_length());
	if((*p_result)[147].get_length())
		(*p_result)[147].get_blob(&p_role_data_save->b_guild_active_receive, (*p_result)[147].get_length());
	

	p_role_data_save->u16_active_title_id[0]				= (*p_result)[68].get_int();
	p_role_data_save->u16_active_title_id[1]				= (*p_result)[150].get_int();
	p_role_data_save->u16_active_title_id[2]				= (*p_result)[151].get_int();

	p_role_data_save->nInstanceData = (*p_result)[152].get_dword();
	p_role_data_save->nInstanceShaodang = (*p_result)[153].get_dword();
	p_role_data_save->nSaodangIndex = (*p_result)[154].get_dword();
	p_role_data_save->dwSpouseID = (*p_result)[155].get_dword();

	//gx add vip
	p_role_data_save->dwVIPLevel = (*p_result)[156].get_int();
	DataTime2DwordTime(p_role_data_save->dwVIP_Deadline, (*p_result)[157].get_string(), (*p_result)[157].get_length());
	p_role_data_save->dw_shihun = (*p_result)[158].get_dword();
	// 该字段在读取角色具体信息时不用读取
	//ZeroMemory(&p_role_data_save->DisplaySet, sizeof(tagDisplaySet));
	//if(!(*p_result)[2].IsNull())
	//{
	//	(*p_result)[2].GetBlob(&p_role_data_save->DisplaySet, (*p_result)[2].GetLen());
	//}
}

// 获取角色技能
BOOL db_handler::load_one_role_skill(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_skill), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_skill));
}

VOID db_handler::makesql_load_one_role_skill(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("skill", "id,bidden_level,self_level,cooldown,proficiency");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_skill(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_skill, p_buffer, s_skill_save);
	s_skill_save * p_skill = ( s_skill_save * ) p_buffer ; 

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		p_skill[i].dw_id			= (*p_result)[0].get_dword();
		p_skill[i].n_self_level_	= (*p_result)[2].get_int();
		p_skill[i].n_learn_level_	= (*p_result)[1].get_int();
		p_skill[i].n_cool_down_	    = (*p_result)[3].get_int();
		p_skill[i].n_proficiency_	= (*p_result)[4].get_int();

		p_result->next_row();
	}

	p_buffer = &p_skill[*p_number];
}

// 获取角色buff
BOOL db_handler::load_one_role_buff(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_buff), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_buff));
}

VOID db_handler::makesql_load_one_role_buff(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("buff", 
		"src_unit_id,"		"src_skill_id,"			"item_type_id,"		"item_serial_id,"		"buff_id,"
		"cur_tick,"			"level,"				"cur_lap_times,"		"effect_skill_id");

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_buff(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_current, p_buffer, BYTE);
	BYTE * p_current = ( BYTE * ) p_buffer ;  

	s_buff_save *p_role_save;
	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		p_role_save = (s_buff_save*)p_current;

		p_role_save->dw_src_unit_id_			= (*p_result)[0].get_dword();
		p_role_save->dw_src_skill_id_			= (*p_result)[1].get_dword();
		p_role_save->dw_item_type_id_			= (*p_result)[2].get_dword();
		p_role_save->n_serial_			= (*p_result)[3].get_long();

		p_role_save->dw_buff_id_				= (*p_result)[4].get_dword();
		p_role_save->n_persist_tick_			= (*p_result)[5].get_int();
		p_role_save->n_level_				= (INT8)(*p_result)[6].get_int();
		p_role_save->n_cur_lap_times_		= (INT8)(*p_result)[7].get_int();


		p_role_save->n_modifier_num_		= (INT8)((*p_result)[8].get_length()/sizeof(DWORD));

		p_current = p_role_save->by_data_;
		if(p_role_save->n_modifier_num_ > 0)
		{
			(*p_result)[8].get_blob(p_current, (*p_result)[8].get_length());
			p_current += (*p_result)[8].get_length();
		}

		p_result->next_row();
	}

	p_buffer = p_current;
}

// 获取进图限制
BOOL db_handler::load_one_role_map_limit(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_map_limit), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_map_limit));
}

VOID db_handler::makesql_load_one_role_map_limit(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("map_limit",
		"map_id,"	"enter_num," "type");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_map_limit(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_current, p_buffer, BYTE);
	BYTE * p_current = ( BYTE * ) p_buffer ;  

	s_enter_map_limit* p_enter_map_limit;
	*p_number = p_result->get_row_count();

	for(int i = 0; i < *p_number; i++)
	{
		p_enter_map_limit = (s_enter_map_limit*)p_current;

		p_enter_map_limit->dw_map_id_ = (*p_result)[0].get_dword();
		p_enter_map_limit->dw_enter_num_ = (*p_result)[1].get_dword();
		p_enter_map_limit->e_enter_limit_ = (*p_result)[2].get_int();

		p_current += sizeof(s_enter_map_limit);

		p_result->next_row();
	}

	p_buffer = p_current;
}

// 获取角色称号
BOOL db_handler::load_one_role_title(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_title), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_title));
}

VOID db_handler::makesql_load_one_role_title(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("title", "id,count,time");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_title(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	*p_number = p_result->get_row_count();

	//M_trans_pointer(p, p_buffer, s_title_save);
	s_title_save * p = ( s_title_save * ) p_buffer ;  

	for(int i=0; i<*p_number; ++i)
	{
		p[i].n_title_id_		= (*p_result)[0].get_dword();
		p[i].dw_state_mark_	= (*p_result)[1].get_dword();
		
		tagDWORDTime time;
		DataTime2DwordTime(time, (*p_result)[2].get_string(), (*p_result)[2].get_length());
		p[i].dw_time	= time;

		p_result->next_row();
	}

	p_buffer = &p[*p_number];
}

// 获取完成成就
BOOL db_handler::load_complate_achievement(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_complate_achievement), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_complate_achievement));
}

VOID db_handler::makesql_load_complate_achievement(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("achievement", "id,date");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_complate_achievement(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	*p_number = p_result->get_row_count();

	s_achievement_complate_save * p = ( s_achievement_complate_save * ) p_buffer ;  

	for(int i=0; i<*p_number; ++i)
	{
		p[i].dw_id		= (*p_result)[0].get_dword();

		tagDWORDTime time;
		DataTime2DwordTime(time, (*p_result)[1].get_string(), (*p_result)[1].get_length());
		
		p[i].dw_date	= time;

		p_result->next_row();
	}

	p_buffer = &p[*p_number];
}

// 获取副本进度
BOOL db_handler::load_process_instance(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_inst_process), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_inst_process));
}

VOID db_handler::makesql_load_one_role_inst_process(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("inst_process", "map_id, mode, door_process, creature_process,type");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_inst_process(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	*p_number = p_result->get_row_count();

	s_inst_process* p = (s_inst_process*)p_buffer;

	for(INT i = 0; i < *p_number; i++)
	{
		p[i].dw_map_id = (*p_result)[0].get_dword();
		p[i].n_mode = (*p_result)[1].get_int();
		(*p_result)[2].get_blob(p[i].st_door_state, sizeof(p[i].st_door_state));
		(*p_result)[3].get_blob(p[i].dw_creature_id, sizeof(p[i].dw_creature_id));
		p[i].n_type = (*p_result)[4].get_short();

		p_result->next_row();
	}

	p_buffer = &p[*p_number];
}

// 获取签到数据
BOOL db_handler::load_sign_data(LPVOID &p_buffer, DWORD dw_role_id)
{
	int n_count = 0;
	BOOL b_ret = common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_role_sign_data),
		MakeDelegateOfDBHandler(&db_handler::procres_load_role_sign_data));
	if (1 != n_count)
	{
		insert_sign_data(dw_role_id);
		b_ret = common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
			MakeDelegateOfDBHandler(&db_handler::makesql_load_role_sign_data), 
			MakeDelegateOfDBHandler(&db_handler::procres_load_role_sign_data));
		ASSERT(1 == n_count);
	}
	return b_ret;
}

VOID db_handler::makesql_load_role_sign_data(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("sign", "data, reward_data, mianqian_time");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_role_sign_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	*p_number = p_result->get_row_count();

	tagRoleSignData* p = (tagRoleSignData*)p_buffer;

	
	(*p_result)[0].get_blob(p->st_sign_info, sizeof(p->st_sign_info));
	(*p_result)[1].get_blob(p->b_reward, sizeof(p->b_reward));
	p->n16_mianqian_time = (*p_result)[2].get_int();
	
	p_buffer = &p[*p_number];
}

BOOL db_handler::load_huenjing_data(LPVOID &p_buffer, DWORD dw_role_id)
{
	int n_count = 0;
	BOOL b_ret = common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_role_huenjing_data),
		MakeDelegateOfDBHandler(&db_handler::procres_load_role_huenjing_data));
	if (1 != n_count)
	{
		insert_huenjing_data(dw_role_id);
		b_ret = common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
			MakeDelegateOfDBHandler(&db_handler::makesql_load_role_huenjing_data), 
			MakeDelegateOfDBHandler(&db_handler::procres_load_role_huenjing_data));
		ASSERT(1 == n_count);
	}
	return b_ret;
}

VOID db_handler::makesql_load_role_huenjing_data(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("HuenJing", "bag_temp_data, bag_data, role_level_data, role_title_data, cur_artisan");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_role_huenjing_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	*p_number = p_result->get_row_count();

	tagRoleHuenJingData* p = (tagRoleHuenJingData*)p_buffer;


	(*p_result)[0].get_blob(p->s_huenjing_bag_temp, sizeof(p->s_huenjing_bag_temp));
	(*p_result)[1].get_blob(p->s_huenjing_bag, sizeof(p->s_huenjing_bag));
	(*p_result)[2].get_blob(p->s_huenjing_role_level, sizeof(p->s_huenjing_role_level));
	(*p_result)[3].get_blob(p->s_huenjing_role_titel, sizeof(p->s_huenjing_role_titel));
	p->byCurArtisan = (*p_result)[4].get_int();

	p_buffer = &p[*p_number];
}



BOOL db_handler::load_reward_data( LPVOID &p_buffer, DWORD dw_role_id )
{
	int n_count = 0;
	BOOL b_ret = common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_role_reward_data),
		MakeDelegateOfDBHandler(&db_handler::procres_load_role_reward_data));
	if (1 != n_count)
	{
		insert_reward_data(dw_role_id);
		b_ret = common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
			MakeDelegateOfDBHandler(&db_handler::makesql_load_role_reward_data), 
			MakeDelegateOfDBHandler(&db_handler::procres_load_role_reward_data));
		ASSERT(1 == n_count);
	}
	return b_ret;
}


VOID db_handler::makesql_load_role_reward_data(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("reward", "reward_data");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_role_reward_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	*p_number = p_result->get_row_count();

	tagRewardData* p = (tagRewardData*)p_buffer;

	(*p_result)[0].get_blob(p, sizeof(tagRewardData) * MAX_REWARD_NUMBER * RF_NUM);

	p_buffer = &p[*p_number * MAX_REWARD_NUMBER * RF_NUM];
}

BOOL db_handler::load_mounts_data( LPVOID &p_buffer, DWORD dw_role_id )
{
	int n_count = 0;
	BOOL b_ret = common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_role_mounts_data),
		MakeDelegateOfDBHandler(&db_handler::procres_load_role_mounts_data));
	if (1 != n_count)
	{
		insert_mounts_data(dw_role_id);
		b_ret = common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
			MakeDelegateOfDBHandler(&db_handler::makesql_load_role_mounts_data), 
			MakeDelegateOfDBHandler(&db_handler::procres_load_role_mounts_data));
		ASSERT(1 == n_count);
	}
	return b_ret;
}

VOID db_handler::makesql_load_role_mounts_data(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("mounts", "step, grade, exp");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_role_mounts_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	*p_number = p_result->get_row_count();

	s_mounts_save* p = (s_mounts_save*)p_buffer;

	p->nStep = (*p_result)[0].get_int();
	p->nGrade = (*p_result)[1].get_int();
	p->nExp = (*p_result)[2].get_int();

	p_buffer = &p[*p_number];
}


// 获取成就进度
BOOL db_handler::load_progress_achievement(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_progress_achievement), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_progress_achievement));
}

VOID db_handler::makesql_load_progress_achievement(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("achievement_progress", "id,counter");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_progress_achievement(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	*p_number = p_result->get_row_count();

	s_achievement_process_save * p = ( s_achievement_process_save * ) p_buffer ;  

	for(int i=0; i<*p_number; ++i)
	{
		p[i].dw_id		= (*p_result)[0].get_dword();
		p[i].dw_count	= (*p_result)[1].get_dword();

		p_result->next_row();
	}

	p_buffer = &p[*p_number];
}

// 读取序列号礼包
BOOL db_handler::load_serial_reward(LPVOID p_buffer, DWORD dw_account_id)
{
	/*return common_select(p_buffer, NULL, dw_account_id, INVALID_VALUE,
	MakeDelegateOfDBHandler(&db_handler::makesql_load_serial_reward),
	MakeDelegateOfDBHandler(&db_handler::procres_load_serial_reward));*/
	NET_DB2C_load_serial_reward* p_data = (NET_DB2C_load_serial_reward*)p_buffer;

	BOOL bRet = TRUE;
	INT nNum = 0;
	BOOL b_exists = FALSE;

	odbc::sql_language_disposal* pStream = game_db_interface_->get_io();
	if(!pStream)
	{
		return FALSE;
	}

	odbc::tag_mysql_connect* pCon = game_db_interface_->get_idlesse_connect();

	pStream->clear();
	pStream->select_item("serial_reward", "type");
	pStream->where_item();
	pStream->write_string("serial='").write_string(p_data->sz_serial, pCon).write_string("'");

	game_db_interface_->return_use_connect(pCon);

	odbc::execute_result* pResult = game_db_interface_->sql_query(pStream);

	if(!pResult)
	{
		bRet = FALSE;
		goto exit;
	}

	nNum = pResult->get_row_count() ;

	if(nNum > 0)
	{
		b_exists = TRUE;
	}

	NET_DB2S_load_serial_reward send;
	send.b_exists = b_exists;
	send.dw_role_id = p_data->dw_role_id;
	strncpy(send.sz_serial, p_data->sz_serial, X_SHORT_NAME);
	send.n_type = (*pResult)[0].get_short();
	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

exit:
	game_db_interface_->return_io(pStream);
	game_db_interface_->free_result_query(pResult);

	return bRet;

}

// 获取角色称号
BOOL db_handler::load_one_role_items(LPVOID &p_buffer, int &n_count, DWORD dw_account_id, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, dw_account_id, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_items), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_items));
}
VOID db_handler::makesql_load_one_role_items(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->write_string("select ");
	init_item_load_field(p_stream);
	p_stream->write_string(" from item left join equip using(serial)");

	p_stream->where_item();
	/*p_stream->write_string("owner_id=") << dw_role_id;
	p_stream->write_string(" or (account_id=") << dw_account_id;
	p_stream->write_string(" and (container_type_id=") << EICT_Baibao;
	p_stream->write_string(" or container_type_id=") << EICT_RoleWare;
	p_stream->write_string("))");*/
	p_stream->write_string("(owner_id=") << dw_role_id;
	p_stream->write_string(" and (container_type_id >=") << EICT_Bag;
	p_stream->write_string(" or container_type_id <=") << EICT_Equip;
	p_stream->write_string("))");
}

// 获取角色好友
BOOL db_handler::load_one_role_friends(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_friends), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_friends));
}

VOID db_handler::makesql_load_one_role_friends(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("friend", "friend_id,group_id");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_friends(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p, p_buffer, s_friend_save);
	s_friend_save * p = ( s_friend_save * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	if(*p_number > MAX_FRIENDNUM)
	{
		// 应考虑容错处理
		ASSERT(*p_number <= MAX_FRIENDNUM);
		*p_number = MAX_FRIENDNUM; 
	}

	for(int i=0; i<*p_number; ++i)
	{
		p[i].dw_friend_id_ = (*p_result)[0].get_dword();
		p[i].n_group_id_	= (*p_result)[1].get_int();

		p_result->next_row();
	}

	p_buffer = &p[*p_number];
}

// 获取角色仇人
BOOL db_handler::load_one_role_enemies(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_enemies), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_enemies));
}

VOID db_handler::makesql_load_one_role_enemies(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("enemy", "enemy_id");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_enemies(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_enemies, p_buffer, DWORD);
	DWORD * p_enemies = ( DWORD * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	if(*p_number > MAX_ENEMYNUM)
	{
		// 应考虑容错处理
		ASSERT(*p_number <= MAX_ENEMYNUM);
		*p_number = MAX_ENEMYNUM; 
	}

	for(int i=0; i<*p_number; ++i)
	{
		p_enemies[i] = (*p_result)[0].get_dword();

		p_result->next_row();
	}

	p_buffer = &p_enemies[*p_number];
}

// 获取角色未完成任务
BOOL db_handler::load_one_role_quest(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_task), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_quest));
}

VOID db_handler::makesql_load_one_role_task(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("quest", "quest_id,start_time,quest_flag,monster0_num,monster1_num,monster2_num,"
		"monster3_num,monster4_num,monster5_num,script_data, dynamic_target, dynamic_reward, scriptQuest_var,new_flag");//gx add "new_flag" 2013.12.05
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_quest(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_quest, p_buffer, tagQuestSave);
	tagQuestSave * p_quest = ( tagQuestSave * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	if(*p_number > QUEST_MAX_COUNT)
	{
		// 应考虑容错处理
		ASSERT(*p_number <= QUEST_MAX_COUNT);
		*p_number = QUEST_MAX_COUNT;
	}

	for(int i=0; i<*p_number; ++i)
	{
		p_quest[i].u16QuestID	= (UINT16)(*p_result)[0].get_dword();
		p_quest[i].dwAcceptTime	= (*p_result)[1].get_dword();
		p_quest[i].dwQuestFlag	= (*p_result)[2].get_dword();

		for(int j=0; j<QUEST_CREATURES_COUNT; ++j)
		{
			p_quest[i].n16MonsterNum[j] = (INT16)(*p_result)[j+3].get_int();
		}

		(*p_result)[QUEST_CREATURES_COUNT+3].get_blob(p_quest[i].Data.dwData, (*p_result)[QUEST_CREATURES_COUNT+3].get_length());
		if((*p_result)[QUEST_CREATURES_COUNT+4].get_length() > 0)
			(*p_result)[QUEST_CREATURES_COUNT+4].get_blob(&p_quest[i].DynamicTarget, (*p_result)[QUEST_CREATURES_COUNT+4].get_length());

		if((*p_result)[QUEST_CREATURES_COUNT+5].get_length() > 0)
			(*p_result)[QUEST_CREATURES_COUNT+5].get_blob(&p_quest[i].DynamicReward, (*p_result)[QUEST_CREATURES_COUNT+5].get_length());

		if((*p_result)[QUEST_CREATURES_COUNT+6].get_length() > 0)
			(*p_result)[QUEST_CREATURES_COUNT+6].get_blob(&p_quest[i].ScriptQuestVar, (*p_result)[QUEST_CREATURES_COUNT+6].get_length());
		
		p_quest[i].u16Quest_NewFlag = (*p_result)[QUEST_CREATURES_COUNT+7].get_int();//gx add

		p_result->next_row();
	}

	p_buffer = &p_quest[*p_number];
}

// 获取角色完成任务
BOOL db_handler::load_one_role_quest_done(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_quest_done), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_quest_done));
}
VOID db_handler::makesql_load_one_role_quest_done(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("quest_done", "quest_id,times,start_time,new_flag");//gx add "new_flag" 2013.12.05
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_quest_done(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_quest, p_buffer, tagQuestDoneSave);
	tagQuestDoneSave * p_quest = ( tagQuestDoneSave * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		p_quest[i].u16QuestID = (UINT16)(*p_result)[0].get_dword();
		p_quest[i].dwStartTime = (*p_result)[2].get_dword();
		p_quest[i].nTimes = (*p_result)[1].get_int();
		p_quest[i].u16QuestFalg = (*p_result)[3].get_int();//gx add
		p_result->next_row();
	}

	p_buffer = &p_quest[*p_number];
}

// 获取完成任务简单数据
BOOL db_handler::load_one_role_simple_quest_done(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_simple_quest_done), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_simple_quest_done));
}

VOID db_handler::makesql_load_one_role_simple_quest_done(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("quest_done", "quest_id");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_simple_quest_done(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_quest, p_buffer, tagQuestDoneSimSave);
	tagQuestDoneSimSave * p_quest = ( tagQuestDoneSimSave * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		p_quest[i].u16QuestID = (UINT16)(*p_result)[0].get_dword();

		p_result->next_row();
	}

	p_buffer = &p_quest[*p_number];
}

// 获取道具CD时间
BOOL db_handler::load_one_role_item_cd(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_item_cd), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_item_cd));
}
VOID db_handler::makesql_load_one_role_item_cd(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("item_cd_time", "cd_time");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_item_cd(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	if((*p_result)[0].is_null())
	{
		*p_number = 0;
		return;
	}

	*p_number = (*p_result)[0].get_length() / sizeof(tagCDTime);
	(*p_result)[0].get_blob(p_buffer, (*p_result)[0].get_length());	

	p_buffer = (BYTE*)p_buffer + (*p_result)[0].get_length();
}

// 获取角色友好度
BOOL db_handler::load_one_role_friendship(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_friendship), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_frienship));
}

BOOL db_handler::load_one_role_friendship_ex(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_friendship_ex), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_frienship));
}

VOID db_handler::makesql_load_one_role_friendship(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("friendship", "friend_id,friend_val");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::makesql_load_one_role_friendship_ex(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("friendship", "role_id,friend_val");
	p_stream->where_item();
	p_stream->write_string("friend_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_frienship(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p, p_buffer, s_friendship_save);
	s_friendship_save * p = ( s_friendship_save * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	if(*p_number > MAX_FRIENDNUM)
	{
		// 应考虑容错处理
		ASSERT(*p_number <= MAX_FRIENDNUM);
		*p_number = MAX_FRIENDNUM; 
	}

	for(int i=0; i<*p_number; ++i)
	{
		p[i].dw_friend_id_ = (*p_result)[0].get_dword();
		p[i].n_frival_	= (*p_result)[1].get_int();

		p_result->next_row();
	}

	p_buffer = &p[*p_number];
}

// 获取黑名单
BOOL db_handler::load_one_role_black(LPVOID &p_buffer, int &n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_black), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_black));
}

VOID db_handler::makesql_load_one_role_black(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("blacklist", "black_id");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_black(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p, p_buffer, DWORD);
	DWORD * p = ( DWORD * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	if(*p_number > MAX_BLACKLIST)
	{
		// 应考虑容错处理
		ASSERT(*p_number <= MAX_BLACKLIST);
		*p_number = MAX_BLACKLIST;
	}

	for(int i=0; i<*p_number; ++i)
	{
		p[i] = (*p_result)[0].get_dword();

		p_result->next_row();
	}

	p_buffer = &p[*p_number];
}


// 获取角色名片
BOOL db_handler::load_visiting_card(LPVOID &p_buffer, DWORD dw_role_id)
{
	int n_count = 0;
	BOOL b_ret = common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_visiting_card), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_visiting_card));
	return b_ret;
}
VOID db_handler::makesql_load_visiting_card(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("visiting_card", "level,job,mate_role_id,guild_id,position,visibility,"
		"sex,constellation,chinesezodiac,area,province,city,head_url,signature, role_id");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}
VOID db_handler::procres_load_visiting_card(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p, p_buffer, tagVCardData);
	tagVCardData * p = ( tagVCardData * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	ASSERT(1 == *p_number);

	p->dwLevel			= (*p_result)[0].get_int();
	p->dwJob			= (*p_result)[1].get_int();
	p->dwMateRoleID		= (*p_result)[2].get_int();
	p->dwFactionID		= (*p_result)[3].get_int();
	p->dwPosition		= (*p_result)[4].get_int();

	//p->customVCardData.bVisibility		= (*p_result)[5].get_bool();
	//p->customVCardData.bySex			= (*p_result)[6].get_byte();
	p->customVCardData.byConstellation	= (*p_result)[7].get_byte();
	//p->customVCardData.byChineseZodiac	= (*p_result)[8].get_byte();
	//p->customVCardData.byArea			= (*p_result)[9].get_byte();
	//p->customVCardData.byProvince		= (*p_result)[10].get_byte();
	(*p_result)[11].get_tchar(p->customVCardData.chCity, LEN_CITY_NAME);
	//p->customVCardData.byHeadUrlSize	= (*p_result)[12].get_tchar(reinterpret_cast<TCHAR *>(p->customVCardData.byData), 200) * sizeof(TCHAR);
	p->customVCardData.bySignatureSize	= (*p_result)[13].get_tchar(reinterpret_cast<TCHAR *>(&(p->customVCardData.byData)), 200) * sizeof(TCHAR);

	p->dw_role_id		= (*p_result)[14].get_int();

	DWORD dwVCardSize = sizeof(tagVCardData) - 1 /*+ p->customVCardData.byHeadUrlSize */+ p->customVCardData.bySignatureSize;

	p_buffer = reinterpret_cast<BYTE *>(p_buffer) + dwVCardSize;

}
// 获取角色家族数据
BOOL db_handler::load_clan_data(LPVOID &p_buffer, DWORD dw_role_id)
{
	int n_count = 0;
	BOOL b_ret = common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_clan_data),
		MakeDelegateOfDBHandler(&db_handler::procres_load_clan_data));
	if (1 != n_count)
	{
		insert_clan_data(dw_role_id);
		b_ret = common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
			MakeDelegateOfDBHandler(&db_handler::makesql_load_clan_data), 
			MakeDelegateOfDBHandler(&db_handler::procres_load_clan_data));
		ASSERT(1 == n_count);
	}
	return b_ret;
}

VOID db_handler::makesql_load_clan_data(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("clan_data", 
		"rep_xuanyuan,con_xuanyuan,act_cntxuanyuan,"
		"rep_shennong,con_shennong,actcnt_shennong,"
		"rep_fuxi,con_fuxi,actcnt_fuxi,"
		"rep_sanmiao,con_sanmiao,actcnt_sanmiao,"
		"rep_jiuli,con_jiuli,actcnt_jiuli,"
		"rep_yuezhi,con_yuezhi,actcnt_yuezhi,"
		"rep_nvwa,con_nvwa,actcnt_nvwa,"
		"rep_gonggong,con_gonggong,actcnt_gonggong,"
		"fame_mask, rep_reset_timestamp");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}




VOID db_handler::procres_load_clan_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);
	//M_trans_pointer(p, p_buffer, s_db_repute_data);
	s_db_repute_data * p = ( s_db_repute_data * ) p_buffer ;  

	*p_number = p_result->get_row_count();
	ASSERT(1 == *p_number);


	for(int nClanType = ECLT_BEGIN; nClanType < ECLT_END; ++nClanType)
	{
		p->n_reputation_[nClanType]		= (*p_result)[nClanType * 3].get_int();
		p->n_contribution_[nClanType]	= (*p_result)[nClanType * 3 + 1].get_int();
		p->n8_act_count_[nClanType]		= (*p_result)[nClanType * 3 + 2].get_byte();

	}
	p->u16_fame_mask	= (*p_result)[ECLT_NUM * 3].get_short();
	p->last_reset_time_= (*p_result)[ECLT_NUM * 3 + 1].get_dword();

	p_buffer = reinterpret_cast<BYTE *>(p_buffer) + sizeof(s_db_repute_data);
}

// 获取百宝物品
BOOL db_handler::load_item_baibao(DWORD dw_account_id, int n_load, int64 n64_serial, int &n_loaded, int &n_data_size, LPVOID p_buffer)
{
	reset_load_count_limit(n_load);
	reset_load_serial(n64_serial);

	LPVOID	p_temp_data = p_buffer;
	BOOL	b_ret = common_select(p_temp_data, &n_loaded, dw_account_id, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_item_baibao), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_items));

	if(b_ret) n_data_size = (int)((BYTE*)p_temp_data - (BYTE*)p_buffer);

	return b_ret;
}

VOID db_handler::makesql_load_item_baibao(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->write_string("select ");
	init_item_load_field(p_stream);
	p_stream->write_string(" from item_baibao left join equip_baibao using(serial)");

	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
	if (VALID_VALUE(n64_load_baibao_item_serial))
	{
		p_stream->write_string(" and serial=") << n64_load_baibao_item_serial;
	}
	p_stream->limit(n_load_count_limit_);
}
// 获取百宝元宝
BOOL db_handler::load_baibao_yuanbao(DWORD dw_account_id, int& n_yuanbao)
{
	LPVOID p_buffer = (LPVOID)&n_yuanbao;
	return common_select(p_buffer, NULL, dw_account_id, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_baibao_yuanbao), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_baibao_yuanbao));
}
VOID db_handler::makesql_load_baibao_yuanbao(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("account_common", "yuanbao_recharge");
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}
VOID db_handler::procres_load_baibao_yuanbao(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int* n_yuanbao = (int*)p_buffer;

	int n_count	= p_result->get_row_count();
	*n_yuanbao	= 0;

	if (n_count != 0)
	{
		*n_yuanbao = (*p_result)[0].get_int();
	}
}

// 获取网页领奖标志
BOOL db_handler::load_web_receive(DWORD dw_account_id, DWORD& dw_web_receive)
{
	LPVOID p_buffer = (LPVOID)&dw_web_receive;
	return common_select(p_buffer, NULL, dw_account_id, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_web_receive), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_web_receive));
}

VOID db_handler::makesql_load_web_receive(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("account_common", "web_type");
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}

VOID db_handler::procres_load_web_receive(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	DWORD* dw_web_receive = (DWORD*)p_buffer;

	int n_count	= p_result->get_row_count();
	*dw_web_receive	= 0;

	if (n_count != 0)
	{
		*dw_web_receive = (*p_result)[0].get_dword();
	}
}

// 更新元宝充值
BOOL  db_handler::update_yuanbao_recharge(DWORD dw_account_id, INT32 n32_yuanbao)
{
	return common_update(dw_account_id, (LPVOID)&n32_yuanbao, 1, 0, MakeDelegateOfDBHandler(&db_handler::makesql_update_yuanbao_recharge));
}

VOID db_handler::makesql_update_yuanbao_recharge(odbc::sql_language_disposal* p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	p_stream->clear();
	p_stream->update_item("account_common");
	p_stream->write_string("yuanbao_recharge = yuanbao_recharge-") << *(INT32*)(p_buffer);
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}

// 获取百宝日志
BOOL db_handler::load_baibao_log_info(DWORD dw_account_id, int &n_loaded, int &n_data_size, LPVOID p_buffer)
{
	LPVOID	p_temp_data = p_buffer;
	BOOL	b_ret = common_select(p_buffer, &n_loaded, dw_account_id, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_baibao_log_info), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_baibao_log_info));

	if(b_ret) n_data_size = (int)((BYTE*)p_buffer - (BYTE*)p_temp_data);

	return b_ret;
}
VOID db_handler::makesql_load_baibao_log_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("baibao_log", "account_id,role_id,log_time,n16_type,type_id,leave_words");
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
}

VOID db_handler::procres_load_baibao_log_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_baibao_log, p_buffer, tagBaiBaoRecord);
	tagBaiBaoRecord * p_baibao_log = ( tagBaiBaoRecord * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	for(INT16 i=0; i<*p_number; ++i)
	{
		p_baibao_log->dw_role_id = (*p_result)[1].get_dword();
		p_baibao_log->dw_data_id = (*p_result)[4].get_dword();
		p_baibao_log->n16Type = (*p_result)[3].get_short();
		DataTime2DwordTime(*(tagDWORDTime*)&(p_baibao_log->dw_time), (*p_result)[2].get_string(), (*p_result)[2].get_length());
		(*p_result)[5].get_blob(p_baibao_log->szWords, (*p_result)[5].get_length());

		p_baibao_log->n16Size = (INT16)(sizeof(tagBaiBaoRecord) + (*p_result)[5].get_length() - sizeof(p_baibao_log->szWords));

		p_baibao_log = (tagBaiBaoRecord*)((BYTE*)p_baibao_log + p_baibao_log->n16Size);
		p_result->next_row();
	}

	p_buffer = p_baibao_log;
}


// 新增百宝日志
BOOL db_handler::insert_baibao_log_info(DWORD dw_account_id, LPVOID p_buffer)
{
	if (!VALID_VALUE(dw_account_id))
		return FALSE;

	// 1.删除超过上限的记录
	common_select(p_buffer, NULL, dw_account_id, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_baibao_log_info),
		MakeDelegateOfDBHandler(&db_handler::procres_count_and_delete_baibao_log));

	// 2.添加新纪录
	return common_update(dw_account_id, p_buffer, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_baibao_log_info));
}
VOID db_handler::makesql_insert_baibao_log_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, tagBaiBaoRecord);
	tagBaiBaoRecord * p = ( tagBaiBaoRecord * ) p_buffer ;  
	// 取得数据库连接
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->insert_item("baibao_log");
	p_stream->write_string("account_id=") << dw_account_id;
	p_stream->write_string(",role_id=") << p->dw_role_id;

	// 格式化时间
	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_time))
	{
		p_stream->write_string(",log_time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}
	else
	{
		print_message(_T("file:%s line:%d  DwordTime2DataTime()  error !!!!!\r\n"), _TFILE, _TLINE);
		ASSERT(0);
	}

	p_stream->write_string(",n16_type=") << p->n16Type;
	p_stream->write_string(",type_id=") << p->dw_data_id;


	//赠言
	p_stream->write_string(",leave_words='");
	int n_len = _tcsnlen(p->szWords, Max_LeaveWord_Length);
	if (n_len > 0)
	{
		p_stream->write_blob(p->szWords, p->n16Size-sizeof(tagBaiBaoRecord)+sizeof(p->szWords), p_conn);
	}
	p_stream->write_string("'");

	// 归还数据库连接
	game_db_interface_->return_use_connect(p_conn);
}
VOID db_handler::makesql_delete_baibao_log_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer)
{
	ASSERT(VALID_VALUE(dw_account_id));

	//M_trans_pointer(p, p_buffer, tagBaiBaoRecord);
	tagBaiBaoRecord * p = ( tagBaiBaoRecord * ) p_buffer ;  

	p_stream->delete_item("baibao_log");
	p_stream->where_item();
	p_stream->write_string("account_id=") << dw_account_id;
	p_stream->write_string(" and role_id=") << p->dw_role_id;
	p_stream->write_string(" and n16_type=") << p->n16Type;
	p_stream->write_string(" and type_id=") << p->dw_data_id;

	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_time))
	{
		p_stream->write_string(" and log_time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}
	else
	{
		print_message(_T("file:%s line:%d  DwordTime2DataTime()  error !!!!!\r\n"), _TFILE, _TLINE);
		ASSERT(0);
	}
}

VOID db_handler::procres_count_and_delete_baibao_log(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int n_count = p_result->get_row_count();

	if(n_count < MAX_BAIBAO_LOG_NUM) return;

	DWORD dw_account_id = INVALID_VALUE;
	tagBaiBaoRecord temp_log_info = {0};
	DWORD temp_log_time;
	temp_log_time = temp_log_info.dw_time = GetCurrentDWORDTime();

	for(int i=0; i<n_count; ++i)
	{
		DataTime2DwordTime((*(tagDWORDTime*)&temp_log_time), (*p_result)[2].get_string(), (*p_result)[2].get_length());
		if (temp_log_time <= temp_log_info.dw_time)
		{
			dw_account_id = (*p_result)[0].get_dword();
			temp_log_info.dw_role_id = (*p_result)[1].get_dword();
			temp_log_info.dw_data_id = (*p_result)[4].get_dword();
			temp_log_info.n16Type = (*p_result)[3].get_short();
			DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), temp_log_time);
			temp_log_info.dw_time = temp_log_time;
		}
		p_result->next_row();
	}

	common_update(dw_account_id, (LPVOID)&temp_log_info, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_baibao_log_info));
}
// 获取需log的item类型
BOOL db_handler::load_item_need_log(LPVOID pNeedLogItem, int &n_item_count)
{
	return common_select(pNeedLogItem, &n_item_count, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_item_need_log), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_item_need_log));
}
VOID db_handler::makesql_load_item_need_log(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("item_needlog", "type_id, min_qlty");
	p_stream->where_item();
	p_stream->write_string("need_log=") << TRUE;
}

VOID db_handler::procres_load_item_need_log(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_type_id, p_buffer, DWORD);
	s_need_log_item * p_type_id = ( s_need_log_item * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	if(*p_number > MAX_ITEM_NEEDLOG_NUM)
	{
		ASSERT(*p_number <= MAX_ITEM_NEEDLOG_NUM);
		*p_number = MAX_ITEM_NEEDLOG_NUM;
	}

	for(int i=0; i<*p_number; ++i)
	{
		p_type_id[i].dw_data_id = (*p_result)[0].get_dword();
		p_type_id[i].by_quality = (*p_result)[1].get_byte();
		p_result->next_row();
	}

	p_buffer = &p_type_id[*p_number];
}
// 读取元宝账户
BOOL db_handler::load_all_yuanbao_account(LPVOID p_buffer, int &n_count)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_yuanbao_account),
		MakeDelegateOfDBHandler(&db_handler::procres_load_all_yuanbao_account));
}
VOID db_handler::makesql_load_all_yuanbao_account(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("yuanbao_account", "role_id,yuanbao,gold,silver,b_sell_order,b_buy_order");
}
VOID db_handler::procres_load_all_yuanbao_account(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_yuanbao_account, p_buffer, tagYBAccount);
	tagYBAccount * p_yuanbao_account = ( tagYBAccount * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		p_yuanbao_account[i].dw_role_id = (*p_result)[0].get_dword();
		p_yuanbao_account[i].nYuanBao = (*p_result)[1].get_dword();
		p_yuanbao_account[i].nGold = (*p_result)[2].get_int();
		p_yuanbao_account[i].nSilver = (*p_result)[3].get_int();
		p_yuanbao_account[i].bSellOrder = (*p_result)[4].get_bool();
		p_yuanbao_account[i].bBuyOrder = (*p_result)[5].get_bool();

		p_result->next_row();
	}
}

// 读取元宝订单
BOOL db_handler::load_all_yuanbao_order(LPVOID p_buffer, int &n_count)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_yuanbao_order),
		MakeDelegateOfDBHandler(&db_handler::procres_load_all_yuanbao_order));
}

VOID db_handler::makesql_load_all_yuanbao_order(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("yuanbao_order", "id,role_id,order_type,price,number,"
			"order_mode,start_time,end_time,avg_price,deal_num");
	p_stream->where_item();
	p_stream->write_string("order_mode=") << 0;
}

VOID db_handler::procres_load_all_yuanbao_order(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_yuanbao_order, p_buffer, tagYuanBaoOrder);
	tagYuanBaoOrder * p_yuanbao_order = ( tagYuanBaoOrder * ) p_buffer ; 

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		p_yuanbao_order[i].dwID = (*p_result)[0].get_dword();
		p_yuanbao_order[i].dw_role_id = (*p_result)[1].get_dword();
		p_yuanbao_order[i].eYBOType = (EYBOTYPE)(*p_result)[2].get_int();
		p_yuanbao_order[i].nPrice = (*p_result)[3].get_int();
		p_yuanbao_order[i].n_count = (*p_result)[4].get_int();
		p_yuanbao_order[i].eYBOMode = (EYBOMODE)(*p_result)[5].get_int();
		p_yuanbao_order[i].dwStartTime = (*p_result)[6].get_dword();
		p_yuanbao_order[i].dwEndTime =  (*p_result)[7].get_dword();
		p_yuanbao_order[i].nAvgPrice = (*p_result)[8].get_int();
		p_yuanbao_order[i].nDealNum = (*p_result)[9].get_int();

		p_result->next_row();
	}
}

// 读取角色元宝订单
BOOL db_handler::load_one_role_all_yuanbao_order(LPVOID p_buffer, int&n_count, DWORD dw_role_id)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_role_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_one_role_all_yuanbao_order),
		MakeDelegateOfDBHandler(&db_handler::procres_load_one_role_all_yuanbao_order));
}

VOID db_handler::makesql_load_one_role_all_yuanbao_order(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("yuanbao_order", "id,role_id,order_type,price,number,"
		"order_mode,start_time,end_time,avg_price,deal_num");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

VOID db_handler::procres_load_one_role_all_yuanbao_order(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_yuanbao_order, p_buffer, tagYuanBaoOrder);
	tagYuanBaoOrder * p_yuanbao_order = ( tagYuanBaoOrder * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		p_yuanbao_order[i].dwID = (*p_result)[0].get_dword();
		p_yuanbao_order[i].dw_role_id = (*p_result)[1].get_dword();
		p_yuanbao_order[i].eYBOType = (EYBOTYPE)(*p_result)[2].get_int();
		p_yuanbao_order[i].nPrice = (*p_result)[3].get_int();
		p_yuanbao_order[i].n_count = (*p_result)[4].get_int();
		p_yuanbao_order[i].eYBOMode = (EYBOMODE)(*p_result)[5].get_int();
		p_yuanbao_order[i].dwStartTime = (*p_result)[6].get_dword();
		p_yuanbao_order[i].dwEndTime =  (*p_result)[7].get_dword();
		p_yuanbao_order[i].nAvgPrice = (*p_result)[8].get_int();
		p_yuanbao_order[i].nDealNum = (*p_result)[9].get_int();

		p_result->next_row();
	}
}

// 获取所有团购信息
BOOL db_handler::load_all_group_purchase_info(OUT PVOID p_buffer, int &n_info_number)
{
	return common_select(p_buffer, &n_info_number, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_group_purchase_info),
		MakeDelegateOfDBHandler(&db_handler::procres_load_all_group_purchase_info));
}	
VOID db_handler::makesql_load_all_group_purchase_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("group_purchase", "guild_id,role_id,mall_id,ttem_price,"
		"participator_num,require_num,remain_time,participator_list");
}

VOID db_handler::procres_load_all_group_purchase_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_gp_info, p_buffer, s_gp_info);
	s_gp_info * p_gp_info = ( s_gp_info * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	for(INT16 i=0; i<*p_number; ++i)
	{
		p_gp_info->dw_guild_id			= (*p_result)[0].get_dword();
		p_gp_info->dw_role_id			= (*p_result)[1].get_dword();
		p_gp_info->dw_mall_id			= (*p_result)[2].get_dword();
		p_gp_info->n_price				= (*p_result)[3].get_dword();
		p_gp_info->n_participator_num	= (*p_result)[4].get_short();
		p_gp_info->n_require_num		= (*p_result)[5].get_short();
		p_gp_info->n_remain_time		= (*p_result)[6].get_int();

		(*p_result)[7].get_blob(p_gp_info->dw_participators, (*p_result)[7].get_length());

		p_gp_info = (s_gp_info*)((BYTE*)p_gp_info + sizeof(s_gp_info) + (p_gp_info->n_participator_num-1)*sizeof(DWORD));
		p_result->next_row();
	}

	p_buffer = p_gp_info;
}

// 读取所有活动数据
BOOL db_handler::load_activities(LPVOID p_buffer, int &n_count)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_activities), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_activities));
}

VOID db_handler::makesql_load_activities(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("activity", "id, script_data");
}

VOID db_handler::procres_load_activities(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_activity_data, p_buffer, s_activity_data);
	s_activity_data * p_activity_data = ( s_activity_data * ) p_buffer ;  

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		p_activity_data[i].dw_id = (*p_result)[0].get_dword();
		(*p_result)[1].get_blob(p_activity_data[i].data.dwData, (*p_result)[1].get_length());

		p_result->next_row();
	}
}

// 获取获取VIP摊位
BOOL db_handler::get_all_vip_stall_info(OUT PVOID p_buffer)
{
	int v_vip_stall_number = 0;

	// 更新到数据库
	common_select(p_buffer, &v_vip_stall_number, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_get_all_vip_stall_info), 
		MakeDelegateOfDBHandler(&db_handler::procres_get_all_vip_stall_info));

	if (v_vip_stall_number < VIP_STALL_MAX_NUM)
	{
		s_vip_stall_to_db* pInfo = (s_vip_stall_to_db*)p_buffer;

		for (int n=0; n<VIP_STALL_MAX_NUM; n++)
		{
			if (pInfo[n].by_stall_index == (BYTE)INVALID_VALUE)
			{
				pInfo[n].by_stall_index	= n;
				pInfo[n].dw_owner_id		= INVALID_VALUE;
				pInfo[n].n_remain_time	= INVALID_VALUE;
				common_update(INVALID_VALUE, &pInfo[n], 1, INVALID_VALUE, 
					MakeDelegateOfDBHandler(&db_handler::makesql_insert_vip_stall));
			}
		}
	}

	return TRUE;
}
VOID db_handler::makesql_insert_vip_stall(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, s_vip_stall_to_db);
	s_vip_stall_to_db * p = ( s_vip_stall_to_db * ) p_buffer ;  

	p_stream->insert_item("vip_stall");
	p_stream->write_string("stall_id=") << p->by_stall_index;
	p_stream->write_string(",role_id=") << p->dw_owner_id;
	p_stream->write_string(",remain_time=") << p->n_remain_time;
}

VOID db_handler::makesql_get_all_vip_stall_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->select_item("vip_stall", "stall_id,role_id,remain_time");
}

VOID db_handler::procres_get_all_vip_stall_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	//M_trans_pointer(p_vip_stall_info, p_buffer, s_vip_stall_to_db);
	s_vip_stall_to_db * p_vip_stall_info = ( s_vip_stall_to_db * ) p_buffer ; 


	*p_number = p_result->get_row_count();

	for(INT16 i=0; i<*p_number; ++i)
	{
		BYTE index = (*p_result)[0].get_byte();
		p_vip_stall_info[index].by_stall_index	= index;
		p_vip_stall_info[index].dw_owner_id		= (*p_result)[1].get_dword();
		p_vip_stall_info[index].n_remain_time	= (*p_result)[2].get_int();

		p_result->next_row();
	}
}

// 获取1v1积分榜
BOOL db_handler::load_1v1_score_rank()
{
	LPVOID p_buffer = NULL;
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE,
		MakeDelegateOfDBHandler(&db_handler::makesql_load_1v1_rank),
		MakeDelegateOfDBHandler(&db_handler::procres_load_1v1_rank));
}

VOID db_handler::makesql_load_1v1_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->write_string("select role_id, level, class, week_1v1_score from role_data where remove_flag = 0 order by week_1v1_score DESC limit 100");
}

VOID db_handler::procres_load_1v1_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dwNum = p_result->get_row_count();

	NET_DB2S_load_1v1_score_rank send;
	ZeroMemory(&send.st_1v1ScoreRank, sizeof(send.st_1v1ScoreRank));

	send.n_count = dwNum;

	for(INT i = 0; i < dwNum; i++)
	{
		send.st_1v1ScoreRank[i].dw_role_id = (*p_result)[0].get_dword();
		send.st_1v1ScoreRank[i].n_level = (*p_result)[1].get_int();
		send.st_1v1ScoreRank[i].e_class = (EClassType)(*p_result)[2].get_int();
		send.st_1v1ScoreRank[i].dw_score = (*p_result)[3].get_int();
		p_result->next_row();
	}

	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
}

// 获取等级排行榜
BOOL db_handler::load_level_rank()
{
	LPVOID p_buffer = NULL;
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_level_rank), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_level_rank));
}

VOID db_handler::makesql_load_level_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->write_string("select role_id, level, class, guild_id from role_data where remove_flag = 0 order by level DESC,exp_cur_level desc limit 100");
}

VOID db_handler::procres_load_level_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dwNum = p_result->get_row_count();

	NET_DB2S_load_level_rank send;
	ZeroMemory(&send.s_level_rank_, sizeof(send.s_level_rank_));

	send.n_count = dwNum;

	for(int i = 0; i < dwNum; i++)
	{
		send.s_level_rank_[i].dw_role_id = (*p_result)[0].get_dword();
		send.s_level_rank_[i].n_level_ = (*p_result)[1].get_int();
		send.s_level_rank_[i].e_class = (EClassType)(*p_result)[2].get_int();
		send.s_level_rank_[i].dw_guild_id = (*p_result)[3].get_dword();
		p_result->next_row();
	}

	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
}

// 获取帮派排行榜
BOOL db_handler::load_guild_rank()
{
	LPVOID p_buffer = NULL;
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_guild_rank), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_guild_rank));
}

VOID db_handler::makesql_load_guild_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->write_string("select id, level, prosperity, leader_id from guild where Level > 0 order by level desc,fund desc limit 100 ");
}

VOID db_handler::procres_load_guild_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dw_total_count = p_result->get_row_count();

	NET_DB2S_load_guild_rank send;
	send.n_count = dw_total_count;

	for(int i = 0; i < dw_total_count; i++)
	{
		send.s_guild_rank_[i].dw_guild_id = (*p_result)[0].get_dword();
		send.s_guild_rank_[i].n_level_ = (*p_result)[1].get_int();
		send.s_guild_rank_[i].n_prosperity_ = (*p_result)[2].get_int();
		send.s_guild_rank_[i].dw_leader_id_ = (*p_result)[3].get_dword();

		p_result->next_row();
	}

	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
}

// 获取恶人排行榜
BOOL db_handler::load_kill_rank()
{
	LPVOID p_buffer = NULL;
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_kill_rank), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_kill_rank));
}

VOID db_handler::makesql_load_kill_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->write_string("select role_id, level, kill_num, guild_id from role_data where remove_flag = 0 and kill_num > 0 order by kill_num DESC limit 100");
}

VOID db_handler::procres_load_kill_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dw_total_count = p_result->get_row_count();

	NET_DB2S_load_kill_rank send;
	ZeroMemory(&send.s_kill_rank_, sizeof(send.s_kill_rank_));

	send.n_count = dw_total_count;

	for(int i = 0; i < dw_total_count; i++)
	{
		send.s_kill_rank_[i].dw_role_id = (*p_result)[0].get_dword();
		send.s_kill_rank_[i].n_level_ = (*p_result)[1].get_int();
		send.s_kill_rank_[i].n_kill_num_ = (*p_result)[2].get_int();
		send.s_kill_rank_[i].dw_guild_id = (*p_result)[3].get_dword();
		p_result->next_row();
	}

	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
}

// 获取正义榜
BOOL db_handler::load_justice_rank()
{
	LPVOID p_buffer = NULL;
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_justice_rank), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_justice_rank));
}

VOID db_handler::makesql_load_justice_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->write_string("select role_id, level, justice, class, sex from role_data where remove_flag = 0 and justice > 0 order by justice DESC limit 100");
}

VOID db_handler::procres_load_justice_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dw_total_count = p_result->get_row_count();

	NET_DB2S_load_justice_rank send;
	ZeroMemory(&send.s_justice_rank_, sizeof(send.s_justice_rank_));

	send.n_count = dw_total_count;

	for(int i = 0; i < dw_total_count; i++)
	{
		send.s_justice_rank_[i].dw_role_id = (*p_result)[0].get_dword();
		send.s_justice_rank_[i].n_level_ = (*p_result)[1].get_int();
		send.s_justice_rank_[i].n32_justice = (*p_result)[2].get_int();
		send.s_justice_rank_[i].e_class = (EClassType)(*p_result)[3].get_dword();
		send.s_justice_rank_[i].by_Sex = (*p_result)[4].get_int();

		p_result->next_row();
	}

	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
}
//读取师徒榜
BOOL db_handler::load_master_rank()
{
	LPVOID p_buffer = NULL;
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_master_rank), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_master_rank));
}
VOID db_handler::makesql_load_master_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->write_string("select master, graduates from master_prentice where graduates > 0 order by graduates desc limit 100");
}
VOID db_handler::procres_load_master_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dwNum = p_result->get_row_count();
	NET_DB2S_load_MasterGraduate_rank send;
	ZeroMemory(&send.s_master_rank_, sizeof(send.s_master_rank_));
	send.n_count = dwNum;

	for(int i = 0; i < dwNum; i++)
	{
		send.s_master_rank_[i].dw_role_id = (*p_result)[0].get_dword();
		send.s_master_rank_[i].n_Graduates = (*p_result)[1].get_int();
		//根据role_id查该玩家的基本信息
		INT nNum = 0;
		odbc::sql_language_disposal* pStream = game_db_interface_->get_io();
		if(!pStream)
		{
			return ;
		}
		pStream->clear();
		pStream->select_item("role_data", "level,sex,class");
		pStream->where_item();
		pStream->write_string("role_id=") << send.s_master_rank_[i].dw_role_id;

		odbc::execute_result* pResult = game_db_interface_->sql_query(pStream);

		if(!pResult)
		{
			goto exit;
		}

		nNum = pResult->get_row_count() ;

		if(nNum > 0)//只找第一个
		{
			send.s_master_rank_[i].n_level_ = (*pResult)[0].get_int();
			send.s_master_rank_[i].by_Sex = (*pResult)[1].get_byte();
			send.s_master_rank_[i].e_class = (EClassType) (*pResult)[2].get_byte();
		}
exit:
		game_db_interface_->return_io(pStream);
		game_db_interface_->free_result_query(pResult);
		p_result->next_row();
	}

	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
}
//读取噬魂榜
BOOL db_handler::load_shihun_rank()
{
	LPVOID p_buffer = NULL;
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_shihun_rank), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_shihun_rank));
}

VOID db_handler::makesql_load_shihun_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->write_string("select role_id, level, class, shihun, sex from role_data where remove_flag = 0 order by shihun desc limit 100");
}

VOID db_handler::procres_load_shihun_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dwNum = p_result->get_row_count();

	NET_DB2S_load_shihun_rank send;
	ZeroMemory(&send.s_shihun_rank, sizeof(send.s_shihun_rank));

	send.n_count = dwNum;

	for(int i = 0; i < dwNum; i++)
	{
		send.s_shihun_rank[i].dw_role_id = (*p_result)[0].get_dword();
		send.s_shihun_rank[i].n_level_ = (*p_result)[1].get_int();
		send.s_shihun_rank[i].e_class = (EClassType)(*p_result)[2].get_int();
		send.s_shihun_rank[i].dw_shihun = (*p_result)[3].get_dword();
		send.s_shihun_rank[i].by_sex = (*p_result)[4].get_int();
		p_result->next_row();
	}

	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
}

//成就点数榜
BOOL db_handler::load_achpoint_rank()
{
	LPVOID p_buffer = NULL;
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_achpoint_rank), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_achpoint_rank));
}
VOID db_handler::makesql_load_achpoint_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->write_string("select role_id, level, achievemetn_point from role_data where remove_flag = 0 order by achievemetn_point desc limit 100");
}

VOID db_handler::procres_load_achpoint_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dwNum = p_result->get_row_count();

	NET_DB2S_load_achievement_point_rank send;
	ZeroMemory(&send.s_point_rank, sizeof(send.s_point_rank));

	send.n_count = dwNum;

	for(int i = 0; i < dwNum; i++)
	{
		send.s_point_rank[i].dw_role_id = (*p_result)[0].get_dword();
		send.s_point_rank[i].n_level_ = (*p_result)[1].get_int();
		send.s_point_rank[i].dw_point = (*p_result)[2].get_dword();
		p_result->next_row();
	}

	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
}
//成就数量榜
BOOL db_handler::load_achnumber_rank()
{
	LPVOID p_buffer = NULL;
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_achnumber_rank), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_achnumber_rank));
}
VOID db_handler::makesql_load_achnumber_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->write_string("select role_id, level, achievemetn_num from role_data where remove_flag = 0 order by achievemetn_num desc limit 100");
}

VOID db_handler::procres_load_achnumber_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dwNum = p_result->get_row_count();

	NET_DB2S_load_achievement_number_rank send;
	ZeroMemory(&send.s_number_rank, sizeof(send.s_number_rank));

	send.n_count = dwNum;

	for(int i = 0; i < dwNum; i++)
	{
		send.s_number_rank[i].dw_role_id = (*p_result)[0].get_dword();
		send.s_number_rank[i].n_level_ = (*p_result)[1].get_int();
		send.s_number_rank[i].dw_number = (*p_result)[2].get_dword();
		p_result->next_row();
	}

	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
}

//坐骑榜
BOOL db_handler::load_mounts_rank()
{
	LPVOID p_buffer = NULL;
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_mounts_rank), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_mounts_rank));
}
VOID db_handler::makesql_load_mounts_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->write_string("select role_id, step, grade from mounts order by step desc, grade desc limit 100");
}

VOID db_handler::procres_load_mounts_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dwNum = p_result->get_row_count();

	NET_DB2S_load_mounts_rank send;
	ZeroMemory(&send.s_mounts_rank, sizeof(send.s_mounts_rank));

	send.n_count = dwNum;

	for(int i = 0; i < dwNum; i++)
	{
		send.s_mounts_rank[i].dw_role_id = (*p_result)[0].get_dword();
		send.s_mounts_rank[i].n_setp = (*p_result)[1].get_int();
		send.s_mounts_rank[i].n_grade = (*p_result)[2].get_dword();
		p_result->next_row();
	}

	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
}

//充值榜
BOOL db_handler::load_reach_rank()
{
	LPVOID p_buffer = NULL;
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_reach_rank), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_reach_rank));
}
VOID db_handler::makesql_load_reach_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->write_string("select account_id, total_recharge from account_common order by total_recharge desc limit 100");
}

VOID db_handler::procres_load_reach_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dwNum = p_result->get_row_count();

	NET_DB2S_load_reach_rank send;
	ZeroMemory(&send.s_reach_rank, sizeof(send.s_reach_rank));

	send.n_count = dwNum;

	for(int i = 0; i < dwNum; i++)
	{
		send.s_reach_rank[i].dw_role_id = (*p_result)[0].get_dword();
		send.s_reach_rank[i].n_total_reach = (*p_result)[1].get_int();

		p_result->next_row();
	}

	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);
}


// 获取装备排行榜
BOOL db_handler::load_equip_rank(OUT LPVOID &p_buffer, int *p_number)
{
	return common_select(p_buffer, p_number, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_equip_rank), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_items));
}

VOID db_handler::makesql_load_equip_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->clear();
	p_stream->write_string("select ");
	init_item_load_field(p_stream);
	p_stream->write_string(" from item left join equip using(serial)");
	p_stream->where_item();
	p_stream->write_string("container_type_id=") << EICT_Equip;
	p_stream->write_string(" and type_id >= 9000000 and armor=0 ");
	p_stream->write_string("  order by rating desc limit 100 ");
}

// 获取所有邮件
BOOL db_handler::load_all_mail(LPVOID p_buffer)
{
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_mail), 
		MakeDelegateOfDBHandler(&db_handler::procres_laod_all_mail));
}

VOID db_handler::makesql_load_all_mail(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->select_item("mail",
		"mail_id,"		"mail_name,"	"mail_content,"	"send_role_id,"	"recv_role_id,"
		"is_send,"		"is_read,"	"is_withdrawal,"	"solve,"		"give_money,"
		"item_serial,"	"send_time,"	"recv_time,"	"is_at_once,"	"moneytype,"
		"yuanbao_type");
}

VOID db_handler::procres_laod_all_mail(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dw_total_count = p_result->get_row_count();

	int n_max_count = dw_total_count;
	int n_cur_count = 0;

	while(n_max_count > 0)
	{
		if(n_max_count > MAX_LOAD_NUM)
		{
			n_cur_count = MAX_LOAD_NUM;
		}
		else
		{
			n_cur_count = n_max_count;
		}
		memset(p_buffer, 0, sizeof(char)*MAX_LOAD_BUFFER);
		NET_DB2S_load_all_mail* p_send = (NET_DB2S_load_all_mail*)p_buffer;
		M_msg_init(p_send, NET_DB2S_load_all_mail);

		//M_trans_pointer(p, p_send->s_load_mail_, s_load_mail);
		s_load_mail * p = ( s_load_mail * ) p_send->s_load_mail_ ;  


		for(int i = 0; i < n_cur_count; i++)
		{
			p->s_mail_base_.dwID					= (*p_result)[0].get_dword();
			p->s_mail_base_.dwSendRoleID			= (*p_result)[3].get_dword();
			p->s_mail_base_.dwRecvRoleID			= (*p_result)[4].get_dword();
			p->s_mail_base_.bSend					= (*p_result)[5].get_int();
			p->s_mail_base_.bReed					= (*p_result)[6].get_int();
			p->s_mail_base_.bWithdrawal			= (*p_result)[7].get_int();
			p->s_mail_base_.dwSolve				= (*p_result)[8].get_dword();
			p->s_mail_base_.dwGiveMoney			= (*p_result)[9].get_dword();
			DataTime2DwordTime(p->s_mail_base_.dwSendTime, (*p_result)[11].get_string(), (*p_result)[11].get_length());
			DataTime2DwordTime(p->s_mail_base_.dwRecvTime, (*p_result)[12].get_string(), (*p_result)[12].get_length());
			p->s_mail_base_.bAtOnce				= (*p_result)[13].get_int();
			p->s_mail_base_.byType				= (*p_result)[14].get_short();
			p->s_mail_base_.n_yuanbao_type		= (*p_result)[15].get_short();

			(*p_result)[1].get_blob(p->str_name, sizeof(p->str_name));
			(*p_result)[10].get_blob(p->s_mail_base_.n64ItemSerial, sizeof(p->s_mail_base_.n64ItemSerial));
			p_result->next_row();
			++p;
		}

		p_send->n32_num_ = n_cur_count;
		p_send->dw_size = sizeof(NET_DB2S_load_all_mail) + (n_cur_count-1)*sizeof(s_load_mail);

		server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

		n_max_count -= n_cur_count;
	}

}

// 获取邮件内容
BOOL db_handler::load_mail_content(LPVOID p_buffer)
{
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_mail_content), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_mail_content));
}

VOID db_handler::makesql_load_mail_content(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dwMailID)
{
	p_stream->select_item("mail",
		"mail_content,"  "mail_id");
	/*p_stream->where_item();
	p_stream->write_string("mail_id=") << dwMailID;*/
}

VOID db_handler::procres_load_mail_content(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dw_total_count = p_result->get_row_count();

	int n_max_count = dw_total_count;
	int n_cur_count = 0;

	while(n_max_count > 0)
	{
		if(n_max_count > MAX_LOAD_NUM)
		{
			n_cur_count = MAX_LOAD_NUM;
		}
		else
		{
			n_cur_count = n_max_count;
		}
		memset(p_buffer, 0, sizeof(char)*MAX_LOAD_BUFFER);
		NET_DB2S_load_mail_content* p_send = (NET_DB2S_load_mail_content*)p_buffer;
		M_msg_init(p_send, NET_DB2S_load_mail_content);

		//M_trans_pointer(p, p_send->s_load_mail_, s_load_mail);
		tag_mail_content * p = ( tag_mail_content * ) p_send->st_mail_content ;  


		for(int i = 0; i < n_cur_count; i++)
		{
			LPVOID p_buffer = p->sz_content_;
			(*p_result)[0].get_blob(p_buffer, (*p_result)[0].get_length()); 
			p->dw_main_serial = (DWORD)(*p_result)[1].get_dword();
			p_result->next_row();
			++p;
		}

		p_send->n_num = n_cur_count;
		p_send->dw_size = sizeof(NET_DB2S_load_mail_content) + (n_cur_count-1)*sizeof(tag_mail_content);

		server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

		n_max_count -= n_cur_count;
	}
	
}

// 获取邮件物品
BOOL db_handler::load_mail_item(LPVOID p_buffer)
{
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_mail_item), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_mail_item));
}

VOID db_handler::makesql_load_mail_item(odbc::sql_language_disposal* p_stream, DWORD dw_mail_serial, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->write_string("select ");
	init_item_load_field(p_stream);
	p_stream->write_string(" from item left join equip using(serial)");
	p_stream->where_item();
	//p_stream->write_string("owner_id=") << dw_mail_serial;
	p_stream->write_string("container_type_id=") << EICT_MailItem;
}

VOID db_handler::procres_load_mail_item(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dw_total_count = p_result->get_row_count();

	int n_max_count = dw_total_count;
	int n_cur_count = 0;

	while(n_max_count > 0)
	{
		if(n_max_count > MAX_LOAD_NUM)
		{
			n_cur_count = MAX_LOAD_NUM;
		}
		else
		{
			n_cur_count = n_max_count;
		}
		memset(p_buffer, 0, sizeof(char)*MAX_LOAD_BUFFER);
		NET_DB2S_load_mail_item* p_send = (NET_DB2S_load_mail_item*)p_buffer;
		M_msg_init(p_send, NET_DB2S_load_mail_item);

		//M_trans_pointer(p, p_send->s_load_mail_, s_load_mail);
		LPVOID p_temp =  p_send->by_data_ ;  

		for(int i = 0; i < n_cur_count; i++)
		{
			tagItem * pItem = ( tagItem * ) p_temp ;  

			pItem->n64_serial 					= (*p_result)[0].get_long();
			pItem->n16Num 						= (*p_result)[1].get_short();
			pItem->dw_data_id 					= (*p_result)[2].get_dword();
			pItem->byBind 						= (*p_result)[3].get_int();
			pItem->bCreateBind					= (*p_result)[66].get_bool();
			pItem->bLock 						= (*p_result)[4].get_bool();

			pItem->nUseTimes 					= (*p_result)[5].get_int();
			pItem->dwCreatorID 					= (*p_result)[7].get_dword();
			pItem->dwOwnerID 					= (*p_result)[9].get_dword();
			pItem->dw_account_id 					= (*p_result)[10].get_dword();

			pItem->eConType 					= (EItemConType)(*p_result)[11].get_int();
			pItem->n16Index 					= (*p_result)[12].get_short();
			pItem->eCreateMode					= (EItemCreateMode)(*p_result)[46].get_int();
			pItem->dwCreateID					= (*p_result)[47].get_dword();

			pItem->dwNameID						=(*p_result)[51].get_dword();

			if(!DataTime2DwordTime(pItem->dw1stGainTime, (*p_result)[6].get_string(), (*p_result)[6].get_length()))
			{
				print_message(_T("file: %s line: %d  DataTime2DwordTime() error Item Serial is  %lld  !!!!!\r\n"), 
					_TFILE, _TLINE, pItem->n64_serial);
				ASSERT(0);
			}

			if(!DataTime2DwordTime(pItem->dwCreateTime, (*p_result)[8].get_string(), (*p_result)[8].get_length()))
			{
				print_message(_T("file: %s line: %d  DataTime2DwordTime() error Item Serial is  %lld  !!!!!\r\n"), 
					_TFILE, _TLINE, pItem->n64_serial);
				ASSERT(0);
			}

			if(!DataTime2DwordTime(pItem->dwBindTime, (*p_result)[62].get_string(), (*p_result)[62].get_length()))
			{
				print_message(_T("file: %s line: %d  DataTime2DwordTime() error Item Serial is  %lld  !!!!!\r\n"), 
					_TFILE, _TLINE, pItem->n64_serial);
				ASSERT(0);
			}

			pItem->dw_script_data[0] = (*p_result)[64].get_dword();
			pItem->dw_script_data[1] = (*p_result)[65].get_dword();

			if(!MIsEquipment(pItem->dw_data_id))
			{
				p_temp = &pItem[1];
				p_result->next_row();
				continue;
			}

			//M_trans_pointer(p_equip, p_buffer, tagEquip);
			tagEquip * p_equip = ( tagEquip * ) p_temp ;  

			// 装备附加属性
			p_equip->equipSpec.byQuality 		= (*p_result)[13].get_byte();
			/*p_equip->equipSpec.nPotValUsed 		= (*p_result)[14].get_int();
			p_equip->equipSpec.byMinUseLevel 	= (*p_result)[15].get_byte();
			p_equip->equipSpec.byMaxUseLevel 	= (*p_result)[16].get_byte();
			p_equip->equipSpec.n16MinDmg 		= (*p_result)[18].get_short();
			p_equip->equipSpec.n16MaxDmg 		= (*p_result)[19].get_short();
			p_equip->equipSpec.n16Armor 		= (*p_result)[20].get_short();
			p_equip->equipSpec.nPotVal 			= (*p_result)[21].get_int();
			p_equip->equipSpec.byPosyTimes 		= (*p_result)[23].get_byte();
			p_equip->equipSpec.byEngraveTimes 	= (*p_result)[25].get_byte();*/
			p_equip->equipSpec.byHoleNum 		= (*p_result)[27].get_byte();
			//p_equip->equipSpec.byTriggerCount 	= (*p_result)[29].get_byte();
			//p_equip->equipSpec.byLongfuLevel	= (*p_result)[31].get_byte();
			//p_equip->equipSpec.bCanCut 			= (*p_result)[33].get_bool();
			p_equip->equipSpec.byLuck			= (*p_result)[38].get_int();

			//// 时装附加属性
			//p_equip->equipSpec.n16Appearance				= (*p_result)[39].get_int();
			//p_equip->equipSpec.byRein					= (*p_result)[40].get_byte();
			//p_equip->equipSpec.bySavvy					= (*p_result)[41].get_byte();
			//p_equip->equipSpec.byFortune					= (*p_result)[42].get_byte();
			//p_equip->equipSpec.n8ColorID					= (INT8)(*p_result)[43].get_int();

			//p_equip->equipSpec.n16QltyModPct				= (*p_result)[44].get_short();
			//p_equip->equipSpec.n16QltyModPctEx			= (*p_result)[45].get_short();
			//p_equip->equipSpec.n16PotValModPct			= (*p_result)[52].get_short();
			//p_equip->equipSpec.n16AttALimMod				= (*p_result)[48].get_int();
			//p_equip->equipSpec.n16AttALimModPct			= (*p_result)[49].get_int();
			//p_equip->equipSpec.byFlareVal				= (*p_result)[50].get_byte();
			p_equip->equipSpec.byConsolidateLevel		= (*p_result)[53].get_byte();
			/*p_equip->equipSpec.nCurLevelExp				= (*p_result)[54].get_int();
			p_equip->equipSpec.nLevel					= (*p_result)[55].get_int();
			p_equip->equipSpec.byTalentPoint				= (*p_result)[59].get_int();
			p_equip->equipSpec.byMaxTalentPoint			= (*p_result)[60].get_int();*/
			p_equip->equipSpec.nRating					= (*p_result)[63].get_int();

			//(*p_result)[22].get_blob(p_equip->equipSpec.nRoleAttEffect, min((*p_result)[22].get_length(), sizeof(p_equip->equipSpec.nRoleAttEffect)));
			//(*p_result)[24].get_blob(p_equip->equipSpec.PosyEffect, min((*p_result)[24].get_length(), sizeof(p_equip->equipSpec.PosyEffect)));
			//(*p_result)[26].get_blob(p_equip->equipSpec.nEngraveAtt, min((*p_result)[26].get_length(), sizeof(p_equip->equipSpec.nEngraveAtt)));
			(*p_result)[28].get_blob(p_equip->equipSpec.dwHoleGemID, min((*p_result)[28].get_length(), sizeof(p_equip->equipSpec.dwHoleGemID)));
			//(*p_result)[30].get_blob(p_equip->equipSpec.nBrandAtt, min((*p_result)[30].get_length(), sizeof(p_equip->equipSpec.nBrandAtt)));
			//(*p_result)[32].get_blob(p_equip->equipSpec.byHoleGemNess, min((*p_result)[32].get_length(), sizeof(p_equip->equipSpec.byHoleGemNess)));
			(*p_result)[58].get_blob(p_equip->equipSpec.EquipAttitionalAtt, min((*p_result)[58].get_length(), sizeof(p_equip->equipSpec.EquipAttitionalAtt)));
			//(*p_result)[61].get_blob(p_equip->equipSpec.dwSkillList, min((*p_result)[61].get_length(), sizeof(p_equip->equipSpec.dwSkillList)));
			
			//p_equip->equipSpec.byConsolidateLevelStar		= (*p_result)[67].get_byte();
			//p_equip->equipSpec.byAddTalentPoint			= (*p_result)[68].get_byte();
			
			(*p_result)[69].get_blob(p_equip->equipSpec.EquipAttitional, min((*p_result)[69].get_length(), sizeof(p_equip->equipSpec.EquipAttitional)));

			p_temp = &p_equip[1];
			p_result->next_row();
		}

		p_send->n_count = n_cur_count;
		p_send->dw_size = sizeof(NET_DB2S_load_mail_item) - sizeof(BYTE) + (DWORD)((BYTE*)p_temp - (BYTE*)p_send->by_data_);

		server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

		n_max_count -= n_cur_count;
	}
}

// 获取邮件最大编号
BOOL db_handler::get_mail_max_serial(DWORD& dw_mail_serial)
{
	DWORD dw_max = 0;
	get_max_mail_serial("mail", dw_max);

	dw_mail_serial = max(dw_mail_serial, dw_max);

	return TRUE;
}


// 初始化道具(装备)字段
VOID db_handler::init_item_load_field(odbc::sql_language_disposal *p_stream)
{
	p_stream->write_string(
		"serial,"	"num,"				"type_id,"		"bind,"				"lock_state,"
		"use_times,"		"first_gain_time,"	"creator_id,"	"create_time,"		"owner_id,"
		"account_id,"	"container_type_id,"	"suffix,"		"quality,"			"potval_used,"
		"min_use_level,"	"max_use_level,"		"min_dmg_in,"		"min_dmg,"			"max_dmg,"		
		"armor,"		"potval,"			"role_att_effect," "posy_times," "posy_effect,"
		"engrave_times,"	"engrave_att,"		"hole_num,"		"hole_gem_id,"		"brand_level,"
		"derate_val,"	"x_ful_level,"		"hole_gem_ness,"		"can_cut,"			"xer_in_id,"
		"xer_out_id,"	"0,"				"0,"			"special_att,"			"appearance,"
		"rein,"			"savvy,"			"fortune,"		"color_id,"			"quality_mod_pct,"
		"quality_mod_pct_ex,"	"create_mode,"		"create_id,"		"att_a_lim_mod," "att_a_lim_mod_pct,"
		"flare_val,"		"name_id,"			"pot_val_mod_pct,"	"consolidate_level," "exp," "level," 
		"max_dmg_in,"		"armor_in,"			"equip_add_att,"	"talent_point,"		"max_talent_point,"
		"skill_list,"	"bind_time,"			"rating,"			"script_data1,"		"script_data2, " 
		"create_bind,"	"consolidate_level_star," "add_talent_point," "equip_rel_att");
}

// 处理读取的道具(装备)结果集
VOID db_handler::procres_load_items(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		//M_trans_pointer(pItem, p_buffer, tagItem);	
		tagItem * pItem = ( tagItem * ) p_buffer ;  

		pItem->n64_serial 					= (*p_result)[0].get_long();
		pItem->n16Num 						= (*p_result)[1].get_short();
		pItem->dw_data_id 					= (*p_result)[2].get_dword();
		pItem->byBind 						= (*p_result)[3].get_int();
		pItem->bCreateBind					= (*p_result)[66].get_bool();
		pItem->bLock 						= (*p_result)[4].get_bool();

		pItem->nUseTimes 					= (*p_result)[5].get_int();
		pItem->dwCreatorID 					= (*p_result)[7].get_dword();
		pItem->dwOwnerID 					= (*p_result)[9].get_dword();
		pItem->dw_account_id 					= (*p_result)[10].get_dword();

		pItem->eConType 					= (EItemConType)(*p_result)[11].get_int();
		pItem->n16Index 					= (*p_result)[12].get_short();
		pItem->eCreateMode					= (EItemCreateMode)(*p_result)[46].get_int();
		pItem->dwCreateID					= (*p_result)[47].get_dword();

		pItem->dwNameID						=(*p_result)[51].get_dword();

		if(!DataTime2DwordTime(pItem->dw1stGainTime, (*p_result)[6].get_string(), (*p_result)[6].get_length()))
		{
			print_message(_T("file: %s line: %d  DataTime2DwordTime() error Item Serial is  %lld  !!!!!\r\n"), 
				_TFILE, _TLINE, pItem->n64_serial);
			ASSERT(0);
		}

		if(!DataTime2DwordTime(pItem->dwCreateTime, (*p_result)[8].get_string(), (*p_result)[8].get_length()))
		{
			print_message(_T("file: %s line: %d  DataTime2DwordTime() error Item Serial is  %lld  !!!!!\r\n"), 
				_TFILE, _TLINE, pItem->n64_serial);
			ASSERT(0);
		}

		if(!DataTime2DwordTime(pItem->dwBindTime, (*p_result)[62].get_string(), (*p_result)[62].get_length()))
		{
			print_message(_T("file: %s line: %d  DataTime2DwordTime() error Item Serial is  %lld  !!!!!\r\n"), 
				_TFILE, _TLINE, pItem->n64_serial);
			ASSERT(0);
		}

		pItem->dw_script_data[0] = (*p_result)[64].get_dword();
		pItem->dw_script_data[1] = (*p_result)[65].get_dword();

		if(!MIsEquipment(pItem->dw_data_id))
		{
			p_buffer = &pItem[1];
			p_result->next_row();
			continue;
		}

		//M_trans_pointer(p_equip, p_buffer, tagEquip);
		tagEquip * p_equip = ( tagEquip * ) p_buffer ;  

		// 装备附加属性
		p_equip->equipSpec.byQuality 		= (*p_result)[13].get_byte();
		/*p_equip->equipSpec.nPotValUsed 		= (*p_result)[14].get_int();
		p_equip->equipSpec.byMinUseLevel 	= (*p_result)[15].get_byte();
		p_equip->equipSpec.byMaxUseLevel 	= (*p_result)[16].get_byte();
		p_equip->equipSpec.n16MinDmg 		= (*p_result)[18].get_short();
		p_equip->equipSpec.n16MaxDmg 		= (*p_result)[19].get_short();
		p_equip->equipSpec.n16Armor 		= (*p_result)[20].get_short();
		p_equip->equipSpec.nPotVal 			= (*p_result)[21].get_int();
		p_equip->equipSpec.byPosyTimes 		= (*p_result)[23].get_byte();
		p_equip->equipSpec.byEngraveTimes 	= (*p_result)[25].get_byte();*/
		p_equip->equipSpec.byHoleNum 		= (*p_result)[27].get_byte();
		//p_equip->equipSpec.byTriggerCount 	= (*p_result)[29].get_byte();
		//p_equip->equipSpec.byLongfuLevel	= (*p_result)[31].get_byte();
		//p_equip->equipSpec.bCanCut 			= (*p_result)[33].get_bool();
		p_equip->equipSpec.byLuck		= (*p_result)[38].get_int();

		//// 时装附加属性
		//p_equip->equipSpec.n16Appearance				= (*p_result)[39].get_int();
		//p_equip->equipSpec.byRein					= (*p_result)[40].get_byte();
		//p_equip->equipSpec.bySavvy					= (*p_result)[41].get_byte();
		//p_equip->equipSpec.byFortune					= (*p_result)[42].get_byte();
		//p_equip->equipSpec.n8ColorID					= (INT8)(*p_result)[43].get_int();

		//p_equip->equipSpec.n16QltyModPct				= (*p_result)[44].get_short();
		//p_equip->equipSpec.n16QltyModPctEx			= (*p_result)[45].get_short();
		//p_equip->equipSpec.n16PotValModPct			= (*p_result)[52].get_short();
		//p_equip->equipSpec.n16AttALimMod				= (*p_result)[48].get_int();
		//p_equip->equipSpec.n16AttALimModPct			= (*p_result)[49].get_int();
		//p_equip->equipSpec.byFlareVal				= (*p_result)[50].get_byte();
		p_equip->equipSpec.byConsolidateLevel		= (*p_result)[53].get_byte();
		/*p_equip->equipSpec.nCurLevelExp				= (*p_result)[54].get_int();
		p_equip->equipSpec.nLevel					= (*p_result)[55].get_int();
		p_equip->equipSpec.byTalentPoint				= (*p_result)[59].get_int();
		p_equip->equipSpec.byMaxTalentPoint			= (*p_result)[60].get_int();*/
		p_equip->equipSpec.nRating					= (*p_result)[63].get_int();

		//(*p_result)[22].get_blob(p_equip->equipSpec.nRoleAttEffect, min((*p_result)[22].get_length(), sizeof(p_equip->equipSpec.nRoleAttEffect)));
		//(*p_result)[24].get_blob(p_equip->equipSpec.PosyEffect, min((*p_result)[24].get_length(), sizeof(p_equip->equipSpec.PosyEffect)));
		//(*p_result)[26].get_blob(p_equip->equipSpec.nEngraveAtt, min((*p_result)[26].get_length(), sizeof(p_equip->equipSpec.nEngraveAtt)));
		(*p_result)[28].get_blob(p_equip->equipSpec.dwHoleGemID, min((*p_result)[28].get_length(), sizeof(p_equip->equipSpec.dwHoleGemID)));
		//(*p_result)[30].get_blob(p_equip->equipSpec.nBrandAtt, min((*p_result)[30].get_length(), sizeof(p_equip->equipSpec.nBrandAtt)));
		//(*p_result)[32].get_blob(p_equip->equipSpec.byHoleGemNess, min((*p_result)[32].get_length(), sizeof(p_equip->equipSpec.byHoleGemNess)));
		(*p_result)[58].get_blob(p_equip->equipSpec.EquipAttitionalAtt, min((*p_result)[58].get_length(), sizeof(p_equip->equipSpec.EquipAttitionalAtt)));
		//(*p_result)[61].get_blob(p_equip->equipSpec.dwSkillList, min((*p_result)[61].get_length(), sizeof(p_equip->equipSpec.dwSkillList)));
		
		//p_equip->equipSpec.byConsolidateLevelStar	= (*p_result)[67].get_byte();
		//p_equip->equipSpec.byAddTalentPoint			= (*p_result)[68].get_byte();
	
		(*p_result)[69].get_blob(p_equip->equipSpec.EquipAttitional, min((*p_result)[69].get_length(), sizeof(p_equip->equipSpec.EquipAttitional)));

		p_buffer = &p_equip[1];
		p_result->next_row();
	}
}




