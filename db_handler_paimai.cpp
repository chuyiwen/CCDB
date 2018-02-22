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

#include "../common/ServerDefine/item_server_define.h"
#include "../common/ServerDefine/paimai_server_define.h"
#include "../common/ServerDefine/mail_server_define.h"

//  获取拍卖最大编号
BOOL db_handler::get_paimai_max_id(DWORD& dw_paimai_id)
{
	DWORD dwMax = 0;
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	p_stream->select_item("paimai", "max(id)");
	p_stream->where_item();
	p_stream->write_string("id>") << 0;

	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);
	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		dwMax = (DWORD)(*p_result)[0].get_long();
	}

	++dw_read_count_;

	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);

	dw_paimai_id = max(dw_paimai_id, dwMax);

	return TRUE;
}

// 更新拍卖物品
BOOL db_handler::update_paimai_item(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer)
{
	return common_update(dw_role_id, p_buffer, n_count, sizeof(s_item_update), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_item_attribute), pp_buffer);
}

// 保存拍卖信息
BOOL db_handler::insert_paimai(NET_S2DB_insert_paimai* p_paimai)
{
	return common_update(INVALID_VALUE, p_paimai, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_insert_paimai));
}

VOID db_handler::makesql_insert_paimai(odbc::sql_language_disposal* p_stream, DWORD dw_paimai_id, LPVOID p_buffer)
{
	//M_trans_pointer(p, p_buffer, NET_S2DB_insert_paimai);
	NET_S2DB_insert_paimai * p = ( NET_S2DB_insert_paimai * ) p_buffer ;  

	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->insert_item("paimai");

	p_stream->write_string("id=") << p->st_paimai.dw_paimai_id;
	p_stream->write_string(",bidup=") << p->st_paimai.dw_bidup;
	p_stream->write_string(",maxbidup=0");
	p_stream->write_string(",bidup_id=") << p->st_paimai.dw_bidup_id;
	p_stream->write_string(",chaw=") << p->st_paimai.dw_chaw;
	p_stream->write_string(",sell_id=") << p->st_paimai.dw_sell_id;
	p_stream->write_string(",timetype=") << p->st_paimai.by_time_type;
	p_stream->write_string(",show_name=") << p->st_paimai.b_show_name;
	p_stream->write_string(",auto_index=") << p->st_paimai.dw_auto_index;

	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->st_paimai.dw_beigin_time))
	{
		p_stream->write_string(",begintime='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}

	game_db_interface_->return_use_connect(p_conn);
}

// 检测是否已经拍卖
BOOL db_handler::check_is_paimai(DWORD dw_auto_paimai_id)
{
	DWORD dwMax = 0;
	odbc::sql_language_disposal* p_stream = game_db_interface_->get_io();

	p_stream->clear();

	p_stream->select_item("paimai", "auto_index");
	p_stream->where_item();
	p_stream->write_string("auto_index=") << dw_auto_paimai_id;

	odbc::execute_result* p_result = game_db_interface_->sql_query(p_stream);

	NET_DB2S_check_is_paimai send;
	send.dw_auto_paimai_id = dw_auto_paimai_id;

	if(VALID_POINT(p_result) && p_result->get_row_count() > 0)
	{
		send.b_have = TRUE;
	}
	else
	{
		send.b_have = FALSE;
	}

	server_->send_msg(GAMESERVERSESSIONID, &send, send.dw_size);

	++dw_read_count_;

	game_db_interface_->return_io(p_stream);
	game_db_interface_->free_result_query(p_result);

	return TRUE;
}

// 更新拍卖数据
BOOL db_handler::update_auto_paimai(NET_S2DB_update_auto_paimai* p_auto_paimai)
{
	return common_update(INVALID_VALUE, p_auto_paimai, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_auto_paimai));
}

VOID db_handler::makesql_update_auto_paimai(odbc::sql_language_disposal* p_stream, DWORD dw_paimai_id, LPVOID p_buffer)
{
	NET_S2DB_update_auto_paimai* p = (NET_S2DB_update_auto_paimai*)p_buffer;

	p_stream->clear();

	p_stream->update_item("auto_paimai");
	p_stream->write_string("inventory=") << p->st_auto_paimai.dwInventory;
	p_stream->write_string(",is_sell=") << p->st_auto_paimai.bSell;
}

// 读取所有自动拍卖信息
BOOL db_handler::load_all_auto_paimai(LPVOID p_buffer)
{
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE,
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_auto_paimai),
		MakeDelegateOfDBHandler(&db_handler::procres_load_all_auto_paimai));
}

VOID db_handler::makesql_load_all_auto_paimai(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->clear();
	p_stream->select_item("auto_paimai",
		"id,"			"item_id,"		"bidup,"		"chaw,"		"sell_id,"
		"time_type,"	"inventory,"	"complement,"	"is_sell,"	"sell_num,"
		"is_bind,"		"online");
}

