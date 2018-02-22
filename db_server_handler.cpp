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
 *	@file		db_server_handler
 *	@author		mwh
 *	@date		2010/10/11	initial
 *	@version	0.0.1.0
 *	@brief		数据库服务器类
*/

#include "StdAfx.h"
#include "../common/ServerDefine/base_server_define.h"
#include "../common/ServerDefine/role_data_server_define.h"
#include "../common/ServerDefine/mall_server_define.h"
#include "../common/ServerDefine/leftmsg_server_define.h"
#include "../../common/WorldDefine/svn_revision.h"
#include "db_handler.h"
#include "db_server_handler.h"
#include "monitioring_session.h"

file_container* db_server::s_p_var = NULL;
thread_manager*		db_server::p_thread = NULL;
 
db_server::db_server()
{
	p_server_				= new few_connect_server;
	db_handler_				= new db_handler;
	p_vfs					= new file_system;
	db_server::p_thread		= new thread_manager;
	b_terminate_			= FALSE;
	b_server_disconnect_	= FALSE;
	b_has_server_connect_	= FALSE;
	st_log_time_			= 0;
	sz_log_file_postfix_[0]	= '\0';

	ZeroMemory(sz_game_server_name_, sizeof(sz_game_server_name_));

	// 角色基本信息(循环使用)
	simple_role_ = new NET_DB2S_load_simrole;	

	// 创角信息(循环使用)
	create_role_ = new NET_DB2S_create_role;	

	// 百宝信息(循环使用)
	load_baibao_ = new NET_DB2S_load_baibao;

	// 百宝日志(循环使用)<多线程勿用>
	load_baibao_log_ = (NET_DB2S_load_baibao_log*)buffer_;

	// 角色完整信息(循环使用)<多线程勿用>
	full_role_ = (NET_DB2S_load_role *)buffer_;

	db_server::s_p_var = new file_container;
}
db_server::~db_server()
{
	// 跳出循环,让所有线程终止
	Interlocked_Exchange((long*)(&b_terminate_), TRUE);

	g_monitioring_session.destroy();
	db_server::p_thread->waitfor_all_thread_destroy();

	unregister_command();

	SAFE_DELETE(create_role_);
	SAFE_DELETE(simple_role_);
	SAFE_DELETE(load_baibao_);
	SAFE_DELETE(db_handler_);
	SAFE_DELETE(p_server_);

	SAFE_DELETE(s_p_var);

	SAFE_DELETE(p_vfs);

	SAFE_DELETE(db_server::p_thread);

	get_window()->destroy();
}
// 启动
BOOL db_server::start(HINSTANCE hInst)
{
	///* 定时关服务器
	tagDWORDTime dwShutDownTime;
	ZeroMemory(&dwShutDownTime, sizeof(dwShutDownTime));
	dwShutDownTime.year = SHUT_DOWN_YEAY;
	dwShutDownTime.month = SHUT_DOWN_MONTH;

	DWORD dwCurTime = GetCurrentDWORDTime();

	if (dwCurTime > dwShutDownTime)
		return false;
	//*/

	// 初始配置
	init_config();

	if(!create_window(hInst)) return FALSE;
	
	// 注册网络消息处理
	register_command();

	// 设置登录&&退出回调
	p_server_->init(fastdelegate::MakeDelegate(this, &db_server::callback_on_login), 
		fastdelegate::MakeDelegate(this, &db_server::callback_on_logout), n_port_);

	// 启动监控层
	g_monitioring_session.start(this);

	// 创建收消息线程
	if(!db_server::p_thread->create_thread(_T("rec_msg"), &db_server::static_recive_thread, this) ) return FALSE;

	// 创建处理消息线程
	if(!db_server::p_thread->create_thread(_T("proc_msg"), &db_server::static_process_thread, this) )return FALSE;

	// 创建广播消息线程
	if(!db_server::p_thread->create_thread(_T("broadcast"), &db_server::static_heartbeat_thead, this)) return FALSE;

	return TRUE;
}
// 初始配置
BOOL db_server::init_config()
{
	TCHAR sz_path[MAX_PATH];
	ZeroMemory(sz_path, sizeof(sz_path));
	if (!get_file_io_mgr()->get_ini_path(sz_path, _T("server_config/db/world")))
	{
		return FALSE;
	}

	db_server::s_p_var->load(get_file_system(), sz_path);

	n_port_				= db_server::s_p_var->get_dword(_T("db_server port"));
	dw_db_gold_code_	= db_server::s_p_var->get_dword(_T("db_server golden_code"));
	dw_game_gold_code_	= db_server::s_p_var->get_dword(_T("game_server golden_code"));
	
	db_server::s_p_var->clear();

	return TRUE;
}
// 心跳线程
UINT db_server::heartbeat_thead()
{	
	while(true)
	{
		if(b_terminate_)return 0;

		get_status( ).dw_heartbeat_++;

		g_monitioring_session.update();

		Sleep(1000);
	}

	return 0;
}

