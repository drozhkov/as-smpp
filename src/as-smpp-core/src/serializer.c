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

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../include/private-serializer.h"


void as_smpp_serialize_integer(
	t_as_smpp_buffer * a_buffer, uint32_t a_value, size_t a_len )
{

	as_smpp_buffer_ensure_capacity( a_buffer, a_len );

	for ( size_t i = 0; i < a_len; ++i ) {
		a_buffer->ptr[a_buffer->dataLen++] =
			( a_value >> ( ( a_len - 1 - i ) * 8 ) ) & 0xff;
	}
}

uint32_t as_smpp_deserialize_integer(
	t_as_smpp_buffer * a_buffer, size_t * a_index, size_t a_len )
{

	uint32_t result = 0;

	for ( size_t i = 0; i < a_len; ++i ) {
		result |= a_buffer->ptr[( *a_index )++] << ( 8 * ( a_len - i - 1 ) );
	}

	return result;
}

void as_smpp_serialize_string(
	t_as_smpp_buffer * a_buffer, const char * a_value, size_t a_maxLen )
{

	as_smpp_buffer_ensure_capacity( a_buffer, a_maxLen );

	if ( NULL == a_value ) {
		a_buffer->ptr[a_buffer->dataLen++] = '\0';
		return;
	}

	size_t valueLen = strlen( a_value ) + 1;
	valueLen = valueLen > a_maxLen ? a_maxLen : valueLen;

	for ( size_t i = 0; i < valueLen; ++i ) {
		a_buffer->ptr[a_buffer->dataLen++] = a_value[i];
	}
}

char * as_smpp_deserialize_string( char * a_value,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index,
	size_t a_valueBufferSize )
{

	size_t len = strlen( (const char *)a_buffer->ptr + *a_index );

	if ( NULL != a_value ) {
		strcpy( a_value, (const char *)a_buffer->ptr + *a_index );
	}

	( *a_index ) += len + 1;

	return a_value;
}

void as_smpp_serialize_binary(
	t_as_smpp_buffer * a_buffer, const uint8_t * a_value, size_t a_len )
{

	if ( 0 == a_len ) {
		return;
	}

	as_smpp_buffer_ensure_capacity( a_buffer, a_len );

	memcpy( a_buffer->ptr + a_buffer->dataLen, a_value, a_len );
	a_buffer->dataLen += a_len;
}

void as_smpp_deserialize_binary( uint8_t * a_value,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index,
	size_t a_valueLen )
{

	memcpy( a_value, a_buffer->ptr + *a_index, a_valueLen );
	( *a_index ) += a_valueLen;
}

