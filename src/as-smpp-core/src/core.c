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
#include <string.h>
#include <stdbool.h>

#include "as-smpp-core/core.h"
#include "as-smpp-core/converter.h"

#include "../include/private-net.h"


static t_as_smpp_result as_smpp_variant_copy(
	t_as_smpp_variant * a_to, const t_as_smpp_variant * a_from )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	void * valuePtr = a_to->value.ptr;
	*a_to = *a_from;

	if ( AS_SMPP_VARIANT_TYPE_PTR == a_from->type ) {
		if ( NULL == valuePtr ) {
			a_to->value.ptr = AS_SMPP_MALLOC( a_to->len );
		}

		memcpy( a_to->value.ptr, a_from->value.ptr, a_from->len );
	}

	AS_SMPP_RETURN_RESULT();
}

static uint8_t as_smpp_variant_calc_hash( const t_as_smpp_variant * a_v )
{
	uint8_t result = 0;
	uint8_t * p;

	if ( (a_v->type & AS_SMPP_VARIANT_TYPE_SCALAR) != 0 ) {
		p = (uint8_t *)( &a_v->value );
	}
	else {
		p = a_v->value.ptr;
	}

	for ( size_t i = 0; i < a_v->len; ++i ) {
		result ^= p[i];
	}

	return result;
}

static int as_smpp_variant_compare(
	const t_as_smpp_variant * a_l, const t_as_smpp_variant * a_r )
{

	if ( a_l->len != a_r->len ) {
		return ( a_l->len - a_r->len );
	}

	const void * lp =
		AS_SMPP_VARIANT_TYPE_PTR == a_l->type ? a_l->value.ptr : &a_l->value;

	const void * rp =
		AS_SMPP_VARIANT_TYPE_PTR == a_r->type ? a_r->value.ptr : &a_r->value;

	return memcmp( lp, rp, a_l->len );
}

static t_as_smpp_result as_smpp_map_create_node( t_as_smpp_node ** a_node,
	const t_as_smpp_variant * a_key,
	const t_as_smpp_variant * a_value,
	uint8_t a_keyHash )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	*a_node =
		AS_SMPP_MALLOC( sizeof( t_as_smpp_node ) + a_key->len + a_value->len );

	( *a_node )->l = NULL;
	( *a_node )->r = NULL;
	( *a_node )->up = NULL;
	( *a_node )->keyHash = a_keyHash;

	uint8_t * p = (uint8_t *)( ( *a_node ) + 1 );

	if ( AS_SMPP_VARIANT_TYPE_PTR == a_key->type ) {
		( *a_node )->key.value.ptr = p;
		p += a_key->len;
	}

	if ( AS_SMPP_VARIANT_TYPE_PTR == a_value->type ) {
		( *a_node )->value.value.ptr = p;
	}

	as_smpp_variant_copy( &( *a_node )->key, a_key );
	as_smpp_variant_copy( &( *a_node )->value, a_value );

	AS_SMPP_RETURN_RESULT();
}

static void as_smpp_map_destroy_node( t_as_smpp_node * a_node )
{
	AS_SMPP_FREE( a_node );
}

static t_as_smpp_node * as_smpp_map_get_node(
	const t_as_smpp_map * a_map, const t_as_smpp_variant * a_key )
{

	uint8_t keyHash = as_smpp_variant_calc_hash( a_key );
	t_as_smpp_node * node = a_map->nodes[keyHash];

	while ( node != NULL ) {
		int cmp = as_smpp_variant_compare( &node->key, a_key );

		if ( 0 == cmp ) {
			return node;
		}

		node = cmp > 0 ? node->r : node->l;
	}

	return NULL;
}

