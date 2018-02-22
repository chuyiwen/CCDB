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
#include "../../common/WorldDefine/vcard_define.h"
#include "../../common/WorldDefine/Sign_define.h"
#include "../../common/WorldDefine/LianHun_define.h"
#include "../../common/WorldDefine/reward_define.h"

#include "../common/ServerDefine/item_server_define.h"
#include "../common/ServerDefine/role_data_server_define.h"
#include "../common/ServerDefine/clandata_server_define.h"
#include "../common/ServerDefine/item_server_define.h"
#include "../common/ServerDefine/activity_server_define.h"
#include "../common/ServerDefine/mail_server_define.h"
#include "../common/ServerDefine/title_server_define.h"
#include "../common/ServerDefine/achievement_server_define.h"

#include "db_handler.h"

// 参数传递用
struct local_item_id
{
	int64	n64_serial;
	const char*	sz_table;
};
struct local_item_ptr
{
	union
	{
		const tagItem*	pItem;
		const tagEquip* p_equip;
	};

	const char*		sz_table;
};


// 存储角色
VOID db_handler::save_one_role(DWORD dw_role_id, s_role_data_save *p_role_data)
{
	// 保存角色基础数据
	save_role_data(dw_role_id, p_role_data);

	//************************************************
	// 1.最好和读取玩家时顺序一致。
	// 2.注意每次都必须更改指针可写位置。
	// 3.新增数据最好加在尾部，不要从中间插入
	// 4.完成所有数据后需要重设包的大小
	// 此处采用如下顺序：
	// 技能—>状态—>称号—>物品—>未完成任务—>物品CD—>
	// 家族—>宠物—>地图进入限制
	//************************************************

	// 可写开始位置
	LPVOID p_buffer = p_role_data->by_data;

	// 1.技能列表
	save_role_skill(dw_role_id, p_buffer, p_role_data->n_skill_num_, &p_buffer);

	// 2.状态列表
	save_role_buff(dw_role_id, p_buffer, p_role_data->n_buff_num_, &p_buffer);

	// 3.称号列表
	save_role_title(dw_role_id, p_buffer, p_role_data->n_title_num_, &p_buffer);

	// 4.物品列表 
	save_item_attribute(dw_role_id, p_buffer, p_role_data->n_item_num_, &p_buffer);

	// 5.任务列表
	save_quest(dw_role_id, p_buffer, p_role_data->n_quest_num_, &p_buffer);

	// 6.物品冷却时间表
	replace_item_cd_time(dw_role_id, p_buffer, p_role_data->n_item_cd_time_num_, &p_buffer);

	// 7.保存家族数据
	//save_role_clan_data(dw_role_id, p_buffer, p_role_data->b_clan_data_chg_, &p_buffer);

	// 8.保存宠物
	//save_pets(dw_role_id, p_buffer, p_role_data->n_pets_num_, &p_buffer);

	// 9.进入地图限制
	save_enter_map_limit(dw_role_id, p_buffer, p_role_data->n_map_limit_num_, &p_buffer);

	// 10.完成成就
	//save_complate_achievement(dw_role_id, p_buffer, p_role_data->n_achievement_num_, &p_buffer);

	// 11.成就进度
	//save_process_achievement(dw_role_id, p_buffer, p_role_data->n_achievement_criteria_num_, &p_buffer);

	//12. 副本进度
	delete_inst_process(dw_role_id);
	save_process_instance(dw_role_id, p_buffer, p_role_data->n_inst_process_num, &p_buffer);

	//13. 签到数据
	save_sign_data(dw_role_id, p_buffer, &p_buffer);

	//14. 魂晶数据
	//save_huenjing_data(dw_role_id, p_buffer, &p_buffer);

	//15. 奖励数据
	save_reward_data(dw_role_id, p_buffer, &p_buffer);
}


// 新增物品
BOOL db_handler::insert_item(const tagItem &item, const char* sz_table)
{
	local_item_ptr st_new_item;
	st_new_item.pItem	= &item;
	st_new_item.sz_table	= sz_table;
	return common_update(INVALID_VALUE, (LPVOID)&st_new_item, 1, SIZE_ITEM, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_item));
}
VOID db_handler::makesql_insert_item(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(st_item_ptr, p_buffer, local_item_ptr);
	const tagItem *p = st_item_ptr->pItem;

	p_stream->insert_item(st_item_ptr->sz_table);

	p_stream->write_string("serial=") << p->n64_serial;
	p_stream->write_string(",type_id=") << p->dw_data_id;

	p_stream->write_string(",num=") << p->n16Num;
	p_stream->write_string(",bind=") << p->byBind;
	p_stream->write_string(",lock_state=") << p->bLock;
	p_stream->write_string(",create_bind=") << p->bCreateBind;
	p_stream->write_string(",use_times=") << p->nUseTimes;

	p_stream->write_string(",create_mode=") << p->eCreateMode;
	p_stream->write_string(",create_id=") << p->dwCreateID;

	p_stream->write_string(",creator_id=") << p->dwCreatorID;
	p_stream->write_string(",name_id=") << p->dwNameID;

	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dwCreateTime);
	p_stream->write_string(",create_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");

	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw1stGainTime);
	p_stream->write_string(",first_gain_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");

	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dwBindTime);
	p_stream->write_string(",bind_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");

	p_stream->write_string(",owner_id=") << p->dwOwnerID;
	p_stream->write_string(",account_id=") << p->dw_account_id;
	p_stream->write_string(",container_type_id=") << p->eConType;
	p_stream->write_string(",suffix=") << p->n16Index;
	p_stream->write_string(",script_data1=") << p->dw_script_data[0];
	p_stream->write_string(",script_data2=") << p->dw_script_data[1];
}

// 新增装备
BOOL db_handler::insert_equip(const tagEquip &equip, BOOL b_baibao/* = FALSE*/)
{
	const char* p_item_table	= "item";
	const char* p_equip_table	= "equip";

	if(b_baibao)
	{
		p_item_table	= "item_baibao";
		p_equip_table = "equip_baibao";
	}

	BOOL b_ret = insert_item(equip, p_item_table);
	if(!b_ret)return b_ret;

	b_ret = insert_equip_special(equip, p_equip_table);
	if(!b_ret) delete_item(equip.n64_serial, p_item_table);

	return b_ret;
}