UINT db_server::static_heartbeat_thead(LPVOID p_data)
{
	ASSERT(VALID_POINT(p_data));
	db_server* p_this = (db_server*)p_data;
	return p_this->heartbeat_thead();
}

// 收包线程
UINT db_server::recive_thread()
{
	THROW_EXCEPTION_START;
	
	LPBYTE	p_recv		= NULL;
	DWORD	dw_msg_size	= INVALID_VALUE;
	int		n_unrecv	= INVALID_VALUE;
	
	while(true)
	{	
		Sleep(SLEEPTIME);
		
		// 需要结束线程
		if(b_terminate_) break;

		// 没有连接,线程无限休眠
		if(!b_has_server_connect_) continue;

		// 检查是否需要创建日志表
		if(GetCurrentDWORDTime().day != st_log_time_.day)
		{
			if(message_queue_.msg_count() == 0)
				create_current_log_table();
		}
		
		// 接收网络消息包，放入消息队列，等待处理
		while(true)
		{
			p_recv = p_server_->recv_msg(GAMESERVERSESSIONID, dw_msg_size, n_unrecv);
			get_status( ).dw_wait_recv_msg_count_ = n_unrecv;
			
			if(!VALID_POINT(p_recv)) break;

			if(dw_msg_size != ((tag_net_message*)p_recv)->dw_size)
			{
				ASSERT(0);
				print_message(_T("Invalid net command size[<cmd>%u <size>%d]\r\n"), 
					((tag_net_message*)p_recv)->dw_message_id, ((tag_net_message*)p_recv)->dw_size);
				continue;
			}

			// 会激活消息处理线程
			message_queue_.add_msg(p_recv);

			// 包总和
			++get_status( ).dw_recv_msg_count_;
		}
	}	

	THROW_EXCEPTION_END;
	_endthreadex(0);
	return 0;
}

UINT db_server::static_recive_thread(LPVOID p_data)
{
	ASSERT(VALID_POINT(p_data));
	db_server* p_this = (db_server*)p_data;
	return p_this->recive_thread();
}

// 逻辑处理线程
UINT db_server::process_thread()
{
	THROW_EXCEPTION_START;

	LPBYTE	p_msg		= NULL;
	while(true)
	{
		// 需要结束线程
		if(b_terminate_) break;
		
		::WaitForSingleObject(message_queue_.get_event(), SLEEPTIME);

		while(true)
		{
			p_msg = message_queue_.get_msg();
			if(!VALID_POINT(p_msg)) break;

			//处理消息包
			serverframe::net_command_manager::get_singleton().handle_message((tag_net_message*)p_msg, ((tag_net_message*)p_msg)->dw_size, INVALID_VALUE);

			p_server_->free_recv_msg(GAMESERVERSESSIONID, p_msg);

			++get_status( ).dw_process_msg_count_;
		}

		// 服务器断开连接
		if( b_server_disconnect_ )
		{
			b_server_disconnect_ = FALSE;
			
			terminate_world();
			
			// 重置连接状态,保证其他服务器能够连接
			Interlocked_Exchange((LONG *)&b_has_server_connect_, FALSE);
		}
	}

	THROW_EXCEPTION_END;
	_endthreadex(0);
	return 0;
}

