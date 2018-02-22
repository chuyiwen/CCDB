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
#include "db_handler.h"


BOOL db_handler::insert_pet_soul( const s_db_pet_soul_create* p_create, DWORD& dw_pet_id)
{
	BOOL bRtv = common_update(INVALID_VALUE, (LPVOID)p_create, 1, sizeof(s_db_pet_soul_create), 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_pet_soul));
	if (bRtv)
	{
		dw_pet_id = dw_max_pet_id_ + 1;
		++dw_max_pet_id_;
		++n_pet_count_;
	}
	return bRtv;
}

VOID db_handler::makesql_insert_pet_soul( odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer )
{
	M_trans_pointer(p, p_buffer, s_db_pet_soul_create);

	odbc::tag_mysql_connect* pCon = game_db_interface_->get_idlesse_connect();

	p_stream->insert_item("pet_data");
	p_stream->write_string("pet_id=") << dw_max_pet_id_ + 1;
	p_stream->write_string(",pet_name='").write_string(p->sz_name_, pCon).write_string("'");
	p_stream->write_string(",master_id=") << p->dw_master_id;
	p_stream->write_string(",type_id=") << p->dw_proto_id;
	p_stream->write_string(",quality=") << p->n_quality_;
	p_stream->write_string(",aptitude=") << p->n_aptitude_;
	p_stream->write_string(",potential=") << p->n_potential_;
	p_stream->write_string(",cur_spirit=") << p->n_spirte_;

	game_db_interface_->return_use_connect(pCon);
}

BOOL db_handler::delete_pet_soul(DWORD dw_pet_id)
{
	return common_update(dw_pet_id, NULL, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_pet_soul));
}

VOID db_handler::makesql_delete_pet_soul(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->delete_item("pet_data");
	p_stream->where_item();
	p_stream->write_string("pet_id=") << dw_role_id;
}

BOOL db_handler::delete_pet_equip(DWORD dw_pet_id)
{
	return delete_role_items(dw_pet_id);
}

BOOL db_handler::load_pets(OUT PVOID& p_buffer, DWORD dw_master_id, int& n_count)
{
	DWORD dwPets[MAX_PETSOUL_NUM];
	memset(dwPets, INVALID_VALUE, sizeof(dwPets));
	if (!load_role_all_pet_id(dwPets, dw_master_id, n_count) || 0 == n_count)
	{
		return TRUE;
	}
	
	for (int i=0; i<n_count; ++i)
	{
		if (!load_one_pet(p_buffer, dwPets[i]))
		{
			n_count = i;
			break;
		}
	}

	return TRUE;
}
BOOL db_handler::load_pet_soul(LPVOID p_buffer, DWORD dw_pet_id)
{
	int n_count = 0;
	BOOL bRtv = common_select(p_buffer, &n_count, dw_pet_id, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_pet_soul), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_pet_soul));
	ASSERT(n_count);
	return bRtv;
}

VOID db_handler::makesql_load_pet_soul( odbc::sql_language_disposal *p_stream, DWORD dwPetID, DWORD dwMasterID )
{
	p_stream->select_item("pet_data", "pet_id,	pet_name,	master_id,	type_id,"
									"quality,	aptitude,	potential,	cur_spirit,"
									"cur_exp,	step,	grade,	talent_count,"
									"wuxing_energy,	pet_state,	pet_lock, rename_count, happy_value, color, strength, agility, innerforce, attpoint,"
									"time_type, mode_type, xiulian_time");
	p_stream->where_item();

	ASSERT(!VALID_VALUE(dwMasterID));
	
	p_stream->write_string("pet_id=") << dwPetID;
}

