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

#ifndef __AS_SMPP__PRIVATE_SESSION__H
#define __AS_SMPP__PRIVATE_SESSION__H


#include "as-smpp-core/core.h"
#include "as-smpp-core/smpp.h"

#include "private-net.h"
#include "private-threads.h"


struct as_smpp_session_t {
	volatile t_as_smpp_session_state state;
	t_as_smpp_condition stateCondition;
	t_as_smpp_mutex stateMutex;
	t_as_smpp_socket_handle socketHandle;
	uint32_t sequenceNo;
	t_as_smpp_version smppVersion;
	t_as_smpp_session_mode mode;
	t_as_smpp_buffer receiveBuffer;
	t_as_smpp_buffer sendBuffer;
	t_as_smpp_mutex sendMutex;
	_Bool useUdh;
	_Bool useMessagePayloadTlv;
	size_t maxSmLength;
	//t_as_smpp_sequence_no, t_as_smpp_command_status 
	t_as_smpp_handler_bind_response handlerBindResponse;
	/// @brief t_as_smpp_sequence_no, t_as_smpp_command_status, const char *
	t_as_smpp_handler_submit_sm_response handlerSubmitResponse;
	/// @brief t_as_smpp_sequence_no, t_as_smpp_command_status, const
	/// t_as_smpp_pdu_deliver_sm *
	t_as_smpp_handler_deliver_sm handlerDeliver;
};


#ifdef __cplusplus
extern "C" {
#endif


t_as_smpp_result as_smpp_session_set_state(
	t_as_smpp_session_handle a_session, t_as_smpp_session_state a_state );


#ifdef __cplusplus
}
#endif


#endif