UINT db_server::static_process_thread(LPVOID p_data)
{
	ASSERT(VALID_POINT(p_data));
	db_server* p_this = (db_server*)p_data;
	return p_this->process_thread();
}

// 登陆回调
DWORD db_server::callback_on_login(LPBYTE pByte, DWORD dw_size)
{
	__auto_lock__(lock_);

	// 已有游戏服务器连接或连接验证错误
	if(b_has_server_connect_ != FALSE
		|| *(DWORD*)pByte != get_tool()->crc32("NET_retification"))
	{
		print_message(_T("A new game world apply for connecting，or connection message is wrong!\r\n"));
		return INVALID_VALUE;
	}

	M_trans_pointer(pNc, pByte, NET_retification);
	if(pNc->dw_golden_code_ != dw_game_gold_code_)
	{
		print_message(_T("error connection, golden code is wrong!"));
		return INVALID_VALUE;
	}
	
	tstring strWindowName = pNc->sz_world_name_;
	strWindowName = _T("DBServer") + strWindowName;
	::SetWindowText(get_window()->getHWND(), strWindowName.c_str());

	if(init_world(pNc->sz_world_name_) != E_Success)
	{
		return INVALID_VALUE;
	}

	// 确保DBServer与gameserver是一对一连接
	Interlocked_Exchange((LONG *)&b_has_server_connect_, TRUE);
	b_server_disconnect_	= FALSE;

	print_message(_T("GameServer of %s LoginServer!\n"), pNc->sz_world_name_);
	
	return GAMESERVERSESSIONID; 
}
// 下线回调
DWORD db_server::callback_on_logout(DWORD dw_reserve)
{
	__auto_lock__(lock_);

	int		n_unrecv	= 0;
	DWORD dw_msg_size	= 0;
	while(true)
	{
		LPBYTE p_recv = p_server_->recv_msg(GAMESERVERSESSIONID, dw_msg_size, n_unrecv);
		
		get_status( ).dw_wait_recv_msg_count_ = n_unrecv;
	
		if(!VALID_POINT(p_recv))break;

		if(dw_msg_size != ((tag_net_message*)p_recv)->dw_size)
		{
			ASSERT(0);
			print_message(_T("Invalid net command size[<cmd>%u <size>%d]\r\n"), 
				((tag_net_message*)p_recv)->dw_message_id, ((tag_net_message*)p_recv)->dw_size);
			continue;
		}

		message_queue_.add_msg(p_recv);

		// 包总和
		++get_status( ).dw_recv_msg_count_;
	}

	// 设置游戏服务器断线
	b_server_disconnect_	= TRUE;

	print_message(_T("GameServer of %s logout!\n"), sz_game_server_name_);

	return 0;
}
// 初始游戏世界
DWORD db_server::init_world(LPCTSTR sz_game_server)
{
	TCHAR sz_init_file[MAX_PATH];

	// 获取配置文件名称
	_tcscpy(sz_init_file, _T("server_config/db/"));
	_tcscat(sz_init_file, sz_game_server);

	TCHAR sz_path[MAX_PATH];
	ZeroMemory(sz_path, sizeof(sz_path));
	if (!get_file_io_mgr()->get_ini_path(sz_path, sz_init_file)) return FALSE;

	// 初始化数据库
	if(!db_handler_->start(this, sz_path, p_server_))
	{
		print_message(_T("Database for %s connected failed!\n"), sz_game_server_name_);
		return INVALID_VALUE;
	}

	// 创建日志表
	create_current_log_table();

	// 记录游戏世界名称和运行游戏世界的机器名
	_tcsncpy(sz_game_server_name_, sz_game_server, cal_tchar_array_num(sz_game_server_name_) - 1);

	// 初始化统计数据
	get_status( ).init();

	print_message(_T("Database for %s connected!\n"), sz_game_server_name_);

	return E_Success;
}
// 关闭游戏世界
VOID db_server::terminate_world()
{
	while(true)
	{
		LPBYTE p_msg = message_queue_.get_msg();
		if(!VALID_POINT(p_msg)) break;

		// 处理消息
		serverframe::net_command_manager::get_singleton().handle_message((tag_net_message*)p_msg, ((tag_net_message*)p_msg)->dw_size, INVALID_VALUE);

		++get_status( ).dw_process_msg_count_;

		p_server_->free_recv_msg(GAMESERVERSESSIONID, p_msg);
	}

	// 删除数据库处理
	db_handler_->destroy();
	st_log_time_ = 0;
	sz_log_file_postfix_[0]	= '\0';

	ZeroMemory(sz_game_server_name_, sizeof(sz_game_server_name_));
}
// 创建窗口
BOOL db_server::create_window(HINSTANCE hInst)
{
	get_window()->init(800,600,TRUE);
	get_window()->create_window(_T("DBServer"), hInst);

	return TRUE;
}
// 创建窗口
VOID db_server::refresh_window()
{
	const db_server::status_record *p_status = get_status_ptr();

	// 系统参数
	get_window()->watch_info(_T("CPU内核:"), p_status->n_cpu_number_);
	get_window()->watch_info(_T("内存总量(M):"), p_status->dw_total_phys_/1024/1024);
	get_window()->watch_info(_T("可用内存(M):"), p_status->dw_avail_phys_/1024/1024);
	get_window()->watch_info(_T("非页面缓冲池使用:"), p_status->n_quota_non_paged_pool_usage_);

	// 版本信息
	get_window()->watch_info(_T("游戏数据库版本:"), get_status( ).dw_version_game_db_);
	

	// 心跳次数
	get_window()->watch_info(_T("是否有连接:"), b_has_server_connect_);
	get_window()->watch_info(_T("服务器心跳:"), p_status->dw_heartbeat_);

	// 消息处理
	get_window()->watch_info(_T("包总和:"), p_status->dw_recv_msg_count_);
	get_window()->watch_info(_T("处理包总和:"), p_status->dw_process_msg_count_);
	get_window()->watch_info(_T("发送包总和:"), p_status->dw_send_msg_count_);
	get_window()->watch_info(_T("接收包总大小:"), p_status->dw_recv_msg_size_);
	get_window()->watch_info(_T("发送包总大小:"), p_status->dw_send_msg_size_);

	get_window()->watch_info(_T("读数据库次数:"), p_status->dw_read_count_);
	get_window()->watch_info(_T("写数据库次数:"), p_status->dw_write_count_);

	get_window()->watch_info(_T("频率(ms):"), p_status->dw_interval_);
	get_window()->watch_info(_T("一个tick内收包数:"), p_status->dw_tick_recv_msg_count_);
	get_window()->watch_info(_T("一个tick内发包数:"), p_status->dw_tick_send_msg_count_);
	get_window()->watch_info(_T("一个tick内收包大小:"), p_status->dw_tick_recv_msg_size_);
	get_window()->watch_info(_T("一个tick内发包大小:"), p_status->dw_tick_send_msg_size_);

	get_window()->watch_info(_T("一个tick内处理包数:"), p_status->dw_tick_proc_msg_count_);
	get_window()->watch_info(_T("一个tick内读数据库次数:"), p_status->dw_tick_read_db_count_);
	get_window()->watch_info(_T("一个tick内写数据库次数:"), p_status->dw_tick_write_db_count_);

	get_window()->watch_info(_T("等待发送的包数:"), p_status->dw_wait_send_msg_count_);
	get_window()->watch_info(_T("等待处理的包数: "), p_status->dw_wait_recv_msg_count_);
}
// 创建窗口
const db_server::status_record* db_server::get_status_ptr( )
{
	// 系统相关
	MEMORYSTATUS mem_status;
	mem_status.dwLength = sizeof(mem_status);
	GlobalMemoryStatus(&mem_status);
	get_status( ).dw_total_phys_ = (DWORD)mem_status.dwTotalPhys;
	get_status( ).dw_avail_phys_ = (DWORD)mem_status.dwAvailPhys;

	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	get_status( ).n_quota_non_paged_pool_usage_ = (int)pmc.QuotaNonPagedPoolUsage;

	// 系统时间
	DWORD dw_tick = GetTickCount();
	get_status( ).dw_interval_ = dw_tick - get_status( ).dw_last_tick_;
	get_status( ).dw_last_tick_ = dw_tick;

	// 游戏服务器相关
	get_status( ).dw_read_count_		= db_handler_->get_total_read_count();
	get_status( ).dw_write_count_		= db_handler_->get_total_write_count();

	get_status( ).dw_send_msg_count_	= p_server_->get_send_msg_total_num();
	get_status( ).dw_recv_msg_size_	= p_server_->get_recv_msg_total_bytes();
	get_status( ).dw_send_msg_size_	= p_server_->get_send_msg_total_bytes();

	get_status( ).dw_tick_recv_msg_count_	= get_status( ).dw_recv_msg_count_ - get_status( ).dw_last_recv_msg_count_;
	get_status( ).dw_tick_proc_msg_count_	= get_status( ).dw_process_msg_count_ - get_status( ).dw_last_proc_msg_count_;
	get_status( ).dw_tick_send_msg_count_	= get_status( ).dw_send_msg_count_ - get_status( ).dw_last_send_msg_count_;

	get_status( ).dw_tick_recv_msg_size_	= get_status( ).dw_recv_msg_size_ - get_status( ).dw_last_recv_msg_size_;
	get_status( ).dw_tick_send_msg_size_	= get_status( ).dw_send_msg_size_ - get_status( ).dw_last_send_msg_size_;

	get_status( ).dw_tick_read_db_count_	= get_status( ).dw_read_count_ - get_status( ).dw_last_read_count_;
	get_status( ).dw_tick_write_db_count_	= get_status( ).dw_write_count_ - get_status( ).dw_last_write_count_;

	get_status( ).dw_last_recv_msg_count_	= get_status( ).dw_recv_msg_count_;
	get_status( ).dw_last_proc_msg_count_	= get_status( ).dw_process_msg_count_;
	get_status( ).dw_last_send_msg_count_	= get_status( ).dw_send_msg_count_;

	get_status( ).dw_last_recv_msg_size_	= get_status( ).dw_recv_msg_size_;
	get_status( ).dw_last_send_msg_size_	= get_status( ).dw_send_msg_size_;

	get_status( ).dw_last_read_count_	= get_status( ).dw_read_count_;
	get_status( ).dw_last_write_count_	= get_status( ).dw_write_count_;

	return &get_status( );
}
// 主循环
VOID db_server::main_loop()
{
	DWORD dw_msg, dw_param1, dw_param2;
	
	while(!get_window()->message_loop() && !is_terminate())
	{	
		if(FALSE == get_window()->is_window_active())
		{
			Sleep(50);
			continue;
		}
		
		while(get_window()->peek_window_message(dw_msg, dw_param1, dw_param2))
		{
			if(dw_msg == WM_QUIT)
				return;
			if(dw_msg == WM_MYSELF_USER)
			{
				if(dw_param1 == 1)
				{
					get_window()->print_list();
				}
			}
		}
		
		// 刷界面
		refresh_window();
		Sleep(500);
	}
}
// 读取不在线留言
DWORD db_server::load_leave_msg( DWORD p_msg, DWORD dw_reserve )
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_load_left_message);

	BYTE buffer[1024 * MAX_LEFTMSG_NUM] = {0};
	NET_DB2S_load_left_message* p_send = reinterpret_cast<NET_DB2S_load_left_message*>(buffer);
	p_send->dw_message_id = get_tool()->crc32("NET_DB2S_load_left_message");
	p_send->dw_role_id = p_recv->dw_role_id;

	DWORD dw_all_leave_msg_size = 0;
	BOOL b_has = db_handler_->load_leave_msg(p_recv->dw_role_id, p_send->by_data, dw_all_leave_msg_size);
	if (b_has) p_send->dw_size = sizeof(NET_DB2S_load_left_message) - 1 + dw_all_leave_msg_size;
	else p_send->dw_size = sizeof(NET_DB2S_load_left_message);
	
	p_server_->send_msg(GAMESERVERSESSIONID, p_send, p_send->dw_size);
	return E_Success;
}

