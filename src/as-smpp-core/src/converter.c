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

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "as-smpp-core/converter.h"


static uint8_t as_smpp_CharMapGsmToLatin1[256];
static uint8_t as_smpp_CharMapGsmXToLatin1[256];
static uint16_t as_smpp_CharMapLatin1ToGsm[256];


static _Bool as_smpp_is_big_endian( void )
{
	union {
		uint16_t u;
		uint8_t b[2];
	} t;

	t.u = 0xabcd;

	return ( t.b[0] == 0xab );
}

void as_smpp_converter_init( void )
{
	memset(
		as_smpp_CharMapGsmXToLatin1, 0xff, sizeof as_smpp_CharMapGsmXToLatin1 );

	for ( size_t i = 0; i < ( sizeof as_smpp_CharMapLatin1ToGsm /
								sizeof as_smpp_CharMapLatin1ToGsm[0] );
		  ++i ) {

		as_smpp_CharMapLatin1ToGsm[i] = i;
	}

	as_smpp_CharMapLatin1ToGsm[0x5f] = 0x11; // Underscore
	as_smpp_CharMapLatin1ToGsm[0x40] = 0x00; // At sign
	as_smpp_CharMapLatin1ToGsm[0xA3] = 0x01; // Pound sign
	as_smpp_CharMapLatin1ToGsm[0x24] = 0x02; // Dollar sign
	as_smpp_CharMapLatin1ToGsm[0xa5] = 0x03; // Yuan/Yen sign
	as_smpp_CharMapLatin1ToGsm[0xe8] = 0x04; // Small letter e with grave accent
	as_smpp_CharMapLatin1ToGsm[0xe9] = 0x05; // Small letter e with acute accent
	as_smpp_CharMapLatin1ToGsm[0xf9] = 0x06; // Small letter u with grave accent
	as_smpp_CharMapLatin1ToGsm[0xec] = 0x07; // Small letter i with grave accent
	as_smpp_CharMapLatin1ToGsm[0xf2] = 0x08; // Small letter o with grave accent
	as_smpp_CharMapLatin1ToGsm[0xc7] = 0x09; // Capital letter C with cedilla
	as_smpp_CharMapLatin1ToGsm[0xd8] = 0x0b; // Capital letter O with stroke
	as_smpp_CharMapLatin1ToGsm[0xf8] = 0x0c; // Small letter o with stroke
	as_smpp_CharMapLatin1ToGsm[0xc5] = 0x0e; // Capital letter A with ring
	as_smpp_CharMapLatin1ToGsm[0xe5] = 0x0f; // Small letter a with ring
	as_smpp_CharMapLatin1ToGsm[0x0c] = 0x1B0A; // Form feed
	as_smpp_CharMapLatin1ToGsm[0x5e] = 0x1b14; // Caret
	as_smpp_CharMapLatin1ToGsm[0x7b] = 0x1b28; // Left curly bracket
	as_smpp_CharMapLatin1ToGsm[0x7d] = 0x1b29; // Right curly bracket
	as_smpp_CharMapLatin1ToGsm[0x5c] = 0x1B2F; // Backslash
	as_smpp_CharMapLatin1ToGsm[0x5b] = 0x1b3c; // Left square bracket
	as_smpp_CharMapLatin1ToGsm[0x7e] = 0x1b3d; // Tilde
	as_smpp_CharMapLatin1ToGsm[0x5d] = 0x1b3e; // Right square bracket
	as_smpp_CharMapLatin1ToGsm[0x7c] = 0x1b40; // Vertical bar
	as_smpp_CharMapLatin1ToGsm[0xa4] = 0x24;   // Currency sign
	as_smpp_CharMapLatin1ToGsm[0xa1] = 0x40;   // Inverted exclamation mark

	for ( size_t i = 0; i < ( sizeof as_smpp_CharMapLatin1ToGsm /
								sizeof as_smpp_CharMapLatin1ToGsm[0] );
		  ++i ) {

		uint16_t ch = as_smpp_CharMapLatin1ToGsm[i];

		if ( ch < 0x100 ) {
			as_smpp_CharMapGsmToLatin1[ch] = i;
		}
		else {
			as_smpp_CharMapGsmToLatin1[ch >> 8] = 0xff;
			as_smpp_CharMapGsmXToLatin1[ch & 0xff] = i;
		}
	}
}

void as_smpp_converter_fini( void )
{
}

char16_t * as_smpp_converter_transcode_unicode_to_ucs2_be( char16_t * a_text )
{
	if ( as_smpp_is_big_endian() ) {
		return a_text;
	}

	char16_t * t = a_text;

	while ( *t != L'\0' ) {
		uint8_t * p = (uint8_t *)t;
		*t = ( *p << 8 ) | *( p + 1 );
		++t;
	}

	return a_text;
}

t_as_smpp_result as_smpp_converter_latin1_to_gsm(
	t_as_smpp_buffer * a_gsm, const char * a_latin1 )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	size_t index = 0;
	size_t latin1Len = strlen( a_latin1 );
	as_smpp_buffer_ensure_capacity( a_gsm, latin1Len + 1 );

	while ( *a_latin1 != '\0' ) {
		uint16_t ch = as_smpp_CharMapLatin1ToGsm[*a_latin1];

		if ( ch > 0xff ) {
			as_smpp_buffer_add( a_gsm, ch >> 8 );
		}

		as_smpp_buffer_add( a_gsm, ch & 0xff );
	}

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_converter_gsm_to_latin1(
	t_as_smpp_buffer * restrict a_latin1,
	const t_as_smpp_buffer * restrict a_gsm )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_buffer_ensure_capacity( a_latin1, a_gsm->dataLen + 1 );

	for ( size_t i = 0; i < a_gsm->dataLen; ++i ) {
		uint8_t ch = as_smpp_CharMapGsmToLatin1[a_gsm->ptr[i]];

		if ( ch == 0xff && i < a_gsm->dataLen - 1 ) {
			uint8_t ch1 = as_smpp_CharMapGsmXToLatin1[a_gsm->ptr[i + 1]];

			if ( ch1 != 0xff ) {
				ch = ch1;
				++i;
			}
			else {
				ch = a_gsm->ptr[i];
			}
		}

		as_smpp_buffer_add( a_latin1, ch );
	}

	as_smpp_buffer_add( a_latin1, '\0' );

	AS_SMPP_RETURN_RESULT();
}
