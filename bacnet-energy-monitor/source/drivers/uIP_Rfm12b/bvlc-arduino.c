/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "bvlc-arduino.h"
#include "bip.h"
#include "bacint.h"
#include "uip.h"
#include "verbose.h"

/** result from a client request */
BACNET_BVLC_RESULT BVLC_Result_Code = BVLC_RESULT_SUCCESSFUL_COMPLETION;
/** The current BVLC Function Code being handled. */
BACNET_BVLC_FUNCTION BVLC_Function_Code = BVLC_RESULT;  /* A safe default */

static int bvlc_encode_bvlc_result(
    uint8_t * pdu,
    BACNET_BVLC_RESULT result_code)
{
    if (pdu) {
        pdu[0] = BVLL_TYPE_BACNET_IP;
        pdu[1] = BVLC_RESULT;
        /* The 2-octet BVLC Length field is the length, in octets,
           of the entire BVLL message, including the two octets of the
           length field itself, most significant octet first. */
        encode_unsigned16(&pdu[2], 6);
        encode_unsigned16(&pdu[4], (uint16_t) result_code);
    }

    return 6;
}

static int bvlc_send_mpdu(
    uip_ipaddr_t *dest_addr,   /* the destination address */
    uint16_t *dest_port,  /* the destination port */
    uint8_t * mtu,      /* the data */
    uint16_t mtu_len)
{       /* amount of data to send  */
    /* assumes that the driver has already been initialized */
    if (bip_valid()) {
        return 0;
    }

    uint8_t* dataToSend = (uint8_t*)uip_appdata;
    memset(dataToSend, 0, mtu_len); /* Clean buffer */
    memcpy(dataToSend, mtu, mtu_len);
    /* Send the packet */
    uip_udp_send(mtu_len);
    return mtu_len;

}

static void bvlc_send_result(
    uip_ipaddr_t * dest_addr, uint16_t * dest_port,   /* the destination address */
    BACNET_BVLC_RESULT result_code)
{
    uint8_t mtu[MAX_MPDU] = { 0 };
    uint16_t mtu_len = 0;

    mtu_len = (uint16_t) bvlc_encode_bvlc_result(&mtu[0], result_code);
    bvlc_send_mpdu(dest_addr, dest_port, mtu, mtu_len);

    return;
}

uint16_t bvlc_for_non_bbmd(uip_ipaddr_t * addr, uint16_t * port,
		uint8_t * npdu, uint16_t received_bytes){

	uint16_t result_code = 0; /* aka, BVLC_RESULT_SUCCESSFUL_COMPLETION */
	BVLC_Function_Code = npdu[1];       /* The BVLC function */
	switch(BVLC_Function_Code){
		case BVLC_RESULT:
			if (received_bytes >= 6) {
				/* This is the result of our foreign device registration */
				(void) decode_unsigned16(&npdu[4], &result_code);
				BVLC_Result_Code = (BACNET_BVLC_RESULT) result_code;
				VERBOSE_STRING("BVLC: Result Code=\n");
				/* But don't send any response */
				result_code = 0;
			}
			break;
		case BVLC_WRITE_BROADCAST_DISTRIBUTION_TABLE:
			result_code = BVLC_RESULT_WRITE_BROADCAST_DISTRIBUTION_TABLE_NAK;
			break;
		case BVLC_READ_BROADCAST_DIST_TABLE:
			result_code = BVLC_RESULT_READ_BROADCAST_DISTRIBUTION_TABLE_NAK;
			break;
			/* case BVLC_READ_BROADCAST_DIST_TABLE_ACK: */
		case BVLC_REGISTER_FOREIGN_DEVICE:
			result_code = BVLC_RESULT_REGISTER_FOREIGN_DEVICE_NAK;
			break;
		case BVLC_READ_FOREIGN_DEVICE_TABLE:
			result_code = BVLC_RESULT_READ_FOREIGN_DEVICE_TABLE_NAK;
			break;
			/* case BVLC_READ_FOREIGN_DEVICE_TABLE_ACK: */
		case BVLC_DELETE_FOREIGN_DEVICE_TABLE_ENTRY:
			result_code = BVLC_RESULT_DELETE_FOREIGN_DEVICE_TABLE_ENTRY_NAK;
			break;
		case BVLC_DISTRIBUTE_BROADCAST_TO_NETWORK:
			result_code = BVLC_RESULT_DISTRIBUTE_BROADCAST_TO_NETWORK_NAK;
			break;
			/* case BVLC_FORWARDED_NPDU: */
			/* case BVLC_ORIGINAL_UNICAST_NPDU: */
			/* case BVLC_ORIGINAL_BROADCAST_NPDU: */
		default:
			break;
	}

	if (result_code > 0) {
		bvlc_send_result(addr, port, result_code);
	    //VERBOSE_STRING(stderr,"BVLC: NAK code=%d\n", result_code);
		VERBOSE_STRING("BVLC: NAK code");
	}
	return result_code;
}





BACNET_BVLC_FUNCTION bvlc_get_function_code(void)
{
    return BVLC_Function_Code;
}

//uint16_t bvlc_receive(
//    BACNET_ADDRESS * src,   /* returns the source address */
//    uint8_t * npdu, /* returns the NPDU */
//    uint16_t max_npdu,      /* amount of space available in the NPDU  */
//    unsigned timeout){      /* number of milliseconds to wait for a packet */
//
//    uint16_t npdu_len = 0;      /* return value */
//    int received_bytes = 0;
//    uint16_t result_code = 0;
//    uint8_t src_addr[] = {0,0,0,0};
//    uint16_t src_port = 0;
//
//    /* Make sure the socket is open */
//	if (bip_socket() < 0) {
//		return 0;
//	}
//
//	memcpy(&src_addr, &src->mac[0], 4);
//	memcpy(&src_port, &src->mac[4], 2);
//
//	/* Receive if there is something to receive*/
//	if(getRXReceivedSize_func(CW5100Class_new(), bip_socket())){
//		received_bytes = (int)recvfrom_func(bip_socket(), &npdu[0], max_npdu,
//								src_addr, &src_port);
//	}
//
//	/* See if there is a problem */
//	if (received_bytes < 0) {
//		return 0;
//	}
//	/* no problem, just no bytes */
//	if (received_bytes == 0) {
//		return 0;
//	}
//	/* the signature of a BACnet/IP packet */
//	if (npdu[0] != BVLL_TYPE_BACNET_IP) {
//		return 0;
//	}
//	BVLC_Function_Code = npdu[1];
//	/* decode the length of the PDU - length is inclusive of BVLC */
//	(void) decode_unsigned16(&npdu[2], &npdu_len);
//	/* subtract off the BVLC header */
//	npdu_len -= 4;
//	switch(BVLC_Function_Code){
//
//	}
//
//}
//int bvlc_send_pdu(
//    BACNET_ADDRESS * dest,  /* destination address */
//    BACNET_NPDU_DATA * npdu_data,   /* network information */
//    uint8_t * pdu,  /* any data to be sent - may be null */
//    unsigned pdu_len){
//
//	return 0;
//}
