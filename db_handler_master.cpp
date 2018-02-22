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
 *	@file		db_handler_master
 *	@author		mwh
 *	@date		2011/02/12	initial
 *	@version	0.0.1.0
 *	@brief		师徒
*/


#include "StdAfx.h"
#include "db_server_handler.h"
#include "../common/ServerDefine/master_apprentice_server_define.h"
#include "db_handler.h"

// 获取师徒数据
BOOL db_handler::load_all_master_prentices( OUT LPVOID& pBuffer, OUT int& n32Size )
{
	return common_select( pBuffer,&n32Size, 0, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_master_prentices), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_all_master_prentices ));
}

VOID db_handler::makesql_load_all_master_prentices(odbc::sql_language_disposal* p_stream, DWORD dwAccoun, DWORD dwRole )
{
	p_stream->clear( );
	p_stream->write_string("select ");
	init_load_master_prentices_fields(p_stream);
	p_stream->write_string(" from master_prentice");
}
VOID db_handler::init_load_master_prentices_fields(odbc::sql_language_disposal* p_stream )
{
	p_stream->write_string("master," "graduates," "master_moral," "show_in_panel," "prentices" );
}
VOID db_handler::procres_load_all_master_prentices(OUT LPVOID &p_buffer, OUT int *pSize, odbc::execute_result *p_result)
{
	NET_DB2S_load_all_master_prentice * p_send = ( NET_DB2S_load_all_master_prentice *) p_buffer ;   
	
	INT number  = p_result->get_row_count( );
	const INT nInitialSize = sizeof(NET_DB2S_load_all_master_prentice) - sizeof(s_master_prentice_load);

	p_send->u32_number = 0;
	p_send->dw_size = nInitialSize;

	s_master_prentice_load* p = p_send->master_prentice;
	for(UINT32 u32 = 0; u32 < number; ++u32)
	{
		if(p_send->u32_number >= 20480)
		{
			p_send->dw_size += (DWORD)( (char*)p - (char*)p_send->master_prentice );
			server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size );

			p = p_send->master_prentice;	
			p_send->u32_number = 0;
			p_send->dw_size = nInitialSize;
		}

		p_send->u32_number += 1;
		p->dw_master = (*p_result)[0].get_dword( );
		p->dw_graduates = (UINT32)(*p_result)[1].get_dword( );
		p->dw_master_moral = (UINT32)(*p_result)[2].get_dword( );
		p->b_show_in_panel = (*p_result)[3].get_byte( );
		p->by_number =(BYTE)((*p_result)[4].get_length( ) / sizeof(DWORD) );
		(*p_result)[4].get_blob(p->dw_prentices,(*p_result)[4].get_length( ));
		p = (s_master_prentice_load*)( (char*)p + MasterPrenticeLoadRealSize(p->by_number) );
		p_result->next_row( );
	}

	if(p_send->u32_number)
	{
		p_send->dw_size += (DWORD)( (char*)p - (char*)p_send->master_prentice );
		server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size );
	}
/*
	p_send->u32_number = p_result->get_row_count( );
	s_master_prentice_load* p = p_send->master_prentice;
	for( UINT32 u32 = 0; u32 < p_send->u32_number; ++u32)
	{
		p->dw_master = (*p_result)[0].get_dword( );
		p->dw_graduates = (UINT32)(*p_result)[1].get_dword( );
		p->dw_master_moral = (UINT32)(*p_result)[2].get_dword( );
		p->b_show_in_panel = (*p_result)[3].get_byte( );
		p->by_number =(BYTE)((*p_result)[4].get_length( ) / sizeof(DWORD) );
		(*p_result)[4].get_blob(p->dw_prentices,(*p_result)[4].get_length( ));
		p = (s_master_prentice_load*)( (char*)p + MasterPrenticeLoadRealSize(p->by_number) );
		p_result->next_row( );
	}
	p_send->dw_size += (DWORD)( (char*)p - (char*)p_send->master_prentice );
*/
}
// 保存师徒数据
BOOL db_handler::save_master_prentices(LPVOID p_buffer, DWORD dw_role_id, UINT32 u32Number)
{
	return common_update( dw_role_id, p_buffer, u32Number, sizeof(s_master_prentice_save), 
		MakeDelegateOfDBHandler(&db_handler::makesql_save_master_prentices));
}
VOID db_handler::makesql_save_master_prentices(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	s_master_prentice_save* p = (s_master_prentice_save*)p_buffer;

	odbc::tag_mysql_connect* p_conn = game_db_interface_->get_idlesse_connect();

	p_stream->clear( );
	p_stream->update_item("master_prentice");
	p_stream->write_string("graduates=") << p->dw_graduates;
	p_stream->write_string(",master_moral=") << p->dw_master_moral;
	p_stream->write_string(",show_in_panel=") << p->b_show_in_panel;
	p_stream->write_string(",prentices='").write_blob(p->dw_prentices, sizeof(DWORD)*p->by_number, p_conn);
	p_stream->write_string("'");
	p_stream->where_item();
	p_stream->write_string(" master=") << p->dw_master;
	
	game_db_interface_->return_use_connect( p_conn );
}
// 将角色加入师徒榜
BOOL db_handler::add_new_master_prentices( DWORD dw_role_id, LPVOID p_buffer /* = NULL */ )
{
	return common_update( dw_role_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_add_new_master_prentices));
}
VOID db_handler::makesql_add_new_master_prentices(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->clear( );
	p_stream->insert_item("master_prentice");
	p_stream->write_string("master=") << dw_role_id;
	p_stream->write_string(",graduates = 0 ");
	p_stream->write_string(",master_moral = 0");
	p_stream->write_string(",show_in_panel = 0 ");
}
// 更新师傅ID和禁止操作时间
BOOL db_handler::update_master_id_and_forbid_time( DWORD dw_role_id, LPVOID p_buffer )
{
	return common_update( dw_role_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_update_master_id_and_forbid_time));
}