VOID db_handler::procres_load_pet_soul( OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result )
{
	M_trans_pointer(pPetAtt, p_buffer, s_db_pet_att);

	*p_number = p_result->get_row_count();

	ASSERT(1 == *p_number);
		
	pPetAtt->dw_pet_id_	= (*p_result)[0].get_dword();
	(*p_result)[1].get_tchar(pPetAtt->sz_name_, X_SHORT_NAME);
	
	pPetAtt->dw_master_id_	= (*p_result)[2].get_dword();				
	pPetAtt->dw_proto_id_	= (*p_result)[3].get_dword();	
	pPetAtt->n_quality_	= (*p_result)[4].get_int();
	pPetAtt->n_aptitude_	= (*p_result)[5].get_int();
	pPetAtt->n_potential_	= (*p_result)[6].get_int();
	pPetAtt->n_spirit_	= (*p_result)[7].get_int();
	pPetAtt->n_exp_cur_	= (*p_result)[8].get_int();
	pPetAtt->n_step_		= (*p_result)[9].get_int();
	pPetAtt->n_grade_		= (*p_result)[10].get_int();
	pPetAtt->n_talent_count_= (*p_result)[11].get_int();
	pPetAtt->n_wuxing_energy_= (*p_result)[12].get_int();
	pPetAtt->by_pet_state_	= (*p_result)[13].get_int();
	pPetAtt->b_locked_	= (*p_result)[14].get_bool();
	pPetAtt->n_rename_count_=(*p_result)[15].get_int();
	pPetAtt->n_happy_value_ = (*p_result)[16].get_int();
	pPetAtt->n_color		= (*p_result)[17].get_int();
	pPetAtt->n_strength		= (*p_result)[18].get_int();
	pPetAtt->n_agility		= (*p_result)[19].get_int();
	pPetAtt->n_innerForce	= (*p_result)[20].get_int();
	pPetAtt->n_att_point	= (*p_result)[21].get_int();
	pPetAtt->dw_time_type	= (*p_result)[22].get_dword();
	pPetAtt->dw_mode_type	= (*p_result)[23].get_dword();
	pPetAtt->dw_xiulian_time = (*p_result)[24].get_dword();
}
BOOL db_handler::load_role_all_pet_id( DWORD dwPetIDs[], DWORD dw_master_id, int& n_count )
{
	PVOID p_buffer = (PVOID)dwPetIDs;
	return common_select(p_buffer, &n_count,INVALID_VALUE, dw_master_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_role_all_pet_id), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_role_all_pet_id));
}

VOID db_handler::makesql_load_role_all_pet_id( odbc::sql_language_disposal *p_stream, DWORD dwPetID, DWORD dwMasterID )
{
	p_stream->select_item("pet_data", "pet_id");
	p_stream->where_item();
	p_stream->write_string("master_id=") << dwMasterID;
}

VOID db_handler::procres_load_role_all_pet_id( OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result )
{
	M_trans_pointer(pPetIDs, p_buffer, DWORD);

	*p_number = p_result->get_row_count();

	for(int i=0; i<*p_number; ++i)
	{
		pPetIDs[i] = (*p_result)[0].get_dword();
		p_result->next_row();
	}

	p_buffer = (LPVOID)(pPetIDs + *p_number);
}
BOOL db_handler::load_pet_skill(OUT LPVOID& p_buffer, DWORD dw_pet_id, int& n_count)
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_pet_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_pet_skill), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_pet_skill));
}

VOID db_handler::makesql_load_pet_skill( odbc::sql_language_disposal *p_stream, DWORD dwMasterID, DWORD dwPetID )
{
	p_stream->select_item("pet_skill", "pet_skill_type_id,para1,para2");
	p_stream->where_item();
	p_stream->write_string("pet_id=")<<dwPetID;
}

VOID db_handler::procres_load_pet_skill( OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result )
{
	M_trans_pointer(pPetSkills, p_buffer, s_db_pet_skill);
	*p_number = p_result->get_row_count();

	for (int i=0; i<*p_number; ++i)
	{
		pPetSkills[i].dw_data_id		= (*p_result)[0].get_dword();
		pPetSkills[i].n_para1_		= (*p_result)[1].get_dword();
		pPetSkills[i].n_para2_		= (*p_result)[2].get_dword();
		p_result->next_row();
	}
	p_buffer = &pPetSkills[*p_number];
}

BOOL db_handler::delete_pet_skill(DWORD dwPetSkillID, DWORD dw_pet_id)
{
	return common_update(dw_pet_id, &dwPetSkillID, 1, sizeof(DWORD), 
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_pet_skill));
}

VOID db_handler::makesql_delete_pet_skill( odbc::sql_language_disposal *p_stream, DWORD dwPetID, LPVOID p_buffer )
{
	DWORD dwPetSkillID = *((DWORD*)p_buffer);
	p_stream->delete_item("pet_skill");
	p_stream->where_item();
	if (VALID_POINT(dwPetSkillID))
	{
		p_stream->write_string("pet_skill_type_id=")<<dwPetSkillID;
		p_stream->write_string(" and ");
	}
	
	p_stream->write_string("pet_id=")<<dwPetID;
}

