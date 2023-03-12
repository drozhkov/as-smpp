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

#include <stdio.h>

#include "../include/private-net.h"


t_as_smpp_result as_smpp_net_init( void )
{
	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

#ifdef AS_SMPP_TARGET_WINDOWS
	WORD versionRequested = MAKEWORD( 2, 2 );
	WSADATA wsaData;

	AS_SMPP_SET_RESULT_CODE( WSAStartup( versionRequested, &wsaData ) != 0
			? AS_SMPP_ERROR_NET_INIT
			: AS_SMPP_ERROR_NONE );
#else
#endif

	AS_SMPP_RETURN_RESULT();
}

void as_smpp_net_fini( void )
{
#ifdef AS_SMPP_TARGET_WINDOWS
	WSACleanup();
#endif
}

t_as_smpp_result as_smpp_net_connect( t_as_smpp_socket_handle * a_socketHandle,
	const char * a_hostname,
	uint16_t a_portNo )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	struct addrinfo hints = { 0 };
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo * addrInfo;
	char serviceName[8];
	snprintf( serviceName, sizeof serviceName, "%u", (unsigned)a_portNo );

	AS_SMPP_SET_RESULT_CODE_AND_JUMP_IF(
		getaddrinfo( a_hostname, serviceName, &hints, &addrInfo ) != 0,
		exit,
		AS_SMPP_ERROR_GETADDRINFO );

	*a_socketHandle = socket(
		addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol );

	AS_SMPP_SET_RESULT_CODE_AND_JUMP_IF(
		!AS_SMPP_NET_IS_VALID_SOCKET( *a_socketHandle ),
		freeaddrinfo,
		AS_SMPP_ERROR_NET );

	AS_SMPP_JUMP_IF( connect( *a_socketHandle,
						 addrInfo->ai_addr,
						 (int)addrInfo->ai_addrlen ) == 0,
		freeaddrinfo );

	AS_SMPP_CLOSE_SOCKET( *a_socketHandle );

	AS_SMPP_SET_RESULT_CODE( AS_SMPP_ERROR_NET );

	AS_SMPP_CATCH( freeaddrinfo );
	freeaddrinfo( addrInfo );

	AS_SMPP_CATCH( exit );

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_net_send(
	t_as_smpp_socket_handle a_socketHandle, const t_as_smpp_buffer * a_buffer )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	AS_SMPP_SET_RESULT_CODE_IF(
		!AS_SMPP_NET_IS_VALID_SOCKET_OPERATION(
			send( a_socketHandle, a_buffer->ptr, a_buffer->dataLen, 0 ) ),
		AS_SMPP_ERROR_NET );

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_net_receive(
	t_as_smpp_socket_handle a_socketHandle, t_as_smpp_buffer * a_buffer )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	int bytesReceived =
		(int)recv( a_socketHandle, a_buffer->ptr, a_buffer->capacity, 0 );

	AS_SMPP_SET_RESULT_CODE_AND_JUMP_IF(
		bytesReceived <= 0, exit, AS_SMPP_ERROR_NET );

	a_buffer->dataLen = bytesReceived;

	AS_SMPP_CATCH( exit );

	AS_SMPP_RETURN_RESULT();
}
