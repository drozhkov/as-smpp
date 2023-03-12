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

#ifndef __AS_SMPP__SMPP_TYPES__H
#define __AS_SMPP__SMPP_TYPES__H


#include <stdbool.h>
#include <stdint.h>


#define AS_SMPP_LENGTH_COMMAND_LENGTH 4
#define AS_SMPP_LENGTH_COMMAND_ID 4
#define AS_SMPP_LENGTH_COMMAND_STATUS 4
#define AS_SMPP_LENGTH_SEQUENCE_NO 4
#define AS_SMPP_LENGTH_SYSTEM_ID 16
#define AS_SMPP_LENGTH_ADDRESS 21
#define AS_SMPP_LENGTH_MESSAGE 255
#define AS_SMPP_LENGTH_SERVICE_TYPE 6
#define AS_SMPP_LENGTH_TON 1
#define AS_SMPP_LENGTH_NPI 1
#define AS_SMPP_LENGTH_ESM_CLASS 1
#define AS_SMPP_LENGTH_PROTOCOL_ID 1
#define AS_SMPP_LENGTH_PRIORITY_FLAG 1
#define AS_SMPP_LENGTH_REPLACE_IF_PRESENT_FLAG 1
#define AS_SMPP_LENGTH_DATETIME 17
#define AS_SMPP_LENGTH_REGISTERED_DELIVERY 1
#define AS_SMPP_LENGTH_DATA_CODING 1
#define AS_SMPP_LENGTH_SM_DEFAULT_MESSAGE_ID 1
#define AS_SMPP_LENGTH_SM_LENGTH 1
#define AS_SMPP_LENGTH_MESSAGE_ID 65


typedef enum {
	AS_SMPP_SESSION_MODE_TRANSMITTER,
	AS_SMPP_SESSION_MODE_RECEIVER,
	AS_SMPP_SESSION_MODE_TRANSCEIVER
} t_as_smpp_session_mode;

typedef enum {
	AS_SMPP_COMMAND_BIND_RECEIVER = 0x00000001,
	AS_SMPP_COMMAND_BIND_TRANSMITTER = 0x00000002,
	AS_SMPP_COMMAND_BIND_TRANSCEIVER = 0x00000009,
	AS_SMPP_COMMAND_SUBMIT_SM = 0x00000004,
	AS_SMPP_COMMAND_DELIVER_SM = 0x00000005,
	AS_SMPP_COMMAND_ENQUIRE_LINK = 0x00000015,
	AS_SMPP_COMMAND_NACK = 0x80000000,
	AS_SMPP_COMMAND_RESPONSE_FLAG = 0x80000000
} t_as_smpp_command;

typedef enum {
	AS_SMPP_COMMAND_STATUS_ROK = 0x00000000
} t_as_smpp_command_status;

typedef enum {
	AS_SMPP_VERSION_3_4 = 0x34,
	AS_SMPP_VERSION_5_0 = 0x50
} t_as_smpp_version;

typedef enum {
	AS_SMPP_TON_UNKNOWN = 0,
	AS_SMPP_TON_INTERNATIONAL = 1,
	AS_SMPP_TON_NATIONAL = 2,
	AS_SMPP_TON_NETWORK_SPECIFIC = 3,
	AS_SMPP_TON_NETWORK_SUBSCRIBER_NUMBER = 4,
	AS_SMPP_TON_NETWORK_ALPHANUMERIC = 5
} t_as_smpp_ton;

typedef enum {
	AS_SMPP_NPI_UNKNOWN = 0,
	AS_SMPP_NPI_ISDN = 1,
	AS_SMPP_NPI_DATA = 3,
	AS_SMPP_NPI_TELEX = 4
} t_as_smpp_npi;

typedef enum {
	AS_SMPP_ESM_CLASS_MODE_MASK = 0x02 | 0x01,
	AS_SMPP_ESM_CLASS_MODE_DEFAULT = 0,
	AS_SMPP_ESM_CLASS_MODE_DATAGRAM = 1,
	AS_SMPP_ESM_CLASS_MODE_FORWARD = 2,
	AS_SMPP_ESM_CLASS_MODE_STORE_FORWARD = 1 | 2,
	AS_SMPP_ESM_CLASS_TYPE_MASK = 0x20 | 0x10 | 0x08 | 0x04,
	AS_SMPP_ESM_CLASS_TYPE_DEFAULT = 0,
	AS_SMPP_ESM_CLASS_TYPE_DELIVERY_RECEIPT = 0x04,
	AS_SMPP_ESM_CLASS_TYPE_DELIVERY_ACK = 0x08,
	AS_SMPP_ESM_CLASS_TYPE_USER_ACK = 0x10,
	AS_SMPP_ESM_CLASS_TYPE_INTERMEDIATE_DELIVERY_NOTIFICATION = 0x20,
	AS_SMPP_ESM_CLASS_GSM_FEATURE_MASK = 0x80 | 0x40,
	AS_SMPP_ESM_CLASS_GSM_FEATURE_NONE = 0,
	AS_SMPP_ESM_CLASS_GSM_FEATURE_UDH = 0x40,
	AS_SMPP_ESM_CLASS_GSM_FEATURE_REPLY_PATH = 0x80,
} t_as_smpp_esm_class;