VOID db_handler::makesql_update_master_id_and_forbid_time(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{

	NET_DB2C_update_master_id_and_forbid_time * p = ( NET_DB2C_update_master_id_and_forbid_time * ) p_buffer ; 

	p_stream->clear( );
	p_stream->update_item("role_data");
	p_stream->write_string(" master_id=") << p->dw_master;
	p_stream->write_string(",masterprentice_forbid_time=") << p->dw_forbid_time;
	p_stream->where_item();
	p_stream->write_string("role_id=") << p->dw_role_id;
}
// 师傅删除角色
BOOL db_handler::master_delete_role( DWORD dw_role_id, LPVOID p_buffer /* = NULL */ )
{
	return common_update( dw_role_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_master_delete_role));
}
VOID db_handler::makesql_master_delete_role(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->clear( );
	p_stream->delete_item("master_prentice");
	p_stream->where_item();
	p_stream->write_string("master=") << dw_role_id;
}
// 加载拜师榜
BOOL db_handler::load_all_master_recruit(LPVOID pBuffer, OUT INT& n32Size)
{
	return  common_select( pBuffer,&n32Size, 0, INVALID_VALUE, 
		MakeDelegateOfDBHandler(&db_handler::makesql_load_all_master_recruit), 
		MakeDelegateOfDBHandler(&db_handler::procres_load_all_master_recruit ));
}
VOID db_handler::makesql_load_all_master_recruit(odbc::sql_language_disposal* p_stream, DWORD dwAccoun, DWORD dwRole )
{
	p_stream->clear();
	p_stream->write_string("select role_id from master_recruit");
}
VOID db_handler::procres_load_all_master_recruit(OUT LPVOID &p_buffer, OUT int *pSize, odbc::execute_result *p_result)
{
	NET_DB2S_load_all_master_recruit* p_send = (NET_DB2S_load_all_master_recruit*)p_buffer;

	INT number = p_result->get_row_count();
	const INT nInitialSize = sizeof(NET_DB2S_load_all_master_recruit) - sizeof(DWORD);

	p_send->u32_number = 0;
	p_send->dw_size = nInitialSize;
	DWORD* p = p_send->dwRoleID;
	for(UINT n = 0; n < number; ++n)
	{
		if(p_send->u32_number >= 10000)
		{
			p_send->dw_size += p_send->u32_number * sizeof(DWORD);
			server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);

			p = p_send->dwRoleID;
			p_send->u32_number = 0;
			p_send->dw_size = nInitialSize;
		}

		*p = (*p_result)[0].get_dword( ); ++p;
		p_send->u32_number += 1;
		p_result->next_row( );
	}

	if(p_send->u32_number)
	{
		p_send->dw_size += p_send->u32_number * sizeof(DWORD);
		server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);
	}

	/*
	p_send->u32_number = p_result->get_row_count();

	DWORD* p = p_send->dwRoleID;
	for(UINT n = 0; n < p_send->u32_number; ++n, ++p)
	{
		*p = (*p_result)[0].get_dword( );
		p_result->next_row( );
	}
	p_send->dw_size += (DWORD)((char*)p - (char*)p_send->dwRoleID);
	*/
}
// 加入拜师榜
BOOL db_handler::join_master_recruit(DWORD dw_role_id, LPVOID p_buffer /* = NULL */)
{
	return common_update( dw_role_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_join_master_recruit));
}
VOID db_handler::makesql_join_master_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->clear();
	p_stream->insert_item("master_recruit");
	p_stream->write_string("role_id=") << dw_role_id;
}
// 退出拜师榜
BOOL db_handler::leave_master_recruit(DWORD dw_role_id, LPVOID p_buffer /* = NULL */)
{
	return common_update( dw_role_id, p_buffer, 1, 0, 
		MakeDelegateOfDBHandler(&db_handler::makesql_leave_master_recruit));
}
VOID db_handler::makesql_leave_master_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer)
{
	p_stream->clear();
	p_stream->delete_item("master_recruit");
	p_stream->where_item();
	p_stream->write_string("role_id=") << dw_role_id;
}