// 新增装备属性
BOOL db_handler::insert_equip_special(const tagEquip &equip, const char* sz_table)
{
	local_item_ptr st_new_item;
	st_new_item.p_equip	= &equip;
	st_new_item.sz_table	= sz_table;
	return common_update(INVALID_VALUE, (LPVOID)&st_new_item, 1, SIZE_EQUIP, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_equip_special));
}
VOID db_handler::makesql_insert_equip_special(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, local_item_ptr);

	p_stream->insert_item(p->sz_table);

	p_stream->write_string("serial=") << p->p_equip->n64_serial;
	p_stream->write_string(",");
	makesql_update_equip_special_common(p_stream, &p->p_equip->equipSpec);
}
VOID db_handler::makesql_update_equip_special(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_equip_special_update)
{
	M_trans_pointer(p, p_equip_special_update, s_equip_spec_update);

	p_stream->update_item("equip");
	makesql_update_equip_special_common(p_stream, &p->equip_spec);

	p_stream->where_item();
	p_stream->write_string("serial=") << p->n64_serial;
}
VOID db_handler::makesql_update_equip_special_common(odbc::sql_language_disposal *p_stream, const tagEquipSpec *p_equip_special)
{
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->write_string("quality=") << p_equip_special->byQuality;
	//p_stream->write_string(",min_use_level=") << p_equip_special->byMinUseLevel;
	//p_stream->write_string(",max_use_level=") << p_equip_special->byMaxUseLevel;
	//p_stream->write_string(",att_a_lim_mod=") << p_equip_special->n16AttALimMod;
	//p_stream->write_string(",att_a_lim_mod_pct=") << p_equip_special->n16AttALimModPct;

	//p_stream->write_string(",min_dmg=") << p_equip_special->n16MinDmg;
	//p_stream->write_string(",max_dmg=") << p_equip_special->n16MaxDmg;
	//p_stream->write_string(",armor=") << p_equip_special->n16Armor;
	//p_stream->write_string(",potval=") << p_equip_special->nPotVal;
	//p_stream->write_string(",potval_used=") << p_equip_special->nPotValUsed;

	//p_stream->write_string(",posy_times=") << p_equip_special->byPosyTimes;
	//p_stream->write_string(",engrave_times=") << p_equip_special->byEngraveTimes;
	p_stream->write_string(",hole_num=") << p_equip_special->byHoleNum;
	//p_stream->write_string(",brand_level=") << p_equip_special->byTriggerCount;
	//p_stream->write_string(",x_ful_level=") << p_equip_special->byLongfuLevel;

	//p_stream->write_string(",can_cut=") << p_equip_special->bCanCut;
	p_stream->write_string(",special_att=") << p_equip_special->byLuck;
	//p_stream->write_string(",flare_val=") << p_equip_special->byFlareVal;
	//p_stream->write_string(",quality_mod_pct=") << p_equip_special->n16QltyModPct;
	//p_stream->write_string(",quality_mod_pct_Ex=") << p_equip_special->n16QltyModPctEx;
	//p_stream->write_string(",pot_val_mod_pct=") << p_equip_special->n16PotValModPct;

	// 时装属性
	//p_stream->write_string(",appearance=") << p_equip_special->n16Appearance;
	//p_stream->write_string(",rein=") << p_equip_special->byRein;
	//p_stream->write_string(",savvy=") << p_equip_special->bySavvy;
	//p_stream->write_string(",fortune=") << p_equip_special->byFortune;
	//p_stream->write_string(",color_id=") << p_equip_special->n8ColorID;

	p_stream->write_string(",consolidate_level=") << p_equip_special->byConsolidateLevel;
	//p_stream->write_string(",consolidate_level_star=") << p_equip_special->byConsolidateLevelStar;
	//p_stream->write_string(",exp=") << p_equip_special->nCurLevelExp;
	//p_stream->write_string(",level=") << p_equip_special->nLevel;
	//p_stream->write_string(",talent_point=") << p_equip_special->byTalentPoint;
	//p_stream->write_string(",max_talent_point=") << p_equip_special->byMaxTalentPoint;
	//p_stream->write_string(",add_talent_point=") << p_equip_special->byAddTalentPoint;
	p_stream->write_string(",rating=") << p_equip_special->nRating;

	//p_stream->write_string(",role_att_effect='").write_blob(p_equip_special->nRoleAttEffect, sizeof(p_equip_special->nRoleAttEffect), p_conn);
	//p_stream->write_string("',posy_effect='").write_blob(p_equip_special->PosyEffect, sizeof(p_equip_special->PosyEffect), p_conn);
	//p_stream->write_string("',engrave_att='").write_blob(p_equip_special->nEngraveAtt, sizeof(p_equip_special->nEngraveAtt), p_conn);
	p_stream->write_string(",hole_gem_id='").write_blob(p_equip_special->dwHoleGemID, sizeof(p_equip_special->dwHoleGemID), p_conn);
	//p_stream->write_string("',derate_val='").write_blob(p_equip_special->nBrandAtt, sizeof(p_equip_special->nBrandAtt), p_conn);
	//p_stream->write_string("',hole_gem_ness='").write_blob(p_equip_special->byHoleGemNess, sizeof(p_equip_special->byHoleGemNess), p_conn);
	p_stream->write_string("',equip_add_att='").write_blob(p_equip_special->EquipAttitionalAtt, sizeof(p_equip_special->EquipAttitionalAtt), p_conn);
	//p_stream->write_string("',skill_list='").write_blob(p_equip_special->dwSkillList, sizeof(p_equip_special->dwSkillList), p_conn);
	p_stream->write_string("',equip_rel_att='").write_blob(p_equip_special->EquipAttitional, sizeof(p_equip_special->EquipAttitional), p_conn);

	p_stream->write_string("'");

	game_db_interface_->return_use_connect(p_conn);
}

// 保存物品
BOOL db_handler::save_item(const tagItem &item)
{
	return common_update(INVALID_VALUE, (LPVOID)&item, 1, SIZE_ITEM, 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_item));
}

VOID db_handler::makesql_save_item(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID pItem)
{
	M_trans_pointer(p, pItem, tagItem);

	p_stream->update_item("item");

	p_stream->write_string("num=") << p->n16Num;
	p_stream->write_string(",bind=") << p->byBind;
	p_stream->write_string(",lock_state=") << p->bLock;
	p_stream->write_string(",create_bind=") << p->bCreateBind;
	p_stream->write_string(",use_times=") << p->nUseTimes;

	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw1stGainTime);
	p_stream->write_string(",first_gain_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");

	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dwBindTime);
	p_stream->write_string(",bind_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");

	p_stream->write_string(",owner_id=") << p->dwOwnerID;
	p_stream->write_string(",account_id=") << p->dw_account_id;
	p_stream->write_string(",container_type_id=") << p->eConType;
	p_stream->write_string(",suffix=") << p->n16Index;
	p_stream->write_string(",script_data1=") << p->dw_script_data[0];
	p_stream->write_string(",script_data2=") << p->dw_script_data[1];

	p_stream->where_item();
	p_stream->write_string("serial=") << p->n64_serial;
}

// 保存装备
BOOL db_handler::save_equip(const tagEquip &equip)
{
	BOOL b_ret = save_item(equip);
	if(!b_ret) return b_ret;

	b_ret = save_equip_special(equip);
	if(!b_ret) ASSERT(FALSE);

	return b_ret;
}

// 保存装备属性
BOOL db_handler::save_equip_special(const tagEquip &equip)
{
	return common_update(INVALID_VALUE, (LPVOID)&equip, 1, SIZE_EQUIP, 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_equip_special));
}

BOOL db_handler::save_equip_special(const s_equip_spec_update &equip_special_update)
{
	return common_update(INVALID_VALUE, (LPVOID)&equip_special_update, 1, sizeof(s_equip_spec_update), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_equip_special));
}
VOID db_handler::makesql_save_equip_special(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_equip)
{
	M_trans_pointer(p, p_equip, tagEquip);

	p_stream->update_item("equip");
	makesql_update_equip_special_common(p_stream, &p->equipSpec);

	p_stream->where_item();
	p_stream->write_string("serial=") << p->n64_serial;
}
// 删除物品
BOOL db_handler::delete_item(int64 n64_serial, const char *sz_table/* = "item"*/)
{
	local_item_id item_del;
	item_del.n64_serial	= n64_serial;
	item_del.sz_table		= sz_table;
	return common_update(INVALID_VALUE, (LPVOID)&item_del, 1, sizeof(int64), 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_item));
}
VOID db_handler::makesql_delete_item(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, local_item_id);

	p_stream->delete_item(p->sz_table);

	p_stream->where_item();
	p_stream->write_string("serial=") << p->n64_serial;
}

// 删除装备
BOOL db_handler::delete_equip(int64 n64_serial)
{
	if ( !(backup_deleted_equip(n64_serial) && backup_deleted_item(n64_serial)) )
		return FALSE;	

	if(delete_item(n64_serial))
	{
		return delete_equip_special(n64_serial);
	}

	return FALSE;
}

