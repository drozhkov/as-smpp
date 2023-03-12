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

#ifndef __AS_SMPP__PRIVATE_NET__H
#define __AS_SMPP__PRIVATE_NET__H


#include "as-smpp-core/core.h"


#ifdef AS_SMPP_TARGET_WINDOWS
#include "WinSock2.h"
#include "WS2tcpip.h"

#define AS_SMPP_NET_IS_VALID_SOCKET( a_socket ) ( a_socket != INVALID_SOCKET )
#define AS_SMPP_NET_IS_VALID_SOCKET_OPERATION( a_result )                      \
	( a_result != SOCKET_ERROR )

#define AS_SMPP_CLOSE_SOCKET closesocket

typedef SOCKET t_as_smpp_socket_handle;
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#define AS_SMPP_NET_IS_VALID_SOCKET( a_socket ) ( a_socket >= 0 )
#define AS_SMPP_NET_IS_VALID_SOCKET_OPERATION( a_result ) ( a_result >= 0 )
#define AS_SMPP_CLOSE_SOCKET close

typedef int t_as_smpp_socket_handle;
#endif


#ifdef __cplusplus
extern "C" {
#endif


/// @brief
/// @param
/// @return
t_as_smpp_result as_smpp_net_init( void );

/// @brief
/// @param
void as_smpp_net_fini( void );

/// @brief
/// @param a_socketHandle
/// @param a_hostname
/// @param a_portNo
/// @return
t_as_smpp_result as_smpp_net_connect( t_as_smpp_socket_handle * a_socketHandle,
	const char * a_hostname,
	uint16_t a_portNo );

t_as_smpp_result as_smpp_net_send(
	t_as_smpp_socket_handle a_socketHandle, const t_as_smpp_buffer * a_buffer );

t_as_smpp_result as_smpp_net_receive(
	t_as_smpp_socket_handle a_socketHandle, t_as_smpp_buffer * a_buffer );


#ifdef __cplusplus
}
#endif


#endif
