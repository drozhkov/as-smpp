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
#include <inttypes.h>

#include "as-smpp-core/logger.h"


void as_smpp_log_hex( const void * a_buffer, size_t a_len )
{
	const uint8_t * b = (const uint8_t *)a_buffer;

	for ( size_t i = 0; i < a_len; ++i ) {
		printf( "%02" PRIx8 " ", b[i] );
	}

	putchar( '\n' );
}