VOID db_handler::procres_load_all_auto_paimai(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	INT	n_total_count = p_result->get_row_count();

	INT	n_max_count = n_total_count;
	INT	n_cur_count = 0;

	while(n_max_count > 0)
	{
		if(n_max_count > MAX_LOAD_PAI_NUM)
		{
			n_cur_count = MAX_LOAD_PAI_NUM;
		}
		else
		{
			n_cur_count = n_max_count;
		}
		memset(p_buffer, 0, sizeof(char)*MAX_LOAD_PAI_BUFFER);
		NET_DB2S_load_auto_paimai* p_send = (NET_DB2S_load_auto_paimai*)p_buffer;
		M_msg_init(p_send, NET_DB2S_load_auto_paimai);

		M_trans_pointer(p, p_send->st_auto_paimai, tagAutoPaimai);

		for(INT i = 0; i < n_cur_count; i++)
		{
			p->dwID = (*p_result)[0].get_dword();
			p->dwItemID = (*p_result)[1].get_dword();
			p->dwBidup = (*p_result)[2].get_dword();
			p->dwChaw = (*p_result)[3].get_dword();
			p->dwSellRoleID = (*p_result)[4].get_dword();
			p->byTimeType = (*p_result)[5].get_byte();
			p->dwInventory = (*p_result)[6].get_dword();
			p->dwComplement = (*p_result)[7].get_dword();
			p->bSell = (*p_result)[8].get_int();
			p->dwSellNum = (*p_result)[9].get_dword();
			p->bBind = (*p_result)[10].get_byte();
			p->bOnline = (*p_result)[11].get_byte();

			p_result->next_row();
			++p;
		}
		p_send->n_num = n_cur_count;
		p_send->dw_size = sizeof(NET_DB2S_load_auto_paimai) + (n_cur_count-1)*sizeof(tagAutoPaimai);

		server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

		n_max_count -= n_cur_count;
	}
}

// 读取所有拍卖信息
BOOL db_handler::load_all_paimai(LPVOID p_buffer)
{
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE,
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_paimai),
		MakeDelegateOfDBHandler(&db_handler::procres_laod_all_paimai));
}

VOID db_handler::makesql_load_all_paimai(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id)
{
	p_stream->select_item("paimai", 
		"id,"		"bidup,"		"bidup_id,"		"maxbidup,"
		"chaw,"		"sell_id,"		"begintime,"	"timetype,"
		"show_name,"	"auto_index");
}

VOID db_handler::procres_laod_all_paimai(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	INT	n_total_count = p_result->get_row_count();

	INT	n_max_count = n_total_count;
	INT	n_cur_count = 0;

	while(n_max_count > 0)
	{
		if(n_max_count > MAX_LOAD_PAI_NUM)
		{
			n_cur_count = MAX_LOAD_PAI_NUM;
		}
		else
		{
			n_cur_count = n_max_count;
		}
		memset(p_buffer, 0, sizeof(char)*MAX_LOAD_PAI_BUFFER);
		NET_DB2S_load_all_paimai* p_send = (NET_DB2S_load_all_paimai*)p_buffer;
		M_msg_init(p_send, NET_DB2S_load_all_paimai);

		M_trans_pointer(p, p_send->st_paimai, tag_paimai);

		for(INT i = 0; i < n_cur_count; i++)
		{
			p->dw_paimai_id		= (*p_result)[0].get_dword();
			p->dw_bidup			= (*p_result)[1].get_dword();
			p->dw_bidup_id		= (*p_result)[2].get_dword();
			p->dw_chaw			= (*p_result)[4].get_dword();
			p->dw_sell_id		= (*p_result)[5].get_dword();
			DataTime2DwordTime(p->dw_beigin_time, (*p_result)[6].get_string(), (*p_result)[6].get_length());
			p->by_time_type		= (*p_result)[7].get_byte();
			p->b_show_name		= (BOOL)(*p_result)[8].get_byte();
			p->dw_auto_index	= (*p_result)[9].get_dword();

			p_result->next_row();
			++p;
		}
		p_send->n_num = n_cur_count;
		p_send->dw_size = sizeof(NET_DB2S_load_all_paimai) + (n_cur_count-1)*sizeof(tag_paimai);

		server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

		n_max_count -= n_cur_count;
	}

}

// 读取拍卖物品
BOOL  db_handler::load_paimai_item(LPVOID p_buffer)
{
	return common_select(p_buffer, NULL, INVALID_VALUE, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_paimai_item),
		MakeDelegateOfDBHandler(&db_handler::procres_load_all_paimai_item));
}

