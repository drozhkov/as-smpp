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
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "../include/private-threads.h"
#include "../include/private-net.h"
#include "../include/private-session.h"
#include "../include/private-serializer.h"

#include "as-smpp-core/smpp.h"


#define AS_SMPP_CALL_HANDLER( a_handler, ... )                                 \
	if ( a_handler != NULL ) {                                                 \
		a_handler( __VA_ARGS__ );                                              \
	}


static uint32_t as_smpp_get_sequence_no( t_as_smpp_session_handle a_session )
{
	uint32_t result = a_session->sequenceNo;

	if ( ++( a_session->sequenceNo ) >= INT32_MAX ) {
		a_session->sequenceNo = 1;
	}

	return result;
}

void as_smpp_tlv_free( t_as_smpp_tlv * a_tlv )
{
	if ( NULL == a_tlv ) {
		return;
	}

	do {
		void * p = a_tlv;
		a_tlv = a_tlv->next;
		free( p );
	}
	while ( a_tlv != NULL );
}

static t_as_smpp_result as_smpp_send( t_as_smpp_session_handle a_session )
{
	size_t dataLen = a_session->sendBuffer.dataLen;
	a_session->sendBuffer.dataLen = 0;

	AS_SMPP_SERIALIZE_COMMAND_LENGTH(
		&a_session->sendBuffer, (uint32_t)dataLen );

	a_session->sendBuffer.dataLen = dataLen;

	return as_smpp_net_send( a_session->socketHandle, &a_session->sendBuffer );
}

t_as_smpp_result as_smpp_submit_sm( t_as_smpp_session_handle a_session,
	t_as_smpp_sequence_no * a_sequenceNo,
	const char * a_serviceType,
	const t_as_smpp_address * a_sourceAddress,
	const t_as_smpp_address * a_destinationAddress,
	t_as_smpp_data_coding a_dataCoding,
	const t_as_smpp_buffer * a_message )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_mutex_lock( &a_session->sendMutex );

	t_as_smpp_buffer * buffer = &a_session->sendBuffer;
	buffer->dataLen = 0;

	*a_sequenceNo = as_smpp_get_sequence_no( a_session );

	as_smpp_serialize_pdu_header( buffer,
		0,
		AS_SMPP_COMMAND_SUBMIT_SM,
		AS_SMPP_COMMAND_STATUS_ROK,
		*a_sequenceNo );

	as_smpp_serialize_pdu_submit_sm( buffer,
		a_serviceType,
		a_sourceAddress,
		a_destinationAddress,
		a_dataCoding,
		a_message );

	as_smpp_send( a_session );

	as_smpp_mutex_unlock( &a_session->sendMutex );

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_enquire_link_response(
	t_as_smpp_session_handle a_session, const t_as_smpp_pdu_header * a_header )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_mutex_lock( &a_session->sendMutex );

	t_as_smpp_buffer * buffer = &a_session->sendBuffer;
	buffer->dataLen = 0;

	as_smpp_serialize_pdu_header( buffer,
		0,
		AS_SMPP_COMMAND_ENQUIRE_LINK | AS_SMPP_COMMAND_RESPONSE_FLAG,
		AS_SMPP_COMMAND_STATUS_ROK,
		a_header->sequenceNo );

	as_smpp_send( a_session );

	as_smpp_mutex_unlock( &a_session->sendMutex );

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_deliver_sm_response(
	t_as_smpp_session_handle a_session, const t_as_smpp_pdu_header * a_header )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_mutex_lock( &a_session->sendMutex );

	t_as_smpp_buffer * buffer = &a_session->sendBuffer;
	buffer->dataLen = 0;

	as_smpp_serialize_pdu_header( buffer,
		0,
		AS_SMPP_COMMAND_ENQUIRE_LINK | AS_SMPP_COMMAND_RESPONSE_FLAG,
		AS_SMPP_COMMAND_STATUS_ROK,
		a_header->sequenceNo );

	as_smpp_serialize_string( buffer, NULL, AS_SMPP_LENGTH_MESSAGE_ID );

	as_smpp_send( a_session );

	as_smpp_mutex_unlock( &a_session->sendMutex );

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_bind( t_as_smpp_session_handle a_session,
	const char * a_systemId,
	const char * a_password,
	const char * a_systemType,
	t_as_smpp_ton a_addressTon,
	t_as_smpp_npi a_addressNpi,
	const char * a_addressRange )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_mutex_lock( &a_session->sendMutex );

	t_as_smpp_buffer * buffer = &a_session->sendBuffer;
	buffer->dataLen = 0;

	as_smpp_serialize_pdu_header( buffer,
		0,
		a_session->mode == AS_SMPP_SESSION_MODE_TRANSMITTER
			? AS_SMPP_COMMAND_BIND_TRANSMITTER
			: a_session->mode == AS_SMPP_SESSION_MODE_RECEIVER
			? AS_SMPP_COMMAND_BIND_RECEIVER
			: AS_SMPP_COMMAND_BIND_TRANSCEIVER,
		AS_SMPP_COMMAND_STATUS_ROK,
		a_session->sequenceNo++ );

	AS_SMPP_SERIALIZE_SYSTEM_ID( buffer, a_systemId );
	AS_SMPP_SERIALIZE_PASSWORD( buffer, a_password );
	AS_SMPP_SERIALIZE_SYSTEM_TYPE( buffer, a_systemType );
	AS_SMPP_SERIALIZE_VERSION( buffer, a_session->smppVersion );
	AS_SMPP_SERIALIZE_TON( buffer, a_addressTon );
	AS_SMPP_SERIALIZE_NPI( buffer, a_addressNpi );
	AS_SMPP_SERIALIZE_ADDRESS_RANGE( buffer, a_addressRange );

	as_smpp_send( a_session );

	as_smpp_mutex_unlock( &a_session->sendMutex );

	AS_SMPP_RETURN_RESULT();
}