DWORD db_server::insert_leave_msg( DWORD p_msg, DWORD dw_reserve )
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_insert_left_message);
	DWORD dw_indexes[MAX_LEFTMSG_NUM] = {0};
	int n_count = 0;
	db_handler_->load_leave_msg_indexes(dw_indexes, p_recv->data.dw_role_id, n_count);
	if (n_count >= MAX_LEFTMSG_NUM)
	{
		db_handler_->delete_leave_msg(p_recv->data.dw_role_id, dw_indexes[0]);
	}
	db_handler_->insert_leave_msg(p_recv->data.dw_role_id, p_recv->data.dw_date_time, &(p_recv->data));
	return E_Success;
}

DWORD db_server::clear_leave_msg( DWORD p_msg, DWORD dw_reserve )
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_clear_left_message);
	db_handler_->clear_leave_msg(p_recv->dw_role_id);
	return E_Success;
}
// 更新角色属性
DWORD db_server::update_role_attribute( DWORD p_msg, DWORD dw_reserve )
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_update_role_att);
	db_handler_->update_role_attribute(p_recv->dw_role_id, p_recv);
	return E_Success;
}
// 更新角色属性点
DWORD db_server::update_role_attribute_point( DWORD p_msg, DWORD dw_reserve )
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_update_role_att_point);
	db_handler_->update_role_attribute_point(p_recv->dw_role_id, (PVOID)p_msg);
	return E_Success;
}
// 更新角色天赋点
DWORD db_server::update_role_talent_point( DWORD p_msg, DWORD dw_reserve )
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_update_role_talent_point);
	db_handler_->update_role_talent_point(p_recv->dw_role_id, (PVOID)p_msg);
	return E_Success;
}

