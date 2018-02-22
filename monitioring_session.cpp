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
 *	@file		monitioring_session
 *	@author		mwh
 *	@date		2011/02/11	initial
 *	@version	0.0.1.0
 *	@brief		监控连接
*/

#include "stdafx.h"

#include "../common/ServerDefine/montior_message_server.h"

#include "monitioring_session.h"
#include "db_server_handler.h"

monitioring_session::monitioring_session()
{
	db_server_ = NULL;
	b_init_ok_ = FALSE;
	b_terminate_ = FALSE;
	net_session_ = new few_connect_client;
	
}

monitioring_session::~monitioring_session()
{
	SAFE_DELETE(net_session_);
	
}

BOOL monitioring_session::start(db_server *pGame)
{
	db_server_ = pGame;

	TCHAR tszPath[MAX_PATH];
	ZeroMemory(tszPath, sizeof(tszPath));
	if (!get_file_io_mgr()->get_ini_path(tszPath, _T("server_config/db/world"))||
		!db_server::s_p_var->load(db_server_->get_file_system(), tszPath))
	{
		ERROR_CLUE_ON(_T("file: %s line :%d  can't find server_config/db/world.ini\r\n"), _TFILE, _TLINE);
		return FALSE;
	}

	ip_			= db_server::s_p_var->get_string(_T("ip gm_server"));
	dw_port_		= db_server::s_p_var->get_dword(_T("port gm_server"));
	dw_session_id_	= db_server::s_p_var->get_dword(_T("id section"));
	dw_world_id_		= db_server::s_p_var->get_dword(_T("id world"));

	db_server::s_p_var->clear();

	if( !VALID_POINT(net_session_) )
	{
		ERROR_CLUE_ON(_T("file: %s line :%d Create monitioring_session Failed!\r\n"), _TFILE, _TLINE);
		return FALSE;
	}
	net_session_->init();

	register_command();

	if(!db_server::p_thread->create_thread(_T("connect_monitior"), 
		&monitioring_session::static_thread_connect, this))
	{
		return FALSE;
	}

	while( !db_server::p_thread->is_thread_active(_T("connect_monitior")) )
	{
		continue;
	}
	return TRUE;
}

VOID monitioring_session::destroy()
{
	Interlocked_Exchange((LONG*)&b_terminate_, TRUE);
	db_server::p_thread->waitfor_thread_destroy(_T("connect_monitior"), INFINITE);

	UNREGISTER_NET_MSG("NET_S2S_login_service",	monitioring_session, handle_server_connected);
	UNREGISTER_NET_MSG("NET_S2S_close",			monitioring_session, handle_server_connected);

	net_session_->destory();
}

VOID monitioring_session::update()
{
	update_sessions();
	send_server_info();
}

VOID monitioring_session::update_sessions()
{
	if( NULL == net_session_ )
		return;

	if( !net_session_->is_connect() && !db_server::p_thread->is_thread_active(_T("connect_monitior")) )
	{
		Interlocked_Exchange((LONG*)&b_terminate_, TRUE);
		net_session_->disconnect();

		db_server::p_thread->waitfor_thread_destroy(_T("connect_monitior"), INFINITE);

		// 重连
		Interlocked_Exchange((LONG*)&b_terminate_, FALSE);
		db_server::p_thread->create_thread(_T("connect_monitior"), &monitioring_session::static_thread_connect, this);

		while(FALSE == db_server::p_thread->is_thread_active(_T("connect_monitior")))
		{
			continue;
		}

		return;
	}

	while(net_session_->is_connect())
	{
		DWORD dw_size = 0;
		LPBYTE p_recv = net_session_->recv_msg(dw_size);
		if( !VALID_POINT(p_recv) ) break;

		serverframe::net_command_manager::get_singleton().handle_message((tag_net_message*)p_recv, dw_size, INVALID_VALUE);
		net_session_->free_recv_msg(p_recv);	
	}	
}

VOID monitioring_session::send_server_info()
{
	if( !net_session_->is_connect() || !b_init_ok_ ) return;

	if (!VALID_POINT(db_server_))
		return;

	NET_S2C_world_info send;

	if( db_server_->is_connect() )
	{
		send.e_status = ews_well;
	}
	else
	{
		send.e_status = ews_system_error;
	}
	send.n_max_online_num = INVALID_VALUE;
	send.n_online_num = INVALID_VALUE;

	net_session_->send_msg(&send, send.dw_size);
}

UINT monitioring_session::thread_connect()
{
	THROW_EXCEPTION_START;

	while( FALSE == b_terminate_ )
	{
		if( !net_session_->is_connect() )
		{
			if( !net_session_->is_trying_create_connect() )
			{
				net_session_->try_create_connect(get_tool()->unicode_to_unicode8(ip_.c_str()), dw_port_);
			}

			Sleep(100);
			continue;	
		}

		print_message(_T("connected to GMServer %s:%4d\r\n"), ip_.c_str(), dw_port_);

		NET_S2C_login_service send;
		send.dw_section_id	=	dw_session_id_;
		send.dw_world_id		=	dw_world_id_;
		send.e_type			=	est_db;
		net_session_->send_msg(&send, send.dw_size);

		break;
	}

	THROW_EXCEPTION_END;
	return 0;
}

UINT monitioring_session::static_thread_connect(LPVOID p_data)
{
	ASSERT(VALID_POINT(p_data));
	monitioring_session* p_this = (monitioring_session*)p_data;
	return p_this->thread_connect();
}


VOID monitioring_session::register_command()
{
	REGISTER_NET_MSG("NET_S2S_login_service",	monitioring_session, handle_server_connected,	_T("RT Server Certification Reply"));
	REGISTER_NET_MSG("NET_S2S_close",			monitioring_session, handle_server_connected,	_T("Close Server"));
}

DWORD monitioring_session::handle_server_connected(tag_net_message* p_msg, DWORD)
{
	Interlocked_Exchange((LPLONG)&b_init_ok_, TRUE);
	return 0;
}

DWORD monitioring_session::handle_server_disconnect(tag_net_message* p_msg, DWORD)
{
	if(VALID_POINT(db_server_))
	{
		db_server_->shutdown();
		db_server_ = NULL;
	}
	
	return 0;
}

monitioring_session g_monitioring_session;
