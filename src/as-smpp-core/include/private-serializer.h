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

#ifndef __AS_SMPP__PRIVATE_SERIALIZER__H
#define __AS_SMPP__PRIVATE_SERIALIZER__H


#include "as-smpp-core/smpp.h"


#define AS_SMPP_SERIALIZE_COMMAND_LENGTH( a_buffer, a_value )                  \
	as_smpp_serialize_integer( a_buffer, a_value, 4 )

#define AS_SMPP_SERIALIZE_COMMAND_ID( a_buffer, a_value )                      \
	as_smpp_serialize_integer( a_buffer, a_value, 4 )

#define AS_SMPP_SERIALIZE_COMMAND_STATUS( a_buffer, a_value )                  \
	as_smpp_serialize_integer( a_buffer, a_value, 4 )

#define AS_SMPP_SERIALIZE_SEQUENCE_NO( a_buffer, a_value )                     \
	as_smpp_serialize_integer( a_buffer, a_value, 4 )

#define AS_SMPP_SERIALIZE_SYSTEM_ID( a_buffer, a_value )                       \
	as_smpp_serialize_string( a_buffer, a_value, AS_SMPP_LENGTH_SYSTEM_ID );

#define AS_SMPP_SERIALIZE_PASSWORD( a_buffer, a_value )                        \
	as_smpp_serialize_string( a_buffer, a_value, 9 );

#define AS_SMPP_SERIALIZE_SYSTEM_TYPE( a_buffer, a_value )                     \
	as_smpp_serialize_string( a_buffer, a_value, 13 );

#define AS_SMPP_SERIALIZE_VERSION( a_buffer, a_value )                         \
	as_smpp_serialize_integer( a_buffer, a_value, 1 )

#define AS_SMPP_SERIALIZE_TON( a_buffer, a_value )                             \
	as_smpp_serialize_integer( a_buffer, a_value, 1 )

#define AS_SMPP_SERIALIZE_NPI( a_buffer, a_value )                             \
	as_smpp_serialize_integer( a_buffer, a_value, 1 )

#define AS_SMPP_SERIALIZE_ADDRESS_RANGE( a_buffer, a_value )                   \
	as_smpp_serialize_string( a_buffer, a_value, 41 );

#define AS_SMPP_SERIALIZE_SERVICE_TYPE( a_buffer, a_value )                    \
	as_smpp_serialize_string( a_buffer, a_value, AS_SMPP_LENGTH_SERVICE_TYPE );


#ifdef __cplusplus
extern "C" {
#endif


void as_smpp_serialize_integer(
	t_as_smpp_buffer * a_buffer, uint32_t a_value, size_t a_len );

uint32_t as_smpp_deserialize_integer(
	t_as_smpp_buffer * a_buffer, size_t * a_index, size_t a_len );

void as_smpp_serialize_string(
	t_as_smpp_buffer * a_buffer, const char * a_value, size_t a_maxLen );

char * as_smpp_deserialize_string( char * a_value,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index,
	size_t a_valueBufferSize );

void as_smpp_serialize_binary(
	t_as_smpp_buffer * a_buffer, const uint8_t * a_value, size_t a_len );

void as_smpp_deserialize_binary( uint8_t * a_value,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index,
	size_t a_valueLen );

t_as_smpp_result as_smpp_serialize_address(
	t_as_smpp_buffer * a_buffer, const t_as_smpp_address * a_address );

t_as_smpp_result as_smpp_deserialize_address( t_as_smpp_address * a_address,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index );

t_as_smpp_result as_smpp_deserialize_datetime( t_as_smpp_datetime * a_datetime,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index );

t_as_smpp_result as_smpp_deserialize_tlv( t_as_smpp_tlv ** a_tlv,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index,
	size_t a_pduLength );

void as_smpp_serialize_pdu_header( t_as_smpp_buffer * a_buffer,
	t_as_smpp_command_length a_commandLength,
	t_as_smpp_command a_commandId,
	t_as_smpp_command_status a_status,
	t_as_smpp_sequence_no a_sequenceNo );

t_as_smpp_result as_smpp_deserialize_pdu_header(
	t_as_smpp_pdu_header * a_header,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index );

void as_smpp_serialize_pdu_submit_sm( t_as_smpp_buffer * a_buffer,
	const char * a_serviceType,
	const t_as_smpp_address * a_sourceAddress,
	const t_as_smpp_address * a_destinationAddress,
	t_as_smpp_data_coding a_dataCoding,
	const t_as_smpp_buffer * a_message );

t_as_smpp_result as_smpp_deserialize_pdu_submit_sm(
	t_as_smpp_pdu_submit_sm * a_pdu,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index );

t_as_smpp_result as_smpp_deserialize_pdu_deliver_sm(
	t_as_smpp_pdu_deliver_sm * a_pdu,
	t_as_smpp_buffer * a_buffer,
	size_t * a_index );


#ifdef __cplusplus
}
#endif


#endif