VOID db_handler::makesql_load_paimai_item(odbc::sql_language_disposal* p_stream, DWORD dw_paimai_id, DWORD dw_role_id)
{
	p_stream->clear();
	p_stream->write_string("select ");
	init_item_load_field(p_stream);
	p_stream->write_string(" from item left join equip using(serial)");
	p_stream->where_item();
	/*p_stream->write_string("owner_id=") << dw_paimai_id;*/
	p_stream->write_string("container_type_id=") << EICT_PaiMai;
}

VOID db_handler::procres_load_all_paimai_item(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result)
{
	int dw_total_count = p_result->get_row_count();

	int n_max_count = dw_total_count;
	int n_cur_count = 0;

	while(n_max_count > 0)
	{
		if(n_max_count > MAX_LOAD_PAI_NUM)
		{
			n_cur_count = MAX_LOAD_PAI_NUM;
		}
		else
		{
			n_cur_count = n_max_count;
		}
		memset(p_buffer, 0, sizeof(char)*MAX_LOAD_PAI_BUFFER);
		NET_DB2S_load_paimai_item* p_send = (NET_DB2S_load_paimai_item*)p_buffer;
		M_msg_init(p_send, NET_DB2S_load_paimai_item);

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
			//p_equip->equipSpec.nPotValUsed 		= (*p_result)[14].get_int();
			//p_equip->equipSpec.byMinUseLevel 	= (*p_result)[15].get_byte();
			//p_equip->equipSpec.byMaxUseLevel 	= (*p_result)[16].get_byte();
			//p_equip->equipSpec.n16MinDmg 		= (*p_result)[18].get_short();
			//p_equip->equipSpec.n16MaxDmg 		= (*p_result)[19].get_short();
			//p_equip->equipSpec.n16Armor 		= (*p_result)[20].get_short();
			//p_equip->equipSpec.nPotVal 			= (*p_result)[21].get_int();
			//p_equip->equipSpec.byPosyTimes 		= (*p_result)[23].get_byte();
			//p_equip->equipSpec.byEngraveTimes 	= (*p_result)[25].get_byte();
			p_equip->equipSpec.byHoleNum 		= (*p_result)[27].get_byte();
			//p_equip->equipSpec.byTriggerCount 	= (*p_result)[29].get_byte();
			//p_equip->equipSpec.byLongfuLevel	= (*p_result)[31].get_byte();
			//p_equip->equipSpec.bCanCut 			= (*p_result)[33].get_bool();
			p_equip->equipSpec.byLuck		= (*p_result)[38].get_int();

			// 时装附加属性
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
			//p_equip->equipSpec.nCurLevelExp				= (*p_result)[54].get_int();
			//p_equip->equipSpec.nLevel					= (*p_result)[55].get_int();
			//p_equip->equipSpec.byTalentPoint				= (*p_result)[59].get_int();
			//p_equip->equipSpec.byMaxTalentPoint			= (*p_result)[60].get_int();
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
		p_send->dw_size = sizeof(NET_DB2S_load_paimai_item) - sizeof(BYTE) + (DWORD)((BYTE*)p_temp - (BYTE*)p_send->by_data_);

		server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

		n_max_count -= n_cur_count;
	}
}

// 删除拍卖
BOOL db_handler::delete_paimai(NET_S2DB_delete_paimai* p_recv)
{
	return common_update(p_recv->dw_paimai_id, NULL, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_delete_paimai));
}

VOID db_handler::makesql_delete_paimai(odbc::sql_language_disposal* p_stream, DWORD dw_paimai_id, LPVOID p_buffer)
{
	p_stream->delete_item("paimai");
	p_stream->where_item();
	p_stream->write_string("id=") << dw_paimai_id;
}

// 更新拍卖
BOOL db_handler::update_paimai(NET_S2DB_update_paimai* p_recv)
{
	return common_update(INVALID_VALUE, &p_recv->st_paimai, 1, 0,
		MakeDelegateOfDBHandler(&db_handler::makesql_update_paimai));
}

VOID db_handler::makesql_update_paimai(odbc::sql_language_disposal* p_stream, DWORD dw_paimai_id, LPVOID p_buffer)
{
	M_trans_pointer(p, p_buffer, tag_paimai);

	p_stream->update_item("paimai");
	p_stream->write_string("bidup=") << p->dw_bidup;
	p_stream->write_string(",maxbidup=0");
	p_stream->write_string(",bidup_id=") << p->dw_bidup_id;
	p_stream->write_string(",chaw=") << p->dw_chaw;
	p_stream->write_string(",sell_id=") << p->dw_sell_id;
	p_stream->write_string(",timetype=") << p->by_time_type;

	if(DwordTime2DataTime(sz_time_buffer_, sizeof(sz_time_buffer_), p->dw_beigin_time))
	{
		p_stream->write_string(",begintime='").write_string(sz_time_buffer_);
		p_stream->write_string("'");
	}
	p_stream->where_item();
	p_stream->write_string("id=") << p->dw_paimai_id;
}