// 清除角色不在线经验
DWORD db_server::clear_role_leave_exp(DWORD p_msg, DWORD dw_reserve)
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_clean_role_leave_exp);
	db_handler_->clear_role_leave_exp(p_recv->dw_role_id, (PVOID)p_msg);
	return E_Success;
}

// 清除角色不在线义气值
DWORD db_server::clear_role_leave_brotherhood(DWORD p_msg, DWORD dw_reserve)
{
	M_trans_pointer(p_recv, p_msg, NET_DB2C_clean_role_leave_brother);
	db_handler_->clear_role_leave_brotherhood(p_recv->dw_role_id, (PVOID)p_msg);
	return E_Success;
}

// 更新角色挂机次数
DWORD db_server::clean_role_hang_number(DWORD p_msg, DWORD dw_reserve)
{
	db_handler_->clear_role_hang_count(INVALID_VALUE, NULL);
	return E_Success;
}

// 更新签到数据
DWORD db_server::clean_role_sign_data(DWORD p_msg, DWORD dw_reserver)
{
	db_handler_->clean_role_sign(INVALID_VALUE, NULL);
	return E_Success;
}

DWORD db_server::clean_role_day_clear(DWORD p_msg, DWORD dw_reserve)
{
	db_handler_->clear_role_day_clear(INVALID_VALUE, NULL);
	return E_Success;
}
// 更新元宝兑换次数
DWORD db_server::clean_role_exchange_num(DWORD p_msg,	DWORD dw_reserve)
{
	db_handler_->clear_role_yuanbao_exchange_num(INVALID_VALUE, NULL);
	return E_Success;
}