// 删除装备属性
BOOL db_handler::delete_equip_special(int64 n64_serial, char *sz_table/* = "equip"*/)
{
	local_item_id item_del;
	item_del.n64_serial	= n64_serial;
	item_del.sz_table		= sz_table;
	return common_update(INVALID_VALUE, (LPVOID)&item_del, 1, sizeof(int64), 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_equip_special));
}

VOID db_handler::makesql_delete_equip_special(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, local_item_id);

	p_stream->delete_item(p->sz_table);

	p_stream->where_item();
	p_stream->write_string("serial=") << p->n64_serial;
}


//备份删除的道具
BOOL db_handler::backup_deleted_equip(int64 n64_serial)
{
	// 获取流和连接
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();
	ASSERT(VALID_POINT(p_stream));
	ASSERT(VALID_POINT(p_conn));

	// 删除操作
	p_stream->copy_table("equip_del", "*", "equip");
	p_stream->where_item();
	p_stream->write_string("serial=") << n64_serial;

	// 执行SQL
	BOOL b_ret = game_db_interface_->sql_execute(p_stream);

	// 归还sql格式化IO和连接
	game_db_interface_->return_use_connect(p_conn);
	game_db_interface_->return_io(p_stream);

	return b_ret;
}

//备份删除的装备
BOOL db_handler::backup_deleted_item(int64 n64_serial)
{
	// 获取流和连接
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();
	ASSERT(VALID_POINT(p_stream));
	ASSERT(VALID_POINT(p_conn));

	// 删除操作
	p_stream->copy_table("item_del", "*", "item");
	p_stream->where_item();
	p_stream->write_string("serial=") << n64_serial;

	// 执行SQL
	BOOL b_ret = game_db_interface_->sql_execute(p_stream);

	// 归还sql格式化IO和连接
	game_db_interface_->return_use_connect(p_conn);
	game_db_interface_->return_io(p_stream);

	return b_ret;
}

