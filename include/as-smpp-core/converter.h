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

#ifndef __AS_SMPP__CONVERTER__H
#define __AS_SMPP__CONVERTER__H


#include <uchar.h>

#include "as-smpp-core/core.h"


#ifdef __cplusplus
extern "C" {
#endif


void as_smpp_converter_init( void );
void as_smpp_converter_fini( void );

char16_t * as_smpp_converter_transcode_unicode_to_ucs2_be( char16_t * a_text );
t_as_smpp_result as_smpp_converter_latin1_to_gsm(
	t_as_smpp_buffer * a_gsm, const char * a_latin1 );

t_as_smpp_result as_smpp_converter_gsm_to_latin1(
	t_as_smpp_buffer * restrict a_latin1,
	const t_as_smpp_buffer * restrict a_gsm );


#ifdef __cplusplus
}
#endif


#endif
