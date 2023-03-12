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

#include "../../as-smpp-core/include/private-session.h"

#include "as-smpp-esme/esme.h"


t_as_smpp_result as_smpp_esme_start( t_as_smpp_session_handle a_session,
	const char * a_hostname,
	uint16_t a_portNo,
	t_as_smpp_session_mode a_sessionMode,
	const char * a_systemId,
	const char * a_password,
	const char * a_systemType,
	t_as_smpp_version a_version,
	t_as_smpp_ton a_addressTon,
	t_as_smpp_npi a_addressNpi,
	const char * a_addressRange,
	_Bool a_useUdh,
	_Bool a_useMessagePayloadTlv,
	size_t a_maxSmLength )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	AS_SMPP_SET_RESULT_CODE_AND_JUMP_IF(
		a_session == NULL, exit, AS_SMPP_ERROR_ARG_IS_NULL );

	a_session->sequenceNo = UINT32_C( 1 );
	a_session->mode = a_sessionMode;
	a_session->smppVersion = a_version;
	a_session->useUdh = a_useUdh;
	a_session->useMessagePayloadTlv = a_useMessagePayloadTlv;
	a_session->maxSmLength = a_maxSmLength;

	AS_SMPP_SET_RESULT_CODE_AND_JUMP_IF(
		as_smpp_connect( a_session, a_hostname, a_portNo ).errorCode !=
			AS_SMPP_ERROR_NONE,
		exit,
		AS_SMPP_ERROR_NET );

	as_smpp_bind( a_session,
		a_systemId,
		a_password,
		a_systemType,
		a_addressTon,
		a_addressNpi,
		a_addressRange );

	AS_SMPP_CATCH( exit );

	AS_SMPP_RETURN_RESULT();
}