BOOL db_handler::insert_pet_skill( PVOID p_buffer, DWORD dw_pet_id )
{
	return common_update(dw_pet_id, p_buffer, 1, sizeof(s_db_pet_skill), 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_pet_skill));
}

VOID db_handler::makesql_insert_pet_skill( odbc::sql_language_disposal *p_stream, DWORD dwPetID, LPVOID p_buffer )
{
	M_trans_pointer(pSkillData, p_buffer, s_db_pet_skill);

	p_stream->insert_item("pet_skill");
	p_stream->write_string("pet_id=") << dwPetID;
	p_stream->write_string(",pet_skill_type_id=") << pSkillData->dw_data_id;
	p_stream->write_string(",para1=") << (DWORD)pSkillData->n_para1_;
	p_stream->write_string(",para2=") << (DWORD)pSkillData->n_para2_;

}

BOOL db_handler::update_pet_skill( PVOID &p_buffer, const int n_count, DWORD dw_pet_id )
{
	return common_update(dw_pet_id, p_buffer, n_count, sizeof(s_db_pet_skill), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_pet_skill), &p_buffer);
}

VOID db_handler::makesql_update_pet_skill( odbc::sql_language_disposal *p_stream, DWORD dwPetID, LPVOID p_buffer )
{
	M_trans_pointer(pSkillData, p_buffer, s_db_pet_skill);

	p_stream->update_item("pet_skill");
	p_stream->write_string("para1=") << (DWORD)pSkillData->n_para1_;
	p_stream->write_string(",para2=") << (DWORD)pSkillData->n_para2_;

	p_stream->where_item();
	p_stream->write_string("pet_id=") << dwPetID;
	p_stream->write_string(" and pet_skill_type_id=") << pSkillData->dw_data_id;

}
BOOL db_handler::load_pet_equip( OUT LPVOID& p_buffer, DWORD dw_pet_id, int& n_count )
{
	return common_select(p_buffer, &n_count, INVALID_VALUE, dw_pet_id, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_pet_equip), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_items));
}

VOID db_handler::makesql_load_pet_equip( odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dwPetID)
{
	p_stream->clear();
	p_stream->write_string("select ");
	init_item_load_field(p_stream);
	p_stream->write_string(" from item left join equip using(serial)");
	p_stream->where_item();
	p_stream->write_string("owner_id=") << dwPetID;
}
BOOL db_handler::load_one_pet(OUT PVOID &p_buffer, DWORD dw_pet_id )
{
	M_trans_pointer(pPetLoad, p_buffer, s_db_pet);

	if (!load_pet_soul(&pPetLoad->pet_att, dw_pet_id))
	{
		return FALSE;
	}
	PVOID pVarLenData = pPetLoad->by_data;
	load_pet_skill(pVarLenData, dw_pet_id, pPetLoad->n_pet_skill_num);
	load_pet_equip(pVarLenData, dw_pet_id, pPetLoad->n_pet_equip_num);

	p_buffer = pVarLenData;
	return TRUE;
}
BOOL db_handler::save_pets( DWORD dw_role_id, IN LPVOID p_buffer, int n_count, OUT LPVOID* pp_buffer )
{
	M_trans_pointer(pDBPet, p_buffer, s_db_pet);

	while(n_count--)
	{
		pDBPet = (s_db_pet*)p_buffer;
		DWORD dwPetID = pDBPet->pet_att.dw_pet_id_;
		save_one_pet(p_buffer, dwPetID);
	}

	*pp_buffer = p_buffer;
	return TRUE;
}
BOOL db_handler::save_one_pet( OUT PVOID& p_buffer, DWORD dw_pet_id)
{
	M_trans_pointer(pDBPet, p_buffer, s_db_pet);
	
	if (!update_pet_attribute(&pDBPet->pet_att, pDBPet->pet_att.dw_pet_id_))
	{
		return FALSE;
	}
	
	PVOID pVarData = pDBPet->by_data;
	update_pet_skill(pVarData, pDBPet->n_pet_skill_num, dw_pet_id);

	p_buffer = pVarData;

	return TRUE;
}
BOOL db_handler::update_pet_attribute( const s_db_pet_att* p_att, DWORD& dw_pet_id)
{
	return common_update(INVALID_VALUE, (LPVOID)p_att, 1, sizeof(s_db_pet_att), 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_pet_attribute));
}
VOID db_handler::makesql_update_pet_attribute( odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer )
{
	M_trans_pointer(p, p_buffer, s_db_pet_att);

	odbc::tag_mysql_connect* pCon = game_db_interface_->get_idlesse_connect();

	p_stream->update_item("pet_data");
	p_stream->write_string("pet_name='").write_string(p->sz_name_, pCon).write_string("'");
	p_stream->write_string(",master_id=") << p->dw_master_id_;
	p_stream->write_string(",type_id=") << p->dw_proto_id_;
	p_stream->write_string(",quality=") << p->n_quality_;
	p_stream->write_string(",aptitude=") << p->n_aptitude_;
	p_stream->write_string(",potential=") << p->n_potential_;
	p_stream->write_string(",cur_spirit=") << p->n_spirit_;
	p_stream->write_string(",wuxing_energy=") << p->n_wuxing_energy_;
	p_stream->write_string(",talent_count=") << p->n_talent_count_;
	p_stream->write_string(",pet_state=") << p->by_pet_state_;
	p_stream->write_string(",cur_exp=") << p->n_exp_cur_;
	p_stream->write_string(",step=") << p->n_step_;
	p_stream->write_string(",grade=") << p->n_grade_;
	p_stream->write_string(",pet_lock=") << p->b_locked_;
	p_stream->write_string(",rename_count=") << p->n_rename_count_;
	p_stream->write_string(",happy_value=") << p->n_happy_value_;
	p_stream->write_string(",color=") << p->n_color;
	p_stream->write_string(",strength=") << p->n_strength;
	p_stream->write_string(",agility=") << p->n_agility;
	p_stream->write_string(",innerforce=") << p->n_innerForce;
	p_stream->write_string(",attpoint=") << p->n_att_point;
	p_stream->write_string(",time_type=") << p->dw_time_type;
	p_stream->write_string(",mode_type=") << p->dw_mode_type;
	p_stream->write_string(",xiulian_time=") << p->dw_xiulian_time;

	p_stream->where_item();
	p_stream->write_string("pet_id=") << p->dw_pet_id_;
	game_db_interface_->return_use_connect(pCon);
}