DWORD db_server::reset_role_at_0(DWORD p_msg, DWORD dw_reserve)
{
	NET_C2DB_reset_role_value_at_0 *pProtocol = (NET_C2DB_reset_role_value_at_0*)p_msg;
	db_handler_->reset_role_at_0(INVALID_VALUE, pProtocol);
	return E_Success;
}

// 更新角色拍卖次数
DWORD db_server::clean_role_paimai_number(DWORD p_msg, DWORD dw_reserver)
{
	db_handler_->clear_role_paimai_count(INVALID_VALUE, NULL);
	return E_Success;
}
// 更新配偶
DWORD db_server::clean_role_spouse_id(DWORD p_msg, DWORD dw_reserve)
{
	NET_C2DB_clean_role_spouse_id* p_recv = (NET_C2DB_clean_role_spouse_id*)p_msg;
	db_handler_->clear_role_spouse_id(p_recv->dw_role_id,p_recv);
	return E_Success;
}

// 更行角色钱庄拍卖次数
DWORD db_server::clean_role_bank_number(DWORD p_msg, DWORD dw_reserver)
{
	db_handler_->clear_role_bank_count(INVALID_VALUE, NULL);
	return E_Success;
}

// 更新战功商店购买次数
DWORD db_server::clean_role_exploits_num(DWORD p_msg, DWORD dw_reserver)
{
	db_handler_->clean_role_exploits_num(INVALID_VALUE, NULL);
	return E_Success;
}