t_as_smpp_result as_smpp_serialize_address(
	t_as_smpp_buffer * a_buffer, const t_as_smpp_address * a_address )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_serialize_integer( a_buffer, a_address->ton, AS_SMPP_LENGTH_TON );
	as_smpp_serialize_integer( a_buffer, a_address->npi, AS_SMPP_LENGTH_NPI );
	as_smpp_serialize_string(
		a_buffer, a_address->address, AS_SMPP_LENGTH_ADDRESS );

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_deserialize_address( t_as_smpp_address * a_address,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	a_address->ton =
		as_smpp_deserialize_integer( a_buffer, a_index, AS_SMPP_LENGTH_TON );

	a_address->npi =
		as_smpp_deserialize_integer( a_buffer, a_index, AS_SMPP_LENGTH_NPI );

	as_smpp_deserialize_string(
		a_address->address, a_buffer, a_index, AS_SMPP_LENGTH_ADDRESS );

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_deserialize_datetime( t_as_smpp_datetime * a_datetime,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	char buffer[AS_SMPP_LENGTH_DATETIME];
	as_smpp_deserialize_string( buffer, a_buffer, a_index, sizeof buffer );

	if ( buffer[0] != '\0' ) {
		a_datetime->isNull = true;
	}
	else {
	}

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_deserialize_tlv( t_as_smpp_tlv ** a_tlv,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index,
	size_t a_pduLength )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	*a_tlv = NULL;
	t_as_smpp_tlv * previousTlv = NULL;

	while ( *a_index < a_pduLength ) {
		uint16_t tag = as_smpp_deserialize_integer( a_buffer, a_index, 2 );
		uint16_t length = as_smpp_deserialize_integer( a_buffer, a_index, 2 );
		t_as_smpp_tlv * tlv = malloc( sizeof( t_as_smpp_tlv ) + length );
		tlv->data.ptr = (uint8_t *)( tlv + 1 );
		tlv->tag = tag;
		tlv->length = length;
		tlv->next = NULL;

		as_smpp_deserialize_binary( tlv->data.ptr, a_buffer, a_index, length );

		if ( NULL == previousTlv ) {
			*a_tlv = tlv;
		}
		else {
			previousTlv->next = tlv;
		}

		previousTlv = tlv;
	}

	AS_SMPP_RETURN_RESULT();
}

void as_smpp_serialize_pdu_header( t_as_smpp_buffer * a_buffer,
	t_as_smpp_command_length a_commandLength,
	t_as_smpp_command a_commandId,
	t_as_smpp_command_status a_status,
	t_as_smpp_sequence_no a_sequenceNo )
{

	AS_SMPP_SERIALIZE_COMMAND_LENGTH( a_buffer, a_commandLength );
	AS_SMPP_SERIALIZE_COMMAND_ID( a_buffer, a_commandId );
	AS_SMPP_SERIALIZE_COMMAND_STATUS( a_buffer, a_status );
	AS_SMPP_SERIALIZE_SEQUENCE_NO( a_buffer, a_sequenceNo );
}

t_as_smpp_result as_smpp_deserialize_pdu_header(
	t_as_smpp_pdu_header * a_header,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	AS_SMPP_SET_RESULT_CODE_AND_JUMP_IF(
		a_buffer->dataLen < AS_SMPP_LENGTH_COMMAND_LENGTH,
		exit,
		AS_SMPP_ERROR_INCOMPLETE_DATA );

	a_header->commandLength = as_smpp_deserialize_integer(
		a_buffer, a_index, AS_SMPP_LENGTH_COMMAND_LENGTH );

	AS_SMPP_SET_RESULT_CODE_AND_JUMP_IF(
		a_header->commandLength > a_buffer->dataLen,
		exit,
		AS_SMPP_ERROR_INCOMPLETE_DATA );

	a_header->commandId = as_smpp_deserialize_integer(
		a_buffer, a_index, AS_SMPP_LENGTH_COMMAND_ID );

	a_header->status = as_smpp_deserialize_integer(
		a_buffer, a_index, AS_SMPP_LENGTH_COMMAND_STATUS );

	a_header->sequenceNo = as_smpp_deserialize_integer(
		a_buffer, a_index, AS_SMPP_LENGTH_SEQUENCE_NO );

	AS_SMPP_CATCH( exit );

	AS_SMPP_RETURN_RESULT();
}

void as_smpp_serialize_pdu_submit_sm( t_as_smpp_buffer * a_buffer,
	const char * a_serviceType,
	const t_as_smpp_address * a_sourceAddress,
	const t_as_smpp_address * a_destinationAddress,
	t_as_smpp_data_coding a_dataCoding,
	const t_as_smpp_buffer * a_message )
{

	AS_SMPP_SERIALIZE_SERVICE_TYPE( a_buffer, a_serviceType );
	as_smpp_serialize_address( a_buffer, a_sourceAddress );
	as_smpp_serialize_address( a_buffer, a_destinationAddress );
	as_smpp_serialize_integer(
		a_buffer, AS_SMPP_ESM_CLASS_MODE_DEFAULT, AS_SMPP_LENGTH_ESM_CLASS );

	as_smpp_serialize_integer( a_buffer, 0, AS_SMPP_LENGTH_PROTOCOL_ID );
	as_smpp_serialize_integer( a_buffer, 0, AS_SMPP_LENGTH_PRIORITY_FLAG );
	as_smpp_serialize_integer( a_buffer, 0, 1 ); // delivery time
	as_smpp_serialize_integer( a_buffer, 0, 1 ); // validity period
	as_smpp_serialize_integer( a_buffer,
		AS_SMPP_REGISTERED_DELIVERY_RECEIPT_SUCCESS_OR_FAILURE,
		AS_SMPP_LENGTH_REGISTERED_DELIVERY );

	as_smpp_serialize_integer(
		a_buffer, 0, AS_SMPP_LENGTH_REPLACE_IF_PRESENT_FLAG );

	as_smpp_serialize_integer(
		a_buffer, a_dataCoding, AS_SMPP_LENGTH_DATA_CODING );

	as_smpp_serialize_integer(
		a_buffer, 0, AS_SMPP_LENGTH_SM_DEFAULT_MESSAGE_ID );

	as_smpp_serialize_integer(
		a_buffer, a_message->dataLen, AS_SMPP_LENGTH_SM_LENGTH );

	as_smpp_serialize_binary( a_buffer, a_message->ptr, a_message->dataLen );
}

t_as_smpp_result as_smpp_deserialize_pdu_submit_sm(
	t_as_smpp_pdu_submit_sm * a_pdu,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_deserialize_string(
		a_pdu->serviceType, a_buffer, a_index, AS_SMPP_LENGTH_SERVICE_TYPE );

	as_smpp_deserialize_address( &a_pdu->sourceAddress, a_buffer, a_index );
	as_smpp_deserialize_address(
		&a_pdu->destinationAddress, a_buffer, a_index );

	a_pdu->esmClass = as_smpp_deserialize_integer(
		a_buffer, a_index, AS_SMPP_LENGTH_ESM_CLASS );

	a_pdu->protocolId = as_smpp_deserialize_integer(
		a_buffer, a_index, AS_SMPP_LENGTH_PROTOCOL_ID );

	a_pdu->priority = as_smpp_deserialize_integer(
		a_buffer, a_index, AS_SMPP_LENGTH_PRIORITY_FLAG );

	as_smpp_deserialize_datetime(
		&a_pdu->scheduleDeliveryTime, a_buffer, a_index );

	as_smpp_deserialize_datetime( &a_pdu->validityPeriod, a_buffer, a_index );
	a_pdu->registeredDelivery = as_smpp_deserialize_integer(
		a_buffer, a_index, AS_SMPP_LENGTH_REGISTERED_DELIVERY );

	a_pdu->replaceIfPresent =
		as_smpp_deserialize_integer(
			a_buffer, a_index, AS_SMPP_LENGTH_REPLACE_IF_PRESENT_FLAG ) == 1;

	a_pdu->dataCoding = as_smpp_deserialize_integer(
		a_buffer, a_index, AS_SMPP_LENGTH_DATA_CODING );

	a_pdu->defaultMessageId = as_smpp_deserialize_integer(
		a_buffer, a_index, AS_SMPP_LENGTH_SM_DEFAULT_MESSAGE_ID );

	a_pdu->messageLength = as_smpp_deserialize_integer(
		a_buffer, a_index, AS_SMPP_LENGTH_SM_LENGTH );

	as_smpp_deserialize_binary(
		a_pdu->message, a_buffer, a_index, a_pdu->messageLength );

	a_pdu->message[a_pdu->messageLength] = '\0';

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_deserialize_pdu_deliver_sm(
	t_as_smpp_pdu_deliver_sm * a_pdu,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index )
{

	return as_smpp_deserialize_pdu_submit_sm( a_pdu, a_buffer, a_index );
}
