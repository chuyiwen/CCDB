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
 *	@file		db_define
 *	@author		mwh
 *	@date		2010/10/11	initial
 *	@version	0.0.1.0
 *	@brief		基本定义
*/

#ifndef __DATABASE_DEFINE_H__

#define __DATABASE_DEFINE_H__

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;
typedef __int64		   int64;

#define GAMESERVERSESSIONID	 1
#define SLEEPTIME 100

#define _TFILE		_T(__FILE__)
#define _FUNCTION	_T(__FUNCTION__)
#define _TLINE		__LINE__

#endif //__DATABASE_DEFINE_H__