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
#include "db_handler.h"
#include "db_server_handler.h"

// 数据库服务器

int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, int)
{
	// 禁止电源管理
	::SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, 0, NULL, 0);
	// 禁止屏保
	::SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, 0, NULL, 0);

	memorysystem::init_new_protect();
	serverbase::init_network();
	serverbase::init_serverbase();
	THROW_EXCEPTION;

	db_server *p_server = new db_server;
	if(!p_server->start(hInst))
	{
		SAFE_DELETE(p_server);
		ERROR_CLUE_ON(_T("DBServer initial failed!\r\n"));
		return EXIT_FAILURE;
	}

	print_message(_T("DBServer is running!!\r\n"));
	p_server->main_loop();

	SAFE_DELETE(p_server);

	serverbase::destroy_serverbase();
	serverbase::destroy_network();
	return EXIT_SUCCESS;
}

