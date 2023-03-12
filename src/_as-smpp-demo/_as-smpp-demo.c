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
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include "as-smpp-core/logger.h"
#include "as-smpp-core/converter.h"
#include "as-smpp-esme/esme.h"


void handleBindResponse(
	t_as_smpp_sequence_no a_sequenceNo, t_as_smpp_command_status a_status )
{

	printf( "bind response: sequence no = %" PRIu32 ", status = %u\n",
		a_sequenceNo,
		a_status );
}

void handleSubmitResponse( t_as_smpp_sequence_no a_sequenceNo,
	t_as_smpp_command_status a_status,
	const char * a_messageId )
{

	printf( "submit sm response: sequence no = %" PRIu32
			", status = %u, message id = %s\n",
		a_sequenceNo,
		a_status,
		a_messageId );
}

void handleDeliver( t_as_smpp_sequence_no a_sequenceNo,
	t_as_smpp_command_status a_status,
	const t_as_smpp_pdu_deliver_sm * a_pdu )
{

	printf( "deliver sm: sequence no = %" PRIu32 ", status = %u\n",
		a_sequenceNo,
		a_status );

	printf( "deliver sm: message = " );
	as_smpp_log_hex( a_pdu->message, a_pdu->messageLength );

	t_as_smpp_buffer text;
	as_smpp_buffer_init( &text, a_pdu->messageLength + 1 );
	as_smpp_converter_gsm_to_latin1( &text,
		&( t_as_smpp_buffer ){
			.ptr = a_pdu->message, .dataLen = a_pdu->messageLength } );

	printf( "deliver sm: message = %s\n", text.ptr );
	as_smpp_buffer_destroy( &text );
}

int main( void )
{
	as_smpp_init();

	t_as_smpp_session_handle session = as_smpp_session_create();
	as_smpp_set_handler_bind_response( session, handleBindResponse );
	as_smpp_set_handler_submit_sm_response( session, handleSubmitResponse );
	as_smpp_set_handler_deliver_sm( session, handleDeliver );

	AS_SMPP_JUMP_IF( as_smpp_esme_start( session,
						 "192.168.0.213",
						 2775,
						 AS_SMPP_SESSION_MODE_TRANSCEIVER,
						 "foo",
						 "bar",
						 "as-smpp",
						 AS_SMPP_VERSION_3_4,
						 AS_SMPP_TON_UNKNOWN,
						 AS_SMPP_NPI_UNKNOWN,
						 NULL,
						 false,
						 true,
						 140 )
						 .errorCode != _AS_SMPP_ERROR_NONE,
		error );

	puts( "binding..." );
	as_smpp_session_wait_for_state( session, AS_SMPP_SESSION_STATE_BOUND_TRX );
	puts( "AS_SMPP_SESSION_STATE_BOUND_TRX" );

	char16_t textS[] = u"test!@#$%^&*()1234567890";
	char16_t * text = as_smpp_converter_transcode_unicode_to_ucs2_be( textS );
	size_t textLen = sizeof textS - 2;

	t_as_smpp_sequence_no sequenceNo;
	as_smpp_submit_sm( session,
		&sequenceNo,
		"SMPP",
		&( t_as_smpp_address ){ .address = "79163263253",
			.npi = AS_SMPP_NPI_UNKNOWN,
			.ton = AS_SMPP_TON_INTERNATIONAL },
		&( t_as_smpp_address ){ .address = "79163263253",
			.npi = AS_SMPP_NPI_UNKNOWN,
			.ton = AS_SMPP_TON_INTERNATIONAL },
		AS_SMPP_DATA_CODING_UCS2,
		// AS_SMPP_DATA_CODING_DEFAULT,
		&( t_as_smpp_buffer ){ .ptr = (uint8_t *)text, .dataLen = textLen } );

	getchar();

	AS_SMPP_JUMP( exit );

	AS_SMPP_CATCH( error );
	puts( "error" );

	AS_SMPP_CATCH( exit );
	as_smpp_session_destroy( session );

	as_smpp_fini();

	return 0;
}