// 保存角色数据
BOOL db_handler::save_role_data(DWORD dw_role_id, const s_role_data_save *p_role_data)
{
	return common_update(dw_role_id, (LPVOID)p_role_data, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_role_data));
}
VOID db_handler::makesql_save_role_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	s_role_data_save *p = (s_role_data_save *)p_buffer;

	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->update_item("role_data");
	p_stream->write_string("avatar_equip='").write_blob(&p->avatar_equip_, SIZE_AVATAR_EQUIP, p_conn);
	p_stream->write_string("',display_set=")<< (BYTE)p->display_set_;
	p_stream->write_string(",map_id=") << p->dw_map_id_;
	p_stream->write_string(",x=") << p->f_coordinate_[0];// 坐标
	p_stream->write_string(",y=") << p->f_coordinate_[1];
	p_stream->write_string(",z=") << p->f_coordinate_[2];
	p_stream->write_string(",face_x=") << p->f_face_[0];// 朝向
	p_stream->write_string(",face_y=") << p->f_face_[1];
	p_stream->write_string(",face_z=") << p->f_face_[2];

	p_stream->write_string(",reborn_map_id=") << p->dw_reborn_map_id_;

	p_stream->write_string(",class=") << (int)p->e_class_;
	p_stream->write_string(",classex=") << p->e_class_ex_;
	p_stream->write_string(",level=") << p->n_level_;
	p_stream->write_string(",exp_cur_level=") << p->n_cur_exp_;

	p_stream->write_string(",hp=") << p->n_hp_;
	p_stream->write_string(",mp=") << p->n_mp_;
	p_stream->write_string(",rage=") << p->n_rage_;
	p_stream->write_string(",endurance=") << p->n_endurance_;
	p_stream->write_string(",vitality=") << p->n_brotherhood_;
	p_stream->write_string(",injury=") << p->n_injury_;
	p_stream->write_string(",knowledge=") << p->n_knowledge_;
	p_stream->write_string(",morale=") << p->n_morale_;
	p_stream->write_string(",morality=") << p->n_morality_;
	p_stream->write_string(",culture=") << p->n_luck_;
	p_stream->write_string(",credit=") << p->n_credit_;
	p_stream->write_string(",identity=") << p->n_identity_;
	p_stream->write_string(",vip_point=") << p->n_vip_point_;

	p_stream->write_string(",att_avail=") << p->n_att_point_;
	p_stream->write_string(",talent_avail=") << p->n_talent_point_;

	p_stream->write_string(",physique_added=") << p->n_att_point_add_[ERA_Physique];
	p_stream->write_string(",strength_added=") << p->n_att_point_add_[ERA_Strength];
	p_stream->write_string(",pneuma_added=") << p->n_att_point_add_[ERA_Pneuma];
	p_stream->write_string(",innerforce_added=") << p->n_att_point_add_[ERA_InnerForce];
	p_stream->write_string(",technique_added=") << p->n_att_point_add_[ERA_Technique];
	p_stream->write_string(",agility_added=") << p->n_att_point_add_[ERA_Agility];

	p_stream->write_string(",talent_type1=") << p->talent_[0].eType;
	p_stream->write_string(",talent_type2=") << p->talent_[1].eType;
	p_stream->write_string(",talent_type3=") << p->talent_[2].eType;
	p_stream->write_string(",talent_type4=") << p->talent_[3].eType;
	p_stream->write_string(",talent_val1=") << p->talent_[0].nPoint;
	p_stream->write_string(",talent_val2=") << p->talent_[1].nPoint;
	p_stream->write_string(",talent_val3=") << p->talent_[2].nPoint;
	p_stream->write_string(",talent_val4=") << p->talent_[3].nPoint;

	p_stream->write_string(",safe_guard_flag=") << p->b_safe_guard_;
	p_stream->write_string(",pk_value=") << p->n_pk_value_;

	p_stream->write_string(",bag_size=") << p->n16_bag_size_;		// 背包
	p_stream->write_string(",bag_gold=") << p->n_bag_gold_;
	p_stream->write_string(",bag_silver=") << p->n_bag_silver_;
	p_stream->write_string(",bag_copper=")	<< p->n_bag_copper_;
	p_stream->write_string(",bag_bind_gold=") << p->n_bag_bind_gold_;
	p_stream->write_string(",bag_bind_silver=") << p->n_bag_bind_silver_;
	p_stream->write_string(",bag_bind_copper=") << p->n_bag_bind_copper_;
	p_stream->write_string(",bag_yuanbao=") << p->n_bag_tuanbao_;
	p_stream->write_string(",exchange_volume=") << p->n_ex_volume_;

	p_stream->write_string(",pet_packet_num=") << p->n16_pet_packet_size_;

	p_stream->write_string(",total_tax=") << p->n_total_tax_;	// 缴税总额
	p_stream->write_string(",remote_open_set=") << (DWORD)p->s_remote_open_set_;
	p_stream->write_string(",cur_title_id=") << p->u16_active_title_id[0];
	p_stream->write_string(",cur_title_id2=") << p->u16_active_title_id[1];
	p_stream->write_string(",cur_title_id3=") << p->u16_active_title_id[2];
	p_stream->write_string(",guild_id=") << p->dw_guild_id;
	p_stream->write_string(",team_id=") << p->dw_team_id_;

	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->login_time_))
	{
		p_stream->write_string(",login_time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");

		DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->logout_time_);
		p_stream->write_string(",logout_time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");

		DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->close_safe_guard_time_);
		p_stream->write_string(",close_safe_guard_time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");

		DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_time_get_mall_free_);
		p_stream->write_string(",get_mall_free_time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}
	else
	{
		print_message(_T("Error: Func DwordTime2DataTime() run error in db_handler::makesql_save_role_data()!!!!!\r\n"));
		ASSERT(0);
	}

	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->leave_guild_time_))
	{
		p_stream->write_string(",leave_guild_time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}

	p_stream->write_string(",own_instance_id=") << p->dw_own_instance_id_;
	p_stream->write_string(",own_instance_map_id=") << p->dw_own_instance_map_id_;
	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_instance_create_time_))
	{
		p_stream->write_string(",instance_create_time='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}

	p_stream->write_string(",online_time=") << p->n_online_time_;
	p_stream->write_string(",cur_online_time=") << p->n_cur_online_time_;

	p_stream->write_string(",script_data='");
	p_stream->write_blob(p->data_.dwData, sizeof(DWORD)*ESD_Role, p_conn);
	p_stream->write_string("'");

	p_stream->write_string(",stall_level=") << p->by_stall_level_;
	p_stream->write_string(",stall_daily_exp=") << p->n_stall_daily_exp_;
	p_stream->write_string(",stall_cur_exp=") << p->n_stall_cur_exp_;
	p_stream->write_string(",stall_last_time=") << p->dw_last_stall_time_;

	p_stream->write_string(",send_mail_num=") << p->n_send_mail_num_;
	p_stream->write_string(",master_id=") << p->dw_master_id_;
	p_stream->write_string(",masterprentice_forbid_time=") << p->dw_master_prentice_forbid_time_;

	p_stream->write_string(",map_limit_num=") << p->n_map_limit_num_;

	p_stream->write_string(",hang_num=") << p->n16_hang_num_;
	p_stream->write_string(",is_exp=") << p->b_exp_;
	p_stream->write_string(",is_brotherhood=") << p->b_brotherhood_;
	p_stream->write_string(",leave_exp=") << p->n_leave_exp_;
	p_stream->write_string(",leave_brotherhood=") << p->n_leave_brotherhood_;
	p_stream->write_string(",vigour=") << p->n_vigour_;
	p_stream->write_string(",role_help='");
	p_stream->write_blob(p->by_role_help_, sizeof(p->by_role_help_), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",role_talk='");
	p_stream->write_blob(p->by_talk_data_, sizeof(p->by_talk_data_), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",key_info='");
	p_stream->write_blob(&p->st_key_info_, sizeof(p->st_key_info_), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",day_clear='");
	p_stream->write_blob(p->by_role_day_clear, sizeof(p->by_role_day_clear), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",total_mastermoral=") << p->n_total_mater_moral_;
	p_stream->write_string(",kill_num=") << p->n_kill_num_;
	p_stream->write_string(",gift_step=") << p->n_gift_step_;
	p_stream->write_string(",gift_id=") << p->dw_gift_id_;
	p_stream->write_string(",gift_leaving_time=") << p->dw_gift_leave_time_;
	p_stream->write_string(",gift_get=") << p->b_gift_get_;
	p_stream->write_string(",gift_group_id=") << p->n_gift_id_;

	p_stream->write_string(",role_camp=") << p->e_role_camp;
	p_stream->write_string(",paimailimit=") << p->n_paimai_limit;
	p_stream->write_string(",banklimit=") << p->n_bank_limit;
	p_stream->write_string(",exploitslimit=") << p->n_shop_exploits_limit;

	p_stream->write_string(",today_online_tick=") << p->dw_today_online_tick_;
	p_stream->write_string(",history_vigour_cost=") << p->dw_history_vigour_cost_;
	p_stream->write_string(",perday_vigour_get_total=") << p->dw_perday_get_vigour_total;

	p_stream->write_string(",exbagstep=") << p->n16_exbag_step;
	p_stream->write_string(",exwarestep=") << p->n16_exware_step;
	
	p_stream->write_string(",signature_name='").write_string(p->sz_signature_name);
	p_stream->write_string("'");

	p_stream->write_string(",ware_size=") << p->n16WareSize;
	p_stream->write_string(",ware_gold=") << p->nWareGold;
	p_stream->write_string(",ware_silver=") << p->nWareSilver;
	p_stream->write_string(",ware_copper=") << p->nWareCopper;

	p_stream->write_string(",circle_quest='");
	p_stream->write_blob(&p->circle_quest_man_, sizeof(p->circle_quest_man_), p_conn);
	p_stream->write_string("'");

	p_stream->write_string(",yuanbao_exchange_num=") << p->n_yuanbao_exchange_num;
	p_stream->write_string(",achievemetn_point=") << p->n_achievement_point;
	p_stream->write_string(",achievemetn_num=") << p->n_achievement_num;

	p_stream->write_string(",forbid_talk_start=") << p->dw_forbid_talk_start_;
	p_stream->write_string(",forbid_talk_end=") << p->dw_forbid_talk_end_;

	p_stream->write_string(",graduate_num=") << p->n_graduates_num;
	
	p_stream->write_string(",destory_equip_count=") << p->dw_destory_equip_count;
	p_stream->write_string(",cur_1v1_score=") << p->st_1v1_score.n_cur_score;
	p_stream->write_string(",day_1v1_score=") << p->st_1v1_score.n_day_max_score;
	p_stream->write_string(",day_1v1_num=") << p->st_1v1_score.n_day_scroe_num;
	p_stream->write_string(",score_1v1_award=") << p->st_1v1_score.n16_score_award;

	p_stream->write_string(",exploits=") << p->n32_exploits;
	p_stream->write_string(",circle_quest_refresh=") << p->n_circle_quest_fresh;
	p_stream->write_string(",circle_quest_refresh_daymax=") << p->n_circle_quest_freshMax;


	p_stream->write_string(",active_num=") << p->n32_active_num;

	p_stream->write_string(",active_data='");
	p_stream->write_blob(&p->n32_active_data, sizeof(p->n32_active_data), p_conn);
	p_stream->write_string("'");

	p_stream->write_string(",active_receive='");
	p_stream->write_blob(&p->b_active_receive, sizeof(p->b_active_receive), p_conn);
	p_stream->write_string("'");
	
	p_stream->write_string(",guild_active_num=") << p->n32_guild_active_num;

	p_stream->write_string(",guild_active_data='");
	p_stream->write_blob(&p->n32_guild_active_data, sizeof(p->n32_guild_active_data), p_conn);
	p_stream->write_string("'");

	p_stream->write_string(",guild_active_receive='");
	p_stream->write_blob(&p->b_guild_active_receive, sizeof(p->b_guild_active_receive), p_conn);
	p_stream->write_string("'");


	p_stream->write_string(",justice=") << p->n32_rating;
	p_stream->write_string(",purpuredec=") << p->b_PurpureDec;
	p_stream->write_string(",circle_quest_perdaynumber=") << p->n_circle_quest_perdaynum;
	p_stream->write_string(",cooldownrevive_cd=") << p->n_CoolDownReviveCD;

	p_stream->write_string(",shihun=") << p->dw_shihun;
	p_stream->write_string(",perday_hang_getexp_timems=") << p->n_PerDayHangGetExpTimeMS;
	
	p_stream->write_string(",pet_xiulian_size=") << p->n16_pet_xiulian_size;
	
	p_stream->write_string(",god_level=") << p->n_god_level_;

	p_stream->write_string(",instance_pass=") << p->nInstanceData;
	p_stream->write_string(",shaodang_begin_time=") << p->nInstanceShaodang;
	p_stream->write_string(",shaodang_index=") << p->nSaodangIndex;

	
	p_stream->write_string(",spouse_id=") << p->dwSpouseID;
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;

	game_db_interface_->return_use_connect(p_conn);
}

// 保存家族信息
BOOL db_handler::save_role_clan_data(DWORD dw_role_id, IN LPVOID p_buffer, BOOL bChange, OUT LPVOID* pp_buffer)
{
	if (!bChange)
	{
		*pp_buffer = p_buffer;
		return TRUE;
	}
	*pp_buffer = reinterpret_cast<BYTE*>(p_buffer) + sizeof(s_db_repute_data);
	return common_update(dw_role_id, p_buffer, 1, sizeof(s_db_repute_data), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_clan_data));
}
VOID db_handler::makesql_update_clan_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	ASSERT(VALID_POINT(p_buffer));
	M_trans_pointer(p_repute_data, p_buffer, s_db_repute_data);

	p_stream->update_item("clan_data");

	p_stream->write_string("rep_xuanyuan=")		<< p_repute_data->n_reputation_[ECLT_XuanYuan];
	p_stream->write_string(",con_xuanyuan=")		<< p_repute_data->n_contribution_[ECLT_XuanYuan];
	p_stream->write_string(",act_cntxuanyuan=")	<< p_repute_data->n8_act_count_[ECLT_XuanYuan];

	p_stream->write_string(",rep_shennong=")		<< p_repute_data->n_reputation_[ECLT_ShenNong];
	p_stream->write_string(",con_shennong=")		<< p_repute_data->n_contribution_[ECLT_ShenNong];
	p_stream->write_string(",actcnt_shennong=")	<< p_repute_data->n8_act_count_[ECLT_ShenNong];

	p_stream->write_string(",rep_fuxi=")			<< p_repute_data->n_reputation_[ECLT_FuXi];
	p_stream->write_string(",con_fuxi=")			<< p_repute_data->n_contribution_[ECLT_FuXi];
	p_stream->write_string(",actcnt_fuxi=")		<< p_repute_data->n8_act_count_[ECLT_FuXi];

	p_stream->write_string(",rep_sanmiao=")		<< p_repute_data->n_reputation_[ECLT_SanMiao];
	p_stream->write_string(",con_sanmiao=")		<< p_repute_data->n_contribution_[ECLT_SanMiao];
	p_stream->write_string(",actcnt_sanmiao=")	<< p_repute_data->n8_act_count_[ECLT_SanMiao];

	p_stream->write_string(",rep_jiuli=")		<< p_repute_data->n_reputation_[ECLT_JiuLi];
	p_stream->write_string(",con_jiuli=")		<< p_repute_data->n_contribution_[ECLT_JiuLi];
	p_stream->write_string(",actcnt_jiuli=")		<< p_repute_data->n8_act_count_[ECLT_JiuLi];

	p_stream->write_string(",rep_yuezhi=")		<< p_repute_data->n_reputation_[ECLT_YueZhi];
	p_stream->write_string(",con_yuezhi=")		<< p_repute_data->n_contribution_[ECLT_YueZhi];
	p_stream->write_string(",actcnt_yuezhi=")	<< p_repute_data->n8_act_count_[ECLT_YueZhi];

	p_stream->write_string(",rep_nvwa=")			<< p_repute_data->n_reputation_[ECLT_NvWa];
	p_stream->write_string(",con_nvwa=")			<< p_repute_data->n_contribution_[ECLT_NvWa];
	p_stream->write_string(",actcnt_nvwa=")		<< p_repute_data->n8_act_count_[ECLT_NvWa];

	p_stream->write_string(",rep_gonggong=")		<< p_repute_data->n_reputation_[ECLT_GongGong];
	p_stream->write_string(",con_gonggong=")		<< p_repute_data->n_contribution_[ECLT_GongGong];
	p_stream->write_string(",actcnt_gonggong=")	<< p_repute_data->n8_act_count_[ECLT_GongGong];

	p_stream->write_string(",fame_mask=")		<< p_repute_data->u16_fame_mask;
	p_stream->write_string(",rep_reset_timestamp=")	<< p_repute_data->last_reset_time_;

	p_stream->where_item();
	p_stream->write_string("role_id=")			<< dw_role_id;

}