typedef enum {
	AS_SMPP_REGISTERED_DELIVERY_RECEIPT_MASK = 0x02 | 0x01,
	AS_SMPP_REGISTERED_DELIVERY_RECEIPT_NONE = 0,
	AS_SMPP_REGISTERED_DELIVERY_RECEIPT_SUCCESS_OR_FAILURE = 1,
	AS_SMPP_REGISTERED_DELIVERY_RECEIPT_FAILURE = 2,
	AS_SMPP_REGISTERED_DELIVERY_RECEIPT_SUCCESS = 3,
	AS_SMPP_REGISTERED_DELIVERY_SME_ACK_MASK = 0x08 | 0x04,
	AS_SMPP_REGISTERED_DELIVERY_SME_ACK_NONE = 0,
	AS_SMPP_REGISTERED_DELIVERY_SME_ACK_DELIVERY = 0x04,
	AS_SMPP_REGISTERED_DELIVERY_SME_ACK_USER = 0x08,
	AS_SMPP_REGISTERED_DELIVERY_INTERMEDIATE_NOTIFICATION_MASK = 0x10,
	AS_SMPP_REGISTERED_DELIVERY_INTERMEDIATE_NOTIFICATION_NO = 0,
	AS_SMPP_REGISTERED_DELIVERY_INTERMEDIATE_NOTIFICATION_YES = 0x10
} t_as_smpp_registered_delivery;

typedef enum {
	AS_SMPP_DATA_CODING_DEFAULT = 0,
	AS_SMPP_DATA_CODING_ASCII = 1,
	AS_SMPP_DATA_CODING_BINARY2 = 2,
	AS_SMPP_DATA_CODING_LATIN1 = 3,
	AS_SMPP_DATA_CODING_BINARY4 = 4,
	AS_SMPP_DATA_CODING_JIS = 5,
	AS_SMPP_DATA_CODING_CYRILLIC = 6,
	AS_SMPP_DATA_CODING_LATIN_HEBREW = 7,
	AS_SMPP_DATA_CODING_UCS2 = 8,
	AS_SMPP_DATA_CODING_PICTOGRAM = 9,
	AS_SMPP_DATA_CODING_ISO2022 = 10,
	AS_SMPP_DATA_CODING_EXTENDED_KANJI = 13,
	AS_SMPP_DATA_CODING_KS_C_5601 = 14,
	AS_SMPP_DATA_CODING_MASK_GSM = 0x80 | 0x40 | 0x20 | 0x10,
} t_as_smpp_data_coding;

typedef uint32_t t_as_smpp_command_length;
typedef uint32_t t_as_smpp_sequence_no;
typedef uint8_t t_as_smpp_protocol_id;
typedef uint8_t t_as_smpp_priority;
typedef uint8_t t_as_smpp_sm_default_message_id;
typedef uint8_t t_as_smpp_sm_length;

typedef struct as_smpp_tlv_t {
	uint16_t tag;
	uint16_t length;
	union {
		uint8_t * ptr;
		uint32_t n;
	} data;
	struct as_smpp_tlv_t * next;
} t_as_smpp_tlv;

typedef struct {
	t_as_smpp_ton ton;
	t_as_smpp_npi npi;
	char * address;
	char addressBuffer[AS_SMPP_LENGTH_ADDRESS];
} t_as_smpp_address;

typedef struct {
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint16_t decisecond;
	int16_t offsetMinutes;
	_Bool isNull;
} t_as_smpp_datetime;

typedef struct {
	t_as_smpp_command_length commandLength;
	t_as_smpp_command commandId;
	t_as_smpp_command_status status;
	t_as_smpp_sequence_no sequenceNo;
} t_as_smpp_pdu_header;

typedef struct {
	char * systemId;
	char systemIdBuffer[AS_SMPP_LENGTH_SYSTEM_ID];
} t_as_smpp_pdu_bind_response;

typedef struct {
	char * serviceType;
	char serviceTypeBuffer[AS_SMPP_LENGTH_SERVICE_TYPE];
	t_as_smpp_address sourceAddress;
	t_as_smpp_address destinationAddress;
	t_as_smpp_esm_class esmClass;
	t_as_smpp_protocol_id protocolId;
	t_as_smpp_priority priority;
	t_as_smpp_datetime scheduleDeliveryTime;
	t_as_smpp_datetime validityPeriod;
	t_as_smpp_registered_delivery registeredDelivery;
	_Bool replaceIfPresent;
	t_as_smpp_data_coding dataCoding;
	t_as_smpp_sm_default_message_id defaultMessageId;
	t_as_smpp_sm_length messageLength;
	uint8_t * message;
	uint8_t messageBuffer[AS_SMPP_LENGTH_MESSAGE + 1];
} t_as_smpp_pdu_submit_sm;

typedef t_as_smpp_pdu_submit_sm t_as_smpp_pdu_deliver_sm;

typedef struct {
	char * messageId;
	char messageIdBuffer[AS_SMPP_LENGTH_MESSAGE_ID];
} t_as_smpp_pdu_submit_sm_response;

typedef void ( *t_as_smpp_handler_bind_response )(
	t_as_smpp_sequence_no a_sequenceNo, t_as_smpp_command_status a_status );

typedef void ( *t_as_smpp_handler_submit_sm_response )(
	t_as_smpp_sequence_no a_sequenceNo,
	t_as_smpp_command_status a_status,
	const char * a_messageId );

typedef void ( *t_as_smpp_handler_deliver_sm )(
	t_as_smpp_sequence_no a_sequenceNo,
	t_as_smpp_command_status a_status,
	const t_as_smpp_pdu_deliver_sm * a_pdu );


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif


#endif
