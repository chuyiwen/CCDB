// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#ifndef __STD_AFX_h__

#define __STD_AFX_h__

//-----------------------------------------------------------------------------
// 引用数据库引擎
//-----------------------------------------------------------------------------
#include "..\common\ODBC/ODBC.h"
#include "..\common\network/network_define.h"
#include "..\common\dump\dump_define.h"
using namespace serverbase;

using namespace odbc;

using namespace networkbase;

using namespace serverdump;

#ifdef _DEBUG
#define X_STRING_RUN_TIME "Debug"
// #pragma comment(lib, "../vsout/ODBC/debug/ODBC.lib")
// #pragma comment(lib, "../vsout/ServerDefine/debug/ServerDefine.lib")
// #pragma comment(lib, "../vsout/network/debug/network.lib")
// #pragma comment(lib,"..\\vsout\\dump\\debug\\dump.lib")
#else
// #define X_STRING_RUN_TIME "Release"
// #pragma comment(lib, "../vsout/ODBC/release/ODBC.lib")
// #pragma comment(lib, "../vsout/ServerDefine/release/ServerDefine.lib")
// #pragma comment(lib, "../vsout/network/release/network.lib")
// #pragma comment(lib,"..\\vsout\\dump\\release\\dump.lib")
#endif

#include "..\common\filesystem\file_define.h"
using namespace filesystem;

#ifdef _DEBUG
#define X_STRING_RUN_TIME "Debug"
//#pragma comment(lib,"..\\vsout\\filesystem\\debug\\filesystem.lib")
#else
#define X_STRING_RUN_TIME "Release"
//#pragma comment(lib,"..\\vsout\\filesystem\\release\\filesystem.lib")
#endif

#include "..\common\serverframe\frame_define.h"
using namespace serverframe;

#ifdef _DEBUG
#define X_STRING_RUN_TIME "Debug"
//#pragma comment(lib,"..\\vsout\\serverframe\\debug\\serverframe.lib")
#else
#define X_STRING_RUN_TIME "Release"
//#pragma comment(lib,"..\\vsout\\serverframe\\release\\serverframe.lib")
#endif

//------------------------------------------------------------------------------

#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"winmm.lib")


#include "db_define.h"
#include "db_auto_lock.h"
#include "../../common/WorldDefine/base_define.h"
#include "../../common/WorldDefine/protocol_common_errorcode.h"
#include "../common/ServerDefine/base_server_define.h"
#include "../../common/WorldDefine/time.h"
//-----------------------------------------------------------------

#endif //__STD_AFX_h__