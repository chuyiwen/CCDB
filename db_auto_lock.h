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
 *	@file		db_auto_lock
 *	@author		mwh
 *	@date		2010/10/11	initial
 *	@version	0.0.1.0
 *	@brief		自解锁
*/


#ifndef __ASYN_LOCK_H__
#define __ASYN_LOCK_H__

#include <Windows.h>
#include <memory>

// You can improve performance significantly by choosing a small spin count for a critical section of short duration. 
// The heap manager uses a spin count of roughly <!!!! 4000 > for its per-heap critical sections. 
// This gives great performance and scalability in almost all worst-case scenarios.

class non_copyable
{
public:
	non_copyable( ){ };
private:
	non_copyable(const non_copyable&);
	non_copyable& operator=( const non_copyable&);
};

class db_mutex : public non_copyable
{
	CRITICAL_SECTION section_;
public:
 db_mutex( DWORD spin_count = 4000 )
 {
	#if (_WIN32_WINNT >= 0x0403)
		::InitializeCriticalSectionAndSpinCount(&section_, spin_count);
	#else	
		::InitializeCriticalSection(&section_);
	#endif
 }
 ~db_mutex(){ ::DeleteCriticalSection(&section_); }
public:
 void lock()
 { 
#if(_WIN32_WINNT >= 0x0400)
	 if(FALSE == TryEnterCriticalSection(&section_)) {
#endif
		::EnterCriticalSection(&section_);
#if(_WIN32_WINNT >= 0x0400)
	 }
#endif
 }
 void unlock(){ ::LeaveCriticalSection(&section_);}
};

 //自解锁
class auto_lock : public non_copyable
{
  db_mutex &mutex_;
public:
  auto_lock(db_mutex& rg): mutex_(rg){ mutex_.lock(); }
  ~auto_lock(){ mutex_.unlock();}
};

#define  __auto_lock__(lock) auto_lock __FUNCTION__##__LINE__(lock) 

#endif //__ASYN_LOCK_H__