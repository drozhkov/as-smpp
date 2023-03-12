/*
 *	Copyright (c) 2023 Denis Rozhkov <denis@rozhkoff.com>
 *	This file is part of as-smpp.
 *
 *	as-smpp is free software: you can redistribute it and/or modify it under the
 *	terms of the GNU General Public License as published by the Free Software
 *	Foundation, either version 3 of the License, or (at your option) any later
 *	version.
 *
 *	as-smpp is distributed in the hope that it will be useful, but WITHOUT ANY
 *	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *	FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *	details.
 *
 *	You should have received a copy of the GNU General Public License along with
 *	as-smpp. If not, see <https://www.gnu.org/licenses/>.
 */

#include "../include/private-threads.h"


t_as_smpp_result as_smpp_mutex_init(
	t_as_smpp_mutex * a_mutex, t_as_smpp_mutex_type a_type )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

#ifndef __STDC_NO_THREADS__
	AS_SMPP_SET_RESULT_CODE_IF(
		mtx_init( &a_mutex->v,
			( ( a_type & AS_SMPP_MUTEX_TYPE_PLAIN ) != 0 ? mtx_plain : 0 ) |
				( ( a_type & AS_SMPP_MUTEX_TYPE_TIMED ) != 0 ? mtx_timed : 0 ) |
				( ( a_type & AS_SMPP_MUTEX_TYPE_RECURSIVE ) != 0 ? mtx_recursive
																 : 0 ) ) !=
			thrd_success,
		AS_SMPP_ERROR_MUTEX_INIT );
#else
#endif

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_mutex_destroy( t_as_smpp_mutex * a_mutex )
{
	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

#ifndef __STDC_NO_THREADS__
	mtx_destroy( &a_mutex->v );
#else
#endif

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_mutex_lock( t_as_smpp_mutex * a_mutex )
{
	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

#ifndef __STDC_NO_THREADS__
	AS_SMPP_SET_RESULT_CODE_IF(
		mtx_lock( &a_mutex->v ) != thrd_success, AS_SMPP_ERROR_THREADS );
#else
#endif

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_mutex_unlock( t_as_smpp_mutex * a_mutex )
{
	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

#ifndef __STDC_NO_THREADS__
	AS_SMPP_SET_RESULT_CODE_IF(
		mtx_unlock( &a_mutex->v ) != thrd_success, AS_SMPP_ERROR_THREADS );
#else
#endif

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_condition_init( t_as_smpp_condition * a_condition )
{
	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

#ifndef __STDC_NO_THREADS__
	AS_SMPP_SET_RESULT_CODE_IF( cnd_init( &a_condition->v ) != thrd_success,
		AS_SMPP_ERROR_CONDITION_INIT );
#else
#endif

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_condition_destroy( t_as_smpp_condition * a_condition )
{
	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

#ifndef __STDC_NO_THREADS__
	cnd_destroy( &a_condition->v );
#else
#endif

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_condition_signal( t_as_smpp_condition * a_condition )
{
	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

#ifndef __STDC_NO_THREADS__
	AS_SMPP_SET_RESULT_CODE_IF(
		cnd_signal( &a_condition->v ) != thrd_success, AS_SMPP_ERROR_THREADS );
#else
#endif

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_condition_signal_all(
	t_as_smpp_condition * a_condition )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

#ifndef __STDC_NO_THREADS__
	AS_SMPP_SET_RESULT_CODE_IF(
		cnd_broadcast( &a_condition->v ) != thrd_success,
		AS_SMPP_ERROR_THREADS );
#else
#endif

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_condition_wait( t_as_smpp_condition * a_condition,
	t_as_smpp_mutex * a_mutex,
	const struct timespec * a_untilTs )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

#ifndef __STDC_NO_THREADS__
	if ( NULL == a_untilTs ) {
		AS_SMPP_SET_RESULT_CODE_IF(
			cnd_wait( &a_condition->v, &a_mutex->v ) != thrd_success,
			AS_SMPP_ERROR_THREADS );
	}
	else {
		AS_SMPP_SET_RESULT_CODE_IF(
			cnd_timedwait( &a_condition->v, &a_mutex->v, a_untilTs ) !=
				thrd_success,
			AS_SMPP_ERROR_THREADS );
	}
#else
#endif

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_thread_create( t_as_smpp_thread * a_thread,
	t_as_smpp_thread_start a_threadStart,
	void * a_arg )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

#ifndef __STDC_NO_THREADS__
	AS_SMPP_SET_RESULT_CODE_IF(
		thrd_create( &a_thread->v, a_threadStart, a_arg ) != thrd_success,
		AS_SMPP_ERROR_THREADS );
#else
#endif

	AS_SMPP_RETURN_RESULT();
}