t_as_smpp_result as_smpp_init( void )
{
	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_net_init();
	as_smpp_converter_init();

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_fini( void )
{
	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_converter_fini();
	as_smpp_net_fini();

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_buffer_init(
	t_as_smpp_buffer * a_buffer, size_t a_capacity )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	a_buffer->ptr = AS_SMPP_MALLOC( a_capacity );
	a_buffer->capacity = a_capacity;
	a_buffer->dataLen = 0;

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_buffer_destroy( t_as_smpp_buffer * a_buffer )
{
	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	free( a_buffer->ptr );

	a_buffer->ptr = NULL;
	a_buffer->capacity = 0;
	a_buffer->dataLen = 0;

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_buffer_ensure_capacity(
	t_as_smpp_buffer * a_buffer, size_t a_blockSize )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	if ( a_buffer->dataLen + a_blockSize > a_buffer->capacity ) {
		a_buffer->capacity += a_blockSize + 16;
		a_buffer->ptr = AS_SMPP_REALLOC( a_buffer->ptr, a_buffer->capacity );
	}

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_buffer_add(
	t_as_smpp_buffer * a_buffer, uint8_t a_data )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_buffer_ensure_capacity( a_buffer, 1 );
	a_buffer->ptr[a_buffer->dataLen++] = a_data;

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_buffer_copy(
	t_as_smpp_buffer * a_buffer, uint8_t * a_data, size_t a_length )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	as_smpp_buffer_ensure_capacity( a_buffer, a_length );
	memcpy( a_buffer->ptr + a_buffer->dataLen, a_data, a_length );
	a_buffer->dataLen += a_length;

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_buffer_remove(
	t_as_smpp_buffer * a_buffer, size_t a_index, size_t a_len )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	if ( a_buffer->dataLen <= a_len ) {
		a_buffer->dataLen = a_index;
	}
	else {
		memmove( a_buffer->ptr + a_index,
			a_buffer->ptr + a_len,
			a_buffer->dataLen - a_len );

		a_buffer->dataLen -= a_len - a_index;
	}

	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_map_add( t_as_smpp_map * a_map,
	const t_as_smpp_variant * a_key,
	const t_as_smpp_variant * a_value )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	uint8_t keyHash = as_smpp_variant_calc_hash( a_key );
	t_as_smpp_node * node = a_map->nodes[keyHash];

	if ( NULL == node ) {
		t_as_smpp_node * node;
		as_smpp_map_create_node( &node, a_key, a_value, keyHash );

		a_map->nodes[keyHash] = node;

		AS_SMPP_JUMP( exit );
	}

	while ( true ) {
		int cmp = as_smpp_variant_compare( &node->key, a_key );

		AS_SMPP_SET_RESULT_CODE_AND_JUMP_IF(
			0 == cmp, exit, AS_SMPP_ERROR_DUPLICATE );

		t_as_smpp_node ** nextNode = cmp > 0 ? &node->r : &node->l;

		if ( NULL == *nextNode ) {
			as_smpp_map_create_node( nextNode, a_key, a_value, keyHash );
			( *nextNode )->up = node;
			break;
		}

		node = *nextNode;
	}

	AS_SMPP_CATCH( exit );
	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_result as_smpp_map_remove(
	t_as_smpp_map * a_map, const t_as_smpp_variant * a_key )
{

	AS_SMPP_DECLARE_RESULT( AS_SMPP_ERROR_NONE );

	t_as_smpp_node * node = as_smpp_map_get_node( a_map, a_key );

	AS_SMPP_SET_RESULT_CODE_AND_JUMP_IF(
		NULL == node, exit, AS_SMPP_ERROR_NOT_FOUND );

	t_as_smpp_node ** linkToThis = NULL;
	t_as_smpp_node * replaceWith = NULL;

	if ( NULL != node->up ) {
		linkToThis = node == node->up->r ? &node->up->r : &node->up->l;
	}
	else {
		linkToThis = &a_map->nodes[node->keyHash];
	}

	if ( NULL != node->r && NULL != node->l ) {
		t_as_smpp_node * n = (node->keyHash & 0x01) != 0 ? node->l : node->r;

		while ( NULL == replaceWith ) {
			if ( (node->keyHash & 0x01) != 0 ) {
				if ( NULL == n->r ) {
					n->r = node->r;
					node->r->up = n;
					replaceWith = node->l;
				}

				n = n->r;
			}
			else {
				if ( NULL == n->l ) {
					n->l = node->l;
					node->l->up = n;
					replaceWith = node->r;
				}

				n = n->l;
			}
		}
	}
	else {
		replaceWith = NULL == node->r ? node->l : node->r;
	}

	*linkToThis = replaceWith;

	if ( replaceWith != NULL ) {
		replaceWith->up = node->up;
	}

	AS_SMPP_CATCH( destroyNode )
	as_smpp_map_destroy_node( node );

	AS_SMPP_CATCH( exit )
	AS_SMPP_RETURN_RESULT();
}

t_as_smpp_variant * as_smpp_map_get(
	const t_as_smpp_map * a_map, const t_as_smpp_variant * a_key )
{

	t_as_smpp_node * node = as_smpp_map_get_node( a_map, a_key );

	return ( node != NULL ? &node->value : NULL );
}