// 保存装备属性
BOOL db_handler::save_item_attribute(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer)
{
	return common_update(dw_role_id, p_buffer, n_count, sizeof(s_item_update), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_item_attribute), pp_buffer);
}
VOID db_handler::makesql_save_item_attribute(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, s_item_update);

	p_stream->update_item("item");
	p_stream->write_string("num=") << p->n16_num;
	p_stream->write_string(",use_times=") << p->n_use_times;
	p_stream->write_string(",owner_id=") << p->dw_owner_id;
	p_stream->write_string(",account_id=") << p->dw_account_id;
	p_stream->write_string(",container_type_id=") << p->by_conType;
	p_stream->write_string(",suffix=") << p->n16_index;
	p_stream->write_string(",bind=") << p->by_bind;

	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_bind_time);
	p_stream->write_string(",bind_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	p_stream->write_string(",script_data1=") << p->dw_script_data[0];
	p_stream->write_string(",script_data2=") << p->dw_script_data[1];

	p_stream->where_item();
	p_stream->write_string("serial=") << p->n64_serial;
}

// 新增技能
BOOL db_handler::insert_skill(DWORD dw_role_id, s_skill_save *p_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, sizeof(s_skill_save), 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_skill));
}

VOID db_handler::makesql_insert_skill(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, s_skill_save);

	p_stream->insert_item("skill");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",id=") << p->dw_id;
	p_stream->write_string(",bidden_level=") << p->n_learn_level_;
	p_stream->write_string(",self_level=") << p->n_self_level_;
	p_stream->write_string(",proficiency=") << p->n_proficiency_;
	p_stream->write_string(",cooldown=") << p->n_cool_down_;
}

// 删除技能
BOOL db_handler::delete_skill(DWORD dw_role_id, DWORD dw_skill_id)
{
	return common_update(dw_role_id, &dw_skill_id, 1, sizeof(DWORD), 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_skill));
}

VOID db_handler::makesql_delete_skill(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->delete_item("skill");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and id=") << (*(DWORD*)p_buffer);
}

//更新技能
BOOL db_handler::update_skill(DWORD dw_role_id, s_skill_save *p_buffer)
{
	LPVOID p_skill_data = p_buffer;
	return save_role_skill(dw_role_id, p_skill_data, 1, &p_skill_data);
}

// 保存角色技能
BOOL db_handler::save_role_skill(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer)
{
	return common_update(dw_role_id, p_buffer, n_count, sizeof(s_skill_save), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_role_skill), pp_buffer);
}
VOID db_handler::makesql_save_role_skill(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, s_skill_save);

	p_stream->replace_item("skill");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",id=") << p->dw_id;
	p_stream->write_string(",bidden_level=") << p->n_learn_level_;
	p_stream->write_string(",self_level=") << p->n_self_level_;
	p_stream->write_string(",proficiency=") << p->n_proficiency_;
	p_stream->write_string(",cooldown=") << p->n_cool_down_;
}

// 保存进入地图限制
BOOL db_handler::save_enter_map_limit(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer)
{
	//common_update(dw_role_id, NULL, 1, 0, (&db_handler::makesql_delete_all_map_limit));

	BYTE *p_current = (BYTE*)p_buffer;
	s_enter_map_limit* p_enter_map_limit;
	for(int i = 0; i < n_count; i++)
	{
		p_enter_map_limit = (s_enter_map_limit*)p_current;
		common_update(dw_role_id, p_enter_map_limit, 1, sizeof(s_enter_map_limit), 
			MakeDelegateOfDBHandler(&db_handler::makesql_save_map_limit));
		p_current += sizeof(s_enter_map_limit);
	}

	*pp_buffer = p_current;
	return TRUE;
}
VOID db_handler::makesql_save_map_limit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	M_trans_pointer(p, p_reserved, s_enter_map_limit);

	p_stream->update_item("map_limit");
	p_stream->write_string("enter_num=") << p->dw_enter_num_;
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and ");
	p_stream->write_string("map_id=") << p->dw_map_id_;
}

