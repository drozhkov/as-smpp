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

#ifndef __AS_SMPP__SMPP__H
#define __AS_SMPP__SMPP__H


#include <stdbool.h>

#include "as-smpp-core/core.h"
#include "as-smpp-core/session.h"
#include "as-smpp-core/smpp-types.h"


#define AS_SMPP_INIT_ADDRESS( a_varName )                                      \
	a_varName.address = a_varName.addressBuffer;

#define AS_SMPP_INIT_PDU_SUBMIT_SM( a_varName )                                \
	a_varName.serviceType = a_varName.serviceTypeBuffer;                       \
	AS_SMPP_INIT_ADDRESS( a_varName.destinationAddress );                      \
	AS_SMPP_INIT_ADDRESS( a_varName.sourceAddress );                           \
	a_varName.message = a_varName.messageBuffer;

#define AS_SMPP_INIT_PDU_DELIVER_SM( a_varName )                               \
	AS_SMPP_INIT_PDU_SUBMIT_SM( a_varName )

#define AS_SMPP_DECLARE_PDU_SUBMIT_SM( a_varName )                             \
	t_as_smpp_pdu_deliver_sm a_varName;                                        \
	AS_SMPP_INIT_PDU_SUBMIT_SM( a_varName );

#define AS_SMPP_DECLARE_PDU_DELIVER_SM( a_varName )                            \
	AS_SMPP_DECLARE_PDU_SUBMIT_SM( a_varName )

#define AS_SMPP_DECLARE_PDU_SUBMIT_SM_RESPONSE( a_varName )                    \
	t_as_smpp_pdu_submit_sm_response a_varName;                                \
	a_varName.messageId = a_varName.messageIdBuffer;


#ifdef __cplusplus
extern "C" {
#endif


void as_smpp_tlv_free( t_as_smpp_tlv * a_tlv );

t_as_smpp_result as_smpp_connect( t_as_smpp_session_handle a_session,
	const char * a_hostname,
	uint16_t a_portNo );

t_as_smpp_result as_smpp_submit_sm( t_as_smpp_session_handle a_session,
	t_as_smpp_sequence_no * a_sequenceNo,
	const char * a_serviceType,
	const t_as_smpp_address * a_sourceAddress,
	const t_as_smpp_address * a_destinationAddress,
	t_as_smpp_data_coding a_dataCoding,
	const t_as_smpp_buffer * a_message );

t_as_smpp_result as_smpp_enquire_link_response(
	t_as_smpp_session_handle a_session, const t_as_smpp_pdu_header * a_header );

t_as_smpp_result as_smpp_deliver_sm_response(
	t_as_smpp_session_handle a_session, const t_as_smpp_pdu_header * a_header );

t_as_smpp_result as_smpp_bind( t_as_smpp_session_handle a_session,
	const char * a_systemId,
	const char * a_password,
	const char * a_systemType,
	t_as_smpp_ton a_addressTon,
	t_as_smpp_npi a_addressNpi,
	const char * a_addressRange );


#ifdef __cplusplus
}
#endif


#endif
