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

#include <stdlib.h>

#include "as-smpp-core/session.h"

#include "../include/private-threads.h"
#include "../include/private-session.h"


#define AS_SMPP_BUFFER_INIT_CAPACITY ( 1024 * 8 )


t_as_smpp_result as_smpp_session_set_state(
	t_as_smpp_session_handle a_session, t_as_smpp_session_state a_state )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_mutex_lock( &a_session->stateMutex );
	a_session->state = a_state;
	as_smpp_mutex_unlock( &a_session->stateMutex );

	as_smpp_condition_signal_all( &a_session->stateCondition );

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_session_handle as_smpp_session_create( void )
{
	t_as_smpp_session_handle session = malloc( sizeof( t_as_smpp_session ) );
	session->state = AS_SMPP_SESSION_STATE_UNDEFINED;
	session->handlerSubmitResponse = NULL;
	session->handlerDeliver = NULL;
	session->handlerBindResponse = NULL;

	as_smpp_buffer_init(
		&session->receiveBuffer, AS_SMPP_BUFFER_INIT_CAPACITY );

	as_smpp_buffer_init( &session->sendBuffer, AS_SMPP_BUFFER_INIT_CAPACITY );

	as_smpp_mutex_init( &session->sendMutex, AS_SMPP_MUTEX_TYPE_PLAIN );

	as_smpp_condition_init( &session->stateCondition );
	as_smpp_mutex_init( &session->stateMutex, AS_SMPP_MUTEX_TYPE_PLAIN );

	return session;
}

t_as_smpp_result as_smpp_session_destroy( t_as_smpp_session_handle a_session )
{
	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_mutex_destroy( &a_session->stateMutex );
	as_smpp_condition_destroy( &a_session->stateCondition );
	as_smpp_mutex_destroy( &a_session->sendMutex );
	as_smpp_mutex_destroy( &a_session->sendMutex );
	as_smpp_buffer_destroy( &a_session->sendBuffer );
	as_smpp_buffer_destroy( &a_session->receiveBuffer );

	free( a_session );

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_session_state as_smpp_session_get_state(
	t_as_smpp_session_handle a_session )
{

	return a_session->state;
}

t_as_smpp_result as_smpp_session_wait_for_state(
	t_as_smpp_session_handle a_session, t_as_smpp_session_state a_state )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_mutex_lock( &a_session->stateMutex );

	while ( a_session->state != a_state ) {
		as_smpp_condition_wait(
			&a_session->stateCondition, &a_session->stateMutex, NULL );
	}

	as_smpp_mutex_unlock( &a_session->stateMutex );

	as_smpp_condition_signal_all( &a_session->stateCondition );

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_set_handler_bind_response(
	t_as_smpp_session_handle a_session,
	t_as_smpp_handler_bind_response a_handler )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	a_session->handlerBindResponse = a_handler;

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_set_handler_submit_sm_response(
	t_as_smpp_session_handle a_session,
	t_as_smpp_handler_submit_sm_response a_handler )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	a_session->handlerSubmitResponse = a_handler;

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_set_handler_deliver_sm(
	t_as_smpp_session_handle a_session, t_as_smpp_handler_deliver_sm a_handler )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	a_session->handlerDeliver = a_handler;

	AS_SMPP_RETURN_RESULT();
}
