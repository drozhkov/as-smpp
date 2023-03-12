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

#ifndef __AS_SMPP__SESSION__H
#define __AS_SMPP__SESSION__H


#include "as-smpp-core/core.h"
#include "as-smpp-core/smpp-types.h"


struct as_smpp_session_t;
typedef struct as_smpp_session_t t_as_smpp_session, *t_as_smpp_session_handle;

typedef enum {
	AS_SMPP_SESSION_STATE_UNDEFINED = 0,
	AS_SMPP_SESSION_STATE_OPENED = 1,
	AS_SMPP_SESSION_STATE_BOUND_TX = 2 | AS_SMPP_SESSION_STATE_OPENED,
	AS_SMPP_SESSION_STATE_BOUND_RX = 4 | AS_SMPP_SESSION_STATE_OPENED,
	AS_SMPP_SESSION_STATE_BOUND_TRX = 6 | AS_SMPP_SESSION_STATE_OPENED,
	AS_SMPP_SESSION_STATE_CLOSED = 8
} t_as_smpp_session_state;


#ifdef __cplusplus
extern "C" {
#endif


/// @brief
/// @param
/// @return
t_as_smpp_session_handle as_smpp_session_create( void );

/// @brief
/// @param a_session
/// @return
t_as_smpp_result as_smpp_session_destroy( t_as_smpp_session_handle a_session );

t_as_smpp_session_state as_smpp_session_get_state(
	t_as_smpp_session_handle a_session );

t_as_smpp_result as_smpp_session_wait_for_state(
	t_as_smpp_session_handle a_session, t_as_smpp_session_state a_state );

/// @brief
/// @param a_session
/// @param a_handler t_as_smpp_sequence_no, t_as_smpp_command_status
/// @return
t_as_smpp_result as_smpp_set_handler_bind_response(
	t_as_smpp_session_handle a_session,
	t_as_smpp_handler_bind_response a_handler );

/// @brief
/// @param a_session
/// @param a_handler t_as_smpp_sequence_no, const char *
/// @return
t_as_smpp_result as_smpp_set_handler_submit_sm_response(
	t_as_smpp_session_handle a_session,
	t_as_smpp_handler_submit_sm_response a_handler );

/// @brief
/// @param a_session
/// @param a_handler t_as_smpp_sequence_no, t_as_smpp_command_status, const
/// t_as_smpp_pdu_deliver_sm *
/// @return
t_as_smpp_result as_smpp_set_handler_deliver_sm(
	t_as_smpp_session_handle a_session,
	t_as_smpp_handler_deliver_sm a_handler );


#ifdef __cplusplus
}
#endif


#endif