VOID db_handler::makesql_delete_all_map_limit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	p_stream->delete_item("map_limit");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 保存角色BUFF
BOOL db_handler::save_role_buff(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer)
{
	// 先删除buff
	common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_role_buff));

	BYTE		*p_current = (BYTE*)p_buffer;
	s_buff_save *p_buff_save;
	for(int i=0; i<n_count; ++i)
	{
		p_buff_save = (s_buff_save *)p_current;
		common_update(dw_role_id, p_buff_save, 1, sizeof(s_buff_save), 
			MakeDelegateOfDBHandler(&db_handler::makesql_save_role_buff));
		p_current = p_buff_save->by_data_ + p_buff_save->n_modifier_num_ * sizeof(DWORD);
	}

	*pp_buffer = p_current;
	
	return TRUE;
}
VOID db_handler::makesql_delete_role_buff(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	p_stream->delete_item("buff");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}
VOID db_handler::makesql_save_role_buff(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, s_buff_save);

	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->insert_item("buff");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",src_unit_id=") << p->dw_src_unit_id_;
	p_stream->write_string(",src_skill_id=") << p->dw_src_skill_id_;
	p_stream->write_string(",item_type_id=") << p->dw_item_type_id_;
	p_stream->write_string(",item_serial_id=") << p->n_serial_;

	p_stream->write_string(",buff_id=") << p->dw_buff_id_;
	p_stream->write_string(",cur_tick=") << p->n_persist_tick_;
	p_stream->write_string(",level=") << p->n_level_;
	p_stream->write_string(",cur_lap_times=") << p->n_cur_lap_times_;

	if(p->n_modifier_num_ > 0)
	{
		p_stream->write_string(",effect_skill_id='");
		p_stream->write_blob(p->by_data_, p->n_modifier_num_ * sizeof(DWORD), p_conn);
		p_stream->write_string("'");
	}

	game_db_interface_->return_use_connect(p_conn);
}

// 保存角色称号
BOOL db_handler::save_role_title(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer)
{
	return common_update(dw_role_id, p_buffer, n_count, sizeof(s_title_save), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_role_title), pp_buffer);
}
VOID db_handler::makesql_save_role_title(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, s_title_save);

	p_stream->update_item("title");
	p_stream->write_string("count=") << p->dw_state_mark_;

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and id=") << p->n_title_id_;
}

// 完成成就
BOOL db_handler::save_complate_achievement(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer)
{
	return common_update(dw_role_id, p_buffer, n_count, sizeof(s_achievement_complate_save), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_complate_achievement), pp_buffer);
}
VOID db_handler::makesql_save_complate_achievement(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, s_achievement_complate_save);

	p_stream->replace_item("achievement");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",id=") << p->dw_id;

	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_date);
	p_stream->write_string(",date='").write_string(sz_time_buffer_);
	p_stream->write_string("'");


}

// 成就进度
BOOL db_handler::save_process_achievement(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer)
{
	return common_update(dw_role_id, p_buffer, n_count, sizeof(s_achievement_process_save), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_process_achievement), pp_buffer);

}

VOID db_handler::makesql_save_process_achievement(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	M_trans_pointer(p, p_reserved, s_achievement_process_save);

	p_stream->replace_item("achievement_progress");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",id=") << p->dw_id;
	p_stream->write_string(",counter=") << p->dw_count;

	//DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_date);
	//p_stream->write_string(",date='").write_string(sz_time_buffer_);
	//p_stream->write_string("'");
}

// 副本进度
BOOL db_handler::save_process_instance(DWORD	dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer)
{
	return common_update(dw_role_id, p_buffer, n_count, sizeof(s_inst_process), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_process_instance), pp_buffer);
}

VOID db_handler::makesql_save_process_instance(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	M_trans_pointer(p, p_reserved, s_inst_process);

	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->insert_item("inst_process");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",map_id=") << p->dw_map_id;
	p_stream->write_string(",mode=") << p->n_mode;
	p_stream->write_string(",type=") << p->n_type;

	p_stream->write_string(",door_process='").write_blob(p->st_door_state, sizeof(p->st_door_state), p_conn).write_string("'");
	p_stream->write_string(",creature_process='").write_blob(p->dw_creature_id, sizeof(p->dw_creature_id), p_conn).write_string("'");

	game_db_interface_->return_use_connect(p_conn);

}

// 签到数据
BOOL db_handler::save_sign_data(DWORD	dw_role_id, LPVOID p_buffer, OUT LPVOID *pp_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, sizeof(tagRoleSignData), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_sign_data), pp_buffer);
}

VOID db_handler::makesql_save_sign_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, tagRoleSignData);

	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->replace_item("sign");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",data='").write_blob(p->st_sign_info, sizeof(p->st_sign_info), p_conn).write_string("'");
	p_stream->write_string(",reward_data='").write_blob(p->b_reward, sizeof(p->b_reward), p_conn).write_string("'");
	p_stream->write_string(",mianqian_time=") << p->n16_mianqian_time;

	game_db_interface_->return_use_connect(p_conn);

}

BOOL db_handler::save_huenjing_data(DWORD	dw_role_id, LPVOID p_buffer, OUT LPVOID *pp_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, sizeof(tagRoleHuenJingData), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_huenjing_data), pp_buffer);
}

VOID db_handler::makesql_save_huenjing_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, tagRoleHuenJingData);

	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->replace_item("HuenJing");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",bag_temp_data='").write_blob(p->s_huenjing_bag_temp, sizeof(p->s_huenjing_bag_temp), p_conn).write_string("'");
	p_stream->write_string(",bag_data='").write_blob(p->s_huenjing_bag, sizeof(p->s_huenjing_bag), p_conn).write_string("'");
	p_stream->write_string(",role_level_data='").write_blob(p->s_huenjing_role_level, sizeof(p->s_huenjing_role_level), p_conn).write_string("'");
	p_stream->write_string(",role_title_data='").write_blob(p->s_huenjing_role_titel, sizeof(p->s_huenjing_role_titel), p_conn).write_string("'");
	p_stream->write_string(",cur_artisan=") << p->byCurArtisan;

	game_db_interface_->return_use_connect(p_conn);

}

BOOL db_handler::save_reward_data(DWORD dw_role_id, LPVOID p_buffer, OUT LPVOID *pp_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, sizeof(tagRewardData) * RF_NUM * MAX_REWARD_NUMBER, 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_reward_data), pp_buffer);
}

VOID db_handler::makesql_save_reward_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, tagRewardData);

	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->replace_item("reward");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",reward_data='").write_blob(p, sizeof(tagRewardData) * RF_NUM * MAX_REWARD_NUMBER, p_conn).write_string("'");

	game_db_interface_->return_use_connect(p_conn);
}
// 删除副本进度
BOOL db_handler::delete_inst_process_type(INT16 n_type)
{
	return common_update(n_type, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_process_instance_type));
}

VOID db_handler::makesql_delete_process_instance_type(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	p_stream->delete_item("inst_process");
	p_stream->where_item();
	p_stream->write_string("type=") << dw_role_id;
}

// 删除副本进度
BOOL db_handler::delete_inst_process(DWORD	dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_process_instance));
}

