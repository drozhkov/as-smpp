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

#ifndef __AS_SMPP__CORE__H
#define __AS_SMPP__CORE__H


#ifdef _WIN32
#define AS_SMPP_TARGET_WINDOWS
#else
#define AS_SMPP_TARGET_POSIX
#endif


#include <stdint.h>
#include <stddef.h>
#include <stdio.h>


#define AS_SMPP_MALLOC malloc
#define AS_SMPP_REALLOC realloc
#define AS_SMPP_FREE free

#define AS_SMPP_DECLARE_RESULT( a_errorCode )                                  \
	t_as_smpp_result d_result = { 0 };                                         \
	d_result.errorCode = ( a_errorCode );

#define AS_SMPP_SET_RESULT_CODE( a_errorCode )                                 \
	d_result.errorCode = ( a_errorCode )

#define AS_SMPP_SET_RESULT_CODE_IF( a_condition, a_errorCode )                 \
	if ( ( a_condition ) ) {                                                   \
		printf(                                                                \
			"%s: %d, %s %d\n", __FILE__, __LINE__, __func__, a_errorCode );    \
		d_result.errorCode = ( a_errorCode );                                  \
	}

#define AS_SMPP_RETURN_RESULT() return d_result
#define AS_SMPP_SET_RESULT_CODE_AND_JUMP_IF(                                   \
	a_condition, a_location, a_errorCode )                                     \
                                                                               \
	if ( ( a_condition ) ) {                                                   \
		printf(                                                                \
			"%s: %d, %s %d\n", __FILE__, __LINE__, __func__, a_errorCode );    \
		d_result.errorCode = a_errorCode;                                      \
		goto l_##a_location;                                                   \
	}

#define AS_SMPP_JUMP_IF( a_condition, a_location )                             \
	if ( ( a_condition ) ) {                                                   \
		goto l_##a_location;                                                   \
	}

#define AS_SMPP_JUMP( a_location ) goto l_##a_location
#define AS_SMPP_CATCH( a_location ) l_##a_location:


typedef enum as_smpp_error_code_t {
	_AS_SMPP_ERROR_NONE = 0,
	AS_SMPP_ERROR_NONE = _AS_SMPP_ERROR_NONE,
	AS_SMPP_ERROR_SOME = 1,
	AS_SMPP_ERROR_MALLOC,
	AS_SMPP_ERROR_ARG_IS_NULL,
	AS_SMPP_ERROR_NET,
	AS_SMPP_ERROR_GETADDRINFO,
	AS_SMPP_ERROR_NET_INIT,
	AS_SMPP_ERROR_THREADS,
	AS_SMPP_ERROR_MUTEX_INIT,
	AS_SMPP_ERROR_CONDITION_INIT,
	AS_SMPP_ERROR_INCOMPLETE_DATA,
	AS_SMPP_ERROR_DUPLICATE,
	AS_SMPP_ERROR_NOT_FOUND
} t_as_smpp_error_code;

typedef enum {
	AS_SMPP_VARIANT_TYPE_NONE = 0,
	AS_SMPP_VARIANT_TYPE_SCALAR = 1,
	AS_SMPP_VARIANT_TYPE_INT = 2 | AS_SMPP_VARIANT_TYPE_SCALAR,
	AS_SMPP_VARIANT_TYPE_UINT8 = 4 | AS_SMPP_VARIANT_TYPE_SCALAR,
	AS_SMPP_VARIANT_TYPE_UINT16 = 6 | AS_SMPP_VARIANT_TYPE_SCALAR,
	AS_SMPP_VARIANT_TYPE_UINT32 = 8 | AS_SMPP_VARIANT_TYPE_SCALAR,
	AS_SMPP_VARIANT_TYPE_UINT64 = 10 | AS_SMPP_VARIANT_TYPE_SCALAR,
	AS_SMPP_VARIANT_TYPE_PTR = 12
} t_as_smpp_variant_type;

typedef struct {
	void * value;
	void * owner;
	size_t refCount;
} t_as_smpp_ptr;

typedef struct {
	union {
		int i;
		uint8_t u8;
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;
		void * ptr;
	} value;
	t_as_smpp_variant_type type;
	size_t len;
} t_as_smpp_variant;

typedef struct {
	t_as_smpp_error_code errorCode;
	t_as_smpp_variant data;
} t_as_smpp_result;

typedef struct {
	uint8_t * ptr;
	size_t dataLen;
	size_t capacity;
} t_as_smpp_buffer;

typedef struct as_smpp_node_t {
	t_as_smpp_variant key;
	t_as_smpp_variant value;
	struct as_smpp_node_t * up;
	struct as_smpp_node_t * r;
	struct as_smpp_node_t * l;
	uint8_t keyHash;
} t_as_smpp_node;

typedef struct {
	t_as_smpp_node * nodes[256];
} t_as_smpp_map;


#ifdef __cplusplus
extern "C" {
#endif


t_as_smpp_result as_smpp_init( void );
t_as_smpp_result as_smpp_fini( void );

t_as_smpp_result as_smpp_buffer_init(
	t_as_smpp_buffer * a_buffer, size_t a_capacity );

t_as_smpp_result as_smpp_buffer_destroy( t_as_smpp_buffer * a_buffer );
t_as_smpp_result as_smpp_buffer_ensure_capacity(
	t_as_smpp_buffer * a_buffer, size_t a_blockSize );

t_as_smpp_result as_smpp_buffer_add(
	t_as_smpp_buffer * a_buffer, uint8_t a_data );

t_as_smpp_result as_smpp_buffer_copy(
	t_as_smpp_buffer * a_buffer, uint8_t * a_data, size_t a_length );

t_as_smpp_result as_smpp_buffer_remove(
	t_as_smpp_buffer * a_buffer, size_t a_index, size_t a_len );

t_as_smpp_result as_smpp_map_add( t_as_smpp_map * a_map,
	const t_as_smpp_variant * a_key,
	const t_as_smpp_variant * a_value );

t_as_smpp_result as_smpp_map_remove(
	t_as_smpp_map * a_map, const t_as_smpp_variant * a_key );

t_as_smpp_variant * as_smpp_map_get(
	const t_as_smpp_map * a_map, const t_as_smpp_variant * a_key );


#ifdef __cplusplus
}
#endif


#endif