// 更新活跃度数据
DWORD db_server::clean_role_active_data(DWORD p_msg, DWORD dw_reserver)
{
	db_handler_->clean_role_active_data(INVALID_VALUE, NULL);
	return E_Success;
}

// 更新1v1每日积分
DWORD db_server::update_day_1v1_score(DWORD p_msg, DWORD dw_reserve)
{
	db_handler_->update_day_1v1_score(INVALID_VALUE, NULL);
	return E_Success;
}

// 更新1v1周积分
DWORD db_server::update_week_1v1_score(DWORD p_msg, DWORD dw_reserve)
{
	db_handler_->update_week_1v1_score(INVALID_VALUE, NULL);
	db_handler_->reset_role_1v1_score(INVALID_VALUE, NULL);
	return E_Success;
}

//更新1v1领奖标志
DWORD db_server::update_1v1_award(DWORD p_msg, DWORD dw_reserve)
{
	NET_C2DB_update_1v1_award* p_recv = (NET_C2DB_update_1v1_award*)p_msg;

	db_handler_->update_1v1_award(p_recv->dw_role_id, p_recv);

	return E_Success;
}

// 重置玩家元气值
DWORD db_server::reset_all_role_vigour(DWORD p_msg, DWORD dw_reserve)
{
	db_handler_->reset_all_role_vigour(INVALID_VALUE,NULL);
	return E_Success;
}

DWORD db_server::clean_role_luck(DWORD p_msg, DWORD dw_reserve)
{
	db_handler_->clean_all_role_luck(INVALID_VALUE, NULL);
	return E_Success;
}

// 更新噬魂
DWORD db_server::update_shihun(DWORD p_msg, DWORD dw_reserve)
{
	db_handler_->clean_shihun(INVALID_VALUE, NULL);
	return E_Success;
}

DWORD db_server::inc_circle_refreshnumber_and_questnum(DWORD p_msg, DWORD dw_reserve)
{
	NET_C2DB_UpdateCircleRefreshNumberAndQuestNum *p_recv = (NET_C2DB_UpdateCircleRefreshNumberAndQuestNum*)p_msg;

	db_handler_->inc_circle_refreshnumber_and_questnum(INVALID_VALUE, p_recv);
	return E_Success;
}

DWORD db_server::clear_destory_equip_count(DWORD p_msg, DWORD dw_reserve)
{
	db_handler_->clear_destory_equip_count(INVALID_VALUE, NULL);
	return E_Success;
}

DWORD db_server::update_role_1v1_score(DWORD p_msg,	DWORD dw_reserve)
{
	NET_DB2C_update_role_1v1_score* p_recv = (NET_DB2C_update_role_1v1_score*)p_msg;

	db_handler_->update_role_1v1_score(p_recv->dw_role_id, p_recv);

	return E_Success;
}

DWORD db_server::update_noline_1v1_score(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_noline_role_1v1_score* p_recv = (NET_DB2C_update_noline_role_1v1_score*)p_msg;

	db_handler_->update_noline_1v1_score(p_recv->dw_role_id, p_recv);

	return E_Success;
}

// 坐骑
DWORD db_server::updata_mounts(DWORD p_msg, DWORD dw_reserve)
{
	NET_DB2C_update_mounts* p_recv = (NET_DB2C_update_mounts*)p_msg;


	db_handler_->update_mounts(p_recv->dw_role_id, p_recv);

	return E_Success;
}