VOID db_handler::makesql_delete_process_instance(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	p_stream->delete_item("inst_process");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 新增任务
BOOL db_handler::insert_quest(DWORD dw_role_id, LPVOID p_accept_quest)
{
	return common_update(dw_role_id, p_accept_quest, 1, sizeof(tagAcceptQuest), 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_quest));
}
VOID db_handler::makesql_insert_quest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_accept_quest)
{
	M_trans_pointer(p, p_accept_quest, tagAcceptQuest);

	p_stream->insert_item("quest");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",quest_id=") << p->u16QuestID;
	p_stream->write_string(",start_time=") << p->dwAcceptTime;
	p_stream->write_string(",new_flag=") << p->nNewFlag;
}

// 保存任务
BOOL db_handler::save_quest(DWORD dw_role_id, LPVOID p_task, int n_count, OUT LPVOID *pp_buffer)
{
	return common_update(dw_role_id, p_task, n_count, sizeof(tagQuestSave), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_role_task), pp_buffer);
}
VOID db_handler::makesql_save_role_task(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_task)
{
	tagQuestSave *p = (tagQuestSave *)p_task;

	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->replace_item("quest");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",quest_id=") << p->u16QuestID;
	p_stream->write_string(",quest_flag=") << p->dwQuestFlag;
	p_stream->write_string(",start_time=") << p->dwAcceptTime;
	p_stream->write_string(",new_flag=") << p->u16Quest_NewFlag;//gx add
	for(int i=0; i<QUEST_CREATURES_COUNT; ++i)
	{
		p_stream->write_string(",monster") << i;
		p_stream->write_string("_num=") << (INT16)p->n16MonsterNum[i];
	}

	p_stream->write_string(",script_data='");
	p_stream->write_blob(p->Data.dwData, sizeof(DWORD)*ESD_Quest, p_conn);
	p_stream->write_string("'");

	if(p->DynamicTarget.eTargetType != EQTT_NULL)
	{
		//任务动态目标
		p_stream->write_string(",dynamic_target='");
		p_stream->write_blob(&p->DynamicTarget, sizeof(tagQuestDynamicTarget), p_conn);
		p_stream->write_string("'");
	}
	if( p->DynamicReward.rew_item[0] || p->DynamicReward.rew_choice_item[0] )
	{
		//任务动态奖励
		p_stream->write_string(",dynamic_reward='");
		p_stream->write_blob(&p->DynamicReward, sizeof(tagQuestDynamicReward), p_conn );
		p_stream->write_string("'");
	}
	if( p->ScriptQuestVar.Number )
	{//脚本完成条件
		p_stream->write_string(",scriptquest_var='");
		p_stream->write_blob(&p->ScriptQuestVar, sizeof(tagScriptQuestVar), p_conn );
		p_stream->write_string("'");
	}

	game_db_interface_->return_use_connect(p_conn);
}

// 保存完成任务
BOOL db_handler::save_quest_done(DWORD dw_role_id, LPVOID p_task, int n_count)
{
	return common_update(dw_role_id, p_task, n_count, sizeof(tagQuestDoneSave), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_role_task_done));
}

VOID db_handler::makesql_save_role_task_done(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_task)
{
	tagQuestDoneSave *p = (tagQuestDoneSave *)p_task;

	p_stream->replace_item("quest_done");
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(",quest_id=") << QuestIDHelper::RestoreID(p->u16QuestID);
	p_stream->write_string(",start_time=") << p->dwStartTime;
	p_stream->write_string(",times=") << p->nTimes;
	p_stream->write_string(",new_flag=") <<p->u16QuestFalg;
}

// 删除任务
BOOL db_handler::delete_quest(DWORD dw_role_id, DWORD dw_task_id)
{
	return common_update(dw_role_id, &dw_task_id, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_quest));
}

VOID db_handler::makesql_delete_quest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_task_id)
{
	p_stream->delete_item("quest");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
	p_stream->write_string(" and quest_id=") << (*(DWORD *)p_task_id);
}
//gx add 删除每日任务
BOOL db_handler::delte_all_dayquest(DWORD nNewFlag)
{
	return common_update(INVALID_VALUE, &nNewFlag, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_all_day_quest));
}
VOID db_handler::makesql_delete_all_day_quest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_NewFlag)
{
	p_stream->delete_item("quest");
	p_stream->where_item();
	p_stream->write_string("new_flag=") << (*(INT *)p_NewFlag);
}
BOOL db_handler::delte_all_dayquest_done(DWORD nNewFlag)
{
	return common_update(INVALID_VALUE, &nNewFlag, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_all_day_quest_done));
}
VOID db_handler::makesql_delete_all_day_quest_done(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_NewFlag)
{
	p_stream->delete_item("quest_done");
	p_stream->where_item();
	p_stream->write_string("new_flag=") << (*(INT *)p_NewFlag);
}
//
BOOL db_handler::delete_all_teamshare_quest(DWORD nNewFlag)
{
	return common_update(INVALID_VALUE, &nNewFlag, 1, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_all_teamshare_quest));
}
VOID db_handler::makesql_delete_all_teamshare_quest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_NewFlag)
{
	p_stream->delete_item("quest");
	p_stream->where_item();
	p_stream->write_string("new_flag=") << (*(INT *)p_NewFlag);
}
// 新增角色名片
BOOL db_handler::insert_visiting_card(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_visiting_card));
}
VOID db_handler::makesql_insert_visiting_card(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, PVOID pDum)
{
	ASSERT(!VALID_POINT(pDum));

	p_stream->insert_item("visiting_card");
	p_stream->write_string("role_id=") << dw_role_id;
}

// 更新角色名片
BOOL db_handler::update_visiting_card(DWORD dw_role_id, const PVOID p_buffer)
{
	return common_update(dw_role_id, p_buffer, 1, sizeof(tagVCardData), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_visiting_card));
}
VOID db_handler::makesql_update_visiting_card(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, tagVCardData);
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->update_item("visiting_card");

	p_stream->write_string("level=") << (int)p->dwLevel;
	p_stream->write_string(",job=") << (int)p->dwJob;
	p_stream->write_string(",mate_role_id=") << (int)p->dwMateRoleID;
	p_stream->write_string(",guild_id=") << (int)p->dwFactionID;
	p_stream->write_string(",position=") << (int)p->dwPosition;

	//p_stream->write_string(",visibility=") << p->customVCardData.bVisibility;
	//p_stream->write_string(",sex=") << p->customVCardData.bySex;
	p_stream->write_string(",constellation=") << p->customVCardData.byConstellation;
	//p_stream->write_string(",chinesezodiac=") << p->customVCardData.byChineseZodiac;
	//p_stream->write_string(",area=") << p->customVCardData.byArea;
	//p_stream->write_string(",province=") << p->customVCardData.byProvince;
	p_stream->write_string(",city='").write_string(p->customVCardData.chCity, p_conn).write_string("'");
	//p_stream->write_string(",head_url='").write_string(reinterpret_cast<TCHAR *>(p->customVCardData.byData), p_conn).write_string("'");
	p_stream->write_string(",signature='").write_string(reinterpret_cast<TCHAR *>(&(p->customVCardData.byData)), p_conn).write_string("'");

	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;

	game_db_interface_->return_use_connect(p_conn);
}

// 删除角色名片
BOOL db_handler::delete_visiting_card(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_visiting_card));
}

VOID db_handler::makesql_delete_visiting_card(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	ASSERT(!VALID_POINT(p_reserved));
	p_stream->delete_item("visiting_card");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 新增家族数据
BOOL db_handler::insert_clan_data(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_clan_data));
}
VOID db_handler::makesql_insert_clan_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	ASSERT(!VALID_POINT(p_reserved));

	p_stream->insert_item("clan_data");
	p_stream->write_string("role_id=") << dw_role_id;
}

