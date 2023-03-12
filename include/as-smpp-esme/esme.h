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

#ifndef __AS_SMPP__ESME__H
#define __AS_SMPP__ESME__H


#include "as-smpp-core/session.h"
#include "as-smpp-core/smpp.h"


#ifdef __cplusplus
extern "C" {
#endif


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
	const char * a_addressRang0e,
	_Bool a_useUdh,
	_Bool a_useMessagePayloadTlv,
	size_t a_maxSmLength );


#ifdef __cplusplus
}
#endif


#endif
