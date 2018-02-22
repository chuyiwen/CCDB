/*******************************************************************************

Copyright 2010 by tiankong Interactive Game Co., Ltd.
All rights reserved.

This software is the confidential and proprietary information of
tiankong Interactive Game Co., Ltd. ('Confidential Information'). You shall
not disclose such Confidential Information and shall use it only in
accordance with the terms of the license agreement you entered into with
tiankong Interactive  Co., Ltd.

*******************************************************************************/

#ifndef __MONTIORING_SESSION_H__
#define __MONTIORING_SESSION_H__

class db_server;
// 数据库服务器与监视服务器连接
class monitioring_session
{
public:
	monitioring_session( );
	~monitioring_session();
public:
	//初始化 
	BOOL	start(db_server *pGame);
	//销毁
	VOID	destroy();
	//更新
	VOID	update();
public:
	//是否已连接
	BOOL	is_connected()	{ return net_session_->is_connect(); }
private:
	VOID	send_server_info();
	VOID	register_command();
	VOID	update_sessions();

	UINT	thread_connect();
	static UINT WINAPI static_thread_connect(LPVOID p_data);

private:
	DWORD	handle_server_connected(tag_net_message* p_msg, DWORD);
	DWORD	handle_server_disconnect(tag_net_message* p_msg, DWORD);

private:
	
	few_connect_client* net_session_;

	tstring ip_;
	DWORD dw_port_;
	DWORD dw_session_id_;
	DWORD dw_world_id_;

	db_server* db_server_;

	volatile BOOL b_terminate_;
	volatile BOOL b_init_ok_;
};


extern monitioring_session g_monitioring_session;

#endif //__MONTIORING_SESSION_H__