static int as_smpp_receive_worker( void * a_arg )
{
	t_as_smpp_session_handle session = (t_as_smpp_session_handle)a_arg;
	t_as_smpp_buffer buffer = { .ptr = ( uint8_t[1024] ){ 0 },
		.capacity = 1024 };

	t_as_smpp_pdu_header header;

	while ( true ) {
		AS_SMPP_JUMP_IF(
			as_smpp_net_receive( session->socketHandle, &buffer ).errorCode !=
				AS_SMPP_ERROR_NONE,
			exit );

		as_smpp_buffer_copy(
			&session->receiveBuffer, buffer.ptr, buffer.dataLen );

		while ( session->receiveBuffer.dataLen > 0 ) {
			size_t index = 0;

			AS_SMPP_JUMP_IF( as_smpp_deserialize_pdu_header(
								 &header, &session->receiveBuffer, &index )
								 .errorCode != AS_SMPP_ERROR_NONE,
				loop );

			printf( "command ID = %x, status = %u\n",
				header.commandId,
				header.status );

			if ( header.commandId == AS_SMPP_COMMAND_NACK ) {
			}
			else if ( header.commandId & AS_SMPP_COMMAND_RESPONSE_FLAG ) {
				t_as_smpp_command commandId =
					header.commandId ^ AS_SMPP_COMMAND_RESPONSE_FLAG;

				switch ( commandId ) {
					case AS_SMPP_COMMAND_BIND_RECEIVER:
					case AS_SMPP_COMMAND_BIND_TRANSMITTER:
					case AS_SMPP_COMMAND_BIND_TRANSCEIVER: {
						t_as_smpp_pdu_bind_response response;
						response.systemId = response.systemIdBuffer;

						as_smpp_deserialize_string( response.systemId,
							&session->receiveBuffer,
							&index,
							AS_SMPP_LENGTH_SYSTEM_ID );

						AS_SMPP_CALL_HANDLER( session->handlerBindResponse,
							header.sequenceNo,
							header.status );

						as_smpp_session_set_state( session,
							AS_SMPP_COMMAND_BIND_RECEIVER == commandId
								? AS_SMPP_SESSION_STATE_BOUND_RX
								: AS_SMPP_COMMAND_BIND_TRANSMITTER == commandId
								? AS_SMPP_SESSION_STATE_BOUND_TX
								: AS_SMPP_SESSION_STATE_BOUND_TRX );
					}

					break;

					case AS_SMPP_COMMAND_SUBMIT_SM: {
						AS_SMPP_DECLARE_PDU_SUBMIT_SM_RESPONSE( response );
						as_smpp_deserialize_string( response.messageId,
							&session->receiveBuffer,
							&index,
							AS_SMPP_LENGTH_MESSAGE_ID );

						AS_SMPP_CALL_HANDLER( session->handlerSubmitResponse,
							header.sequenceNo,
							header.status,
							response.messageId );
					}

					break;
				}
			}
			else {
				switch ( header.commandId ) {
					case AS_SMPP_COMMAND_ENQUIRE_LINK:
						as_smpp_enquire_link_response( session, &header );
						break;

					case AS_SMPP_COMMAND_DELIVER_SM: {
						AS_SMPP_DECLARE_PDU_DELIVER_SM( request );
						as_smpp_deserialize_pdu_deliver_sm(
							&request, &session->receiveBuffer, &index );

						t_as_smpp_tlv * tlv;
						as_smpp_deserialize_tlv( &tlv,
							&session->receiveBuffer,
							&index,
							header.commandLength );

						while ( tlv != NULL ) {
							tlv = tlv->next;
						}

						AS_SMPP_CALL_HANDLER( session->handlerDeliver,
							header.sequenceNo,
							header.status,
							&request );

						as_smpp_tlv_free( tlv );

						as_smpp_deliver_sm_response( session, &header );
					}

					break;

					default:
						break;
				}
			}

			as_smpp_buffer_remove(
				&session->receiveBuffer, 0, header.commandLength );
		}

		AS_SMPP_CATCH( loop );
	}

	AS_SMPP_CATCH( exit );
	as_smpp_session_set_state( session, AS_SMPP_SESSION_STATE_CLOSED );

	return 0;
}

t_as_smpp_result as_smpp_connect( t_as_smpp_session_handle a_session,
	const char * a_hostname,
	uint16_t a_portNo )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	AS_SMPP_SET_RESULT_CODE_AND_JUMP_IF(
		as_smpp_net_connect( &a_session->socketHandle, a_hostname, a_portNo )
				.errorCode != AS_SMPP_ERROR_NONE,
		exit,
		AS_SMPP_ERROR_NET );

	as_smpp_session_set_state( a_session, AS_SMPP_SESSION_STATE_OPENED );

	t_as_smpp_thread t;
	as_smpp_thread_create( &t, as_smpp_receive_worker, a_session );

	AS_SMPP_CATCH( exit );

	AS_SMPP_RETURN_RESULT();
}