// ³èÎïsns
BOOL  db_handler::insert_pet_sns_info(const tagPetSNSInfo* p_buffer)
{
	return common_update(INVALID_VALUE, (LPVOID)p_buffer, 1, sizeof(tagPetSNSInfo),
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_pet_sns_info));
}

VOID db_handler::makesql_insert_pet_sns_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, tagPetSNSInfo);
	p_stream->clear();
	p_stream->insert_item("pet_sns");
	p_stream->write_string("pet_id=") << p->dw_pet_id;
	p_stream->write_string(",master_id=") << p->dw_master_id;
	p_stream->write_string(",friend_id=") << p->dw_friend_id;

	DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_begin_time);

	p_stream->write_string(",begintime='").write_string(sz_time_buffer_);
	p_stream->write_string("'");
}

BOOL db_handler::delete_pet_sns_info(DWORD dw_pet_id)
{
	return common_update(dw_pet_id, NULL, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_pet_sns_info));
}

VOID db_handler::makesql_delete_pet_sns_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->delete_item("pet_sns");
	p_stream->where_item();
	p_stream->write_string("pet_id=") << dw_role_id;
}

BOOL db_handler::load_pet_sns_info(OUT PVOID p_buffer, OUT INT& nNumber)
{
	return common_select(p_buffer, &nNumber, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_pet_sns_info), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_pet_sns_info));
}

VOID db_handler::makesql_load_pet_sns_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id)
{
	p_stream->select_item("pet_sns", "pet_id,master_id,friend_id,begintime");
}

VOID db_handler::procres_load_pet_sns_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	ASSERT(p_number != NULL);

	tagPetSNSInfo * p_sns_info = ( tagPetSNSInfo * ) p_buffer ;  

	*p_number = p_result->get_row_count();
	
	if ( !VALID_POINT(p_buffer))
		return;

	for(int i=0; i<*p_number; ++i)
	{
		p_sns_info[i].dw_pet_id = (*p_result)[0].get_dword();
		p_sns_info[i].dw_master_id = (*p_result)[1].get_dword();
		p_sns_info[i].dw_friend_id = (*p_result)[2].get_dword();
		tagDWORDTime time;
		DataTime2DwordTime(time, (*p_result)[3].get_string(), (*p_result)[3].get_length());
		p_sns_info[i].dw_begin_time = time;
		p_result->next_row();
	}
}
