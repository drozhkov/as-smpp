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

#ifndef __AS_SMPP__PRIVATE_THREADS__H
#define __AS_SMPP__PRIVATE_THREADS__H


#ifndef __STDC_NO_THREADS__
#include <threads.h>
#else
#endif

#include "as-smpp-core/core.h"


typedef enum {
	AS_SMPP_MUTEX_TYPE_PLAIN = 1,
	AS_SMPP_MUTEX_TYPE_TIMED = 2,
	AS_SMPP_MUTEX_TYPE_RECURSIVE = 4
} t_as_smpp_mutex_type;

typedef struct as_smpp_mutex_t {
#ifndef __STDC_NO_THREADS__
	mtx_t v;
#else
	int v;
#endif
} t_as_smpp_mutex;

typedef struct as_smpp_condition_t {
#ifndef __STDC_NO_THREADS__
	cnd_t v;
#else
	int v;
#endif
} t_as_smpp_condition;

typedef struct as_smpp_once_flag_t {
#ifndef __STDC_NO_THREADS__
	once_flag v;
#else
	int v;
#endif
} t_as_smpp_once_flag;

typedef struct as_smpp_thread_t {
#ifndef __STDC_NO_THREADS__
	thrd_t v;
#else
	int v;
#endif
} t_as_smpp_thread;

typedef int ( *t_as_smpp_thread_start )( void * );


#ifdef __cplusplus
extern "C" {
#endif


t_as_smpp_result as_smpp_mutex_init(
	t_as_smpp_mutex * a_mutex, t_as_smpp_mutex_type a_type );

t_as_smpp_result as_smpp_mutex_destroy( t_as_smpp_mutex * a_mutex );
t_as_smpp_result as_smpp_mutex_lock( t_as_smpp_mutex * a_mutex );
t_as_smpp_result as_smpp_mutex_unlock( t_as_smpp_mutex * a_mutex );

t_as_smpp_result as_smpp_condition_init( t_as_smpp_condition * a_condition );
t_as_smpp_result as_smpp_condition_destroy( t_as_smpp_condition * a_condition );
t_as_smpp_result as_smpp_condition_signal( t_as_smpp_condition * a_condition );
t_as_smpp_result as_smpp_condition_signal_all(
	t_as_smpp_condition * a_condition );

t_as_smpp_result as_smpp_condition_wait( t_as_smpp_condition * a_condition,
	t_as_smpp_mutex * a_mutex,
	const struct timespec * a_untilTs );

t_as_smpp_result as_smpp_thread_create( t_as_smpp_thread * a_thread,
	t_as_smpp_thread_start a_threadStart,
	void * a_arg );


#ifdef __cplusplus
}
#endif


#endif