// 删除家族数据
BOOL db_handler::delete_clan_data(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_clan_data));
}

VOID db_handler::makesql_delete_clan_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	ASSERT(!VALID_POINT(p_reserved));

	p_stream->delete_item("clan_data");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 新增签到数据
BOOL db_handler::insert_sign_data(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_sign_data));
}

VOID db_handler::makesql_insert_sign_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	p_stream->insert_item("sign");
	p_stream->write_string("role_id=") << dw_role_id;
}

BOOL db_handler::insert_huenjing_data(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_huenjing_data));
}

VOID db_handler::makesql_insert_huenjing_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	p_stream->insert_item("HuenJing");
	p_stream->write_string("role_id=") << dw_role_id;
}

BOOL db_handler::insert_reward_data(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_reward_data));
}

VOID db_handler::makesql_insert_reward_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	p_stream->insert_item("reward");
	p_stream->write_string("role_id=") << dw_role_id;
}

BOOL db_handler::insert_mounts_data(DWORD dw_role_id)
{
	return common_update(dw_role_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_mounts_data));
}

VOID db_handler::makesql_insert_mounts_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved)
{
	p_stream->insert_item("mounts");
	p_stream->write_string("role_id=") << dw_role_id;
}
// 存储某个活动数据
BOOL db_handler::save_activity(DWORD dw_id, LPVOID pActivityData)
{
	return common_update(dw_id, pActivityData, 1, sizeof(s_activity_data), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_activity));
}
VOID db_handler::makesql_save_activity(odbc::sql_language_disposal *p_stream, DWORD dw_id, LPVOID p_activity_data)
{
	s_activity_data *p = (s_activity_data *)p_activity_data;
	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();
	p_stream->replace_item("activity");
	p_stream->write_string("id=") << dw_id;
	p_stream->write_string(",script_data='");
	p_stream->write_blob(&p->data.dwData, sizeof(DWORD)*ESD_Activity, p_conn);
	p_stream->write_string("'");
	game_db_interface_->return_use_connect(p_conn);
}
// 保存邮件物品
BOOL db_handler::update_mail_item(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer)
{
	return common_update(dw_role_id, p_buffer, n_count, sizeof(s_item_update), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_item_attribute), pp_buffer);
}

// 保存邮件
BOOL db_handler::save_mail(NET_DB2C_save_mail* p_save_mail)
{
	return common_update(INVALID_VALUE, p_save_mail, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_mail));
}
VOID db_handler::makesql_save_mail(odbc::sql_language_disposal *p_stream, DWORD dw_mail_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, NET_DB2C_save_mail);

	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->insert_item("mail");

	p_stream->write_string("mail_id=") << p->s_mail_base_.dwID;
	p_stream->write_string(",mail_name='").write_blob(p->str_name_, (_tcsclen(p->str_name_)+1) * sizeof(TCHAR), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",mail_content='").write_blob(p->sz_content_, (_tcsclen(p->sz_content_)+1) * sizeof(TCHAR), p_conn);
	p_stream->write_string("'");
	p_stream->write_string(",send_role_id=") << p->s_mail_base_.dwSendRoleID;
	p_stream->write_string(",recv_role_id=") << p->s_mail_base_.dwRecvRoleID;
	p_stream->write_string(",is_at_once=") << p->s_mail_base_.bAtOnce;
	p_stream->write_string(",is_send=") << p->s_mail_base_.bSend;
	p_stream->write_string(",is_read=") << p->s_mail_base_.bReed;
	p_stream->write_string(",is_withdrawal=") << p->s_mail_base_.bWithdrawal;
	p_stream->write_string(",solve=") << p->s_mail_base_.dwSolve;
	p_stream->write_string(",give_money=") << p->s_mail_base_.dwGiveMoney;
	p_stream->write_string(",moneytype=") << p->s_mail_base_.byType;
	p_stream->write_string(",item_serial='").write_blob(p->s_mail_base_.n64ItemSerial, sizeof(p->s_mail_base_.n64ItemSerial), p_conn);
	p_stream->write_string("'");
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->s_mail_base_.dwSendTime);
	p_stream->write_string(",send_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->s_mail_base_.dwRecvTime);
	p_stream->write_string(",recv_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	p_stream->write_string(",yuanbao_type=") << p->s_mail_base_.n_yuanbao_type;

	game_db_interface_->return_use_connect(p_conn);
}


//  删除邮件
BOOL db_handler::delete_mail(NET_DB2C_delete_mail* p_delete_mail)
{
	return common_update(p_delete_mail->dw_mail_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_mail));
}
VOID db_handler::makesql_delete_mail(odbc::sql_language_disposal* p_stream, DWORD dw_mail_id, LPVOID p_buffer)
{
	p_stream->delete_item("mail");
	p_stream->where_item();
	p_stream->write_string("mail_id=") << dw_mail_id;
}

// 更新邮件
BOOL db_handler::update_mail(NET_DB2C_update_mail* p_update_mail)
{
	return common_update(p_update_mail->s_mail_base.dwID, &p_update_mail->s_mail_base, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_mail));
}
VOID db_handler::makesql_update_mail(odbc::sql_language_disposal* p_stream, DWORD dw_mail_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, tagMailBase);

	p_stream->update_item("mail");
	p_stream->write_string("send_role_id=") << p->dwSendRoleID;
	p_stream->write_string(",recv_role_id=") << p->dwRecvRoleID;
	p_stream->write_string(",is_at_once=") << p->bAtOnce;
	p_stream->write_string(",is_send=") << p->bSend;
	p_stream->write_string(",is_read=") << p->bReed;
	p_stream->write_string(",is_withdrawal=") << p->bWithdrawal;
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dwSendTime);
	p_stream->write_string(",send_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dwRecvTime);
	p_stream->write_string(",recv_time='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
	p_stream->write_string(",solve=") << p->dwSolve;
	p_stream->write_string(",give_money=") << p->dwGiveMoney;
	p_stream->where_item();
	p_stream->write_string("mail_id=") << dw_mail_id;
}

// 更改任务金钱
BOOL db_handler::change_role_money(NET_DB2C_change_role_money* p_chang_role_money)
{
	return common_update(p_chang_role_money->dw_role_id_, p_chang_role_money, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_change_role_money));
}

VOID db_handler::makesql_change_role_money(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, NET_DB2C_change_role_money);

	p_stream->update_item("role_data");
	p_stream->write_string("bag_gold += ") << p->n_gold_;
	p_stream->write_string("bag_silver += ") << p->n_silver_;
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}

// 更新角色发邮件计数
BOOL db_handler::update_role_send_mail_count(NET_DB2C_update_role_send_num* p_update_send_num)
{
	if(p_update_send_num->dw_role_id != INVALID_VALUE)
	{
		return common_update(p_update_send_num->dw_role_id, p_update_send_num, 1, 0, 
			MakeDelegateOfDBHandler(&db_handler::makesql_update_role_send_mail_count));
	}
	else
	{
		return common_update(p_update_send_num->dw_role_id, p_update_send_num, 1, 0, 
			MakeDelegateOfDBHandler(&db_handler::makesql_clear_role_send_mail_count));
	}
}
VOID db_handler::makesql_update_role_send_mail_count(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("role_data");
	p_stream->write_string("send_mail_num = send_mail_num+1");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}
VOID db_handler::makesql_clear_role_send_mail_count(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->update_item("role_data");
	p_stream->write_string("send_mail_num = 0");
}
