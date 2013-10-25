/*
 * mybvlc.h
 *
 *  Created on: 6 de Jun de 2013
 *      Author: mgf
 */

#ifndef MYBVLC_H_
#define MYBVLC_H_

#include <stdint.h>
#include "bacenum.h"
#include "bacdef.h"
#include "npdu.h"

uint16_t bvlc_for_non_bbmd(uint8_t * addr,
		uint16_t * port,
		uint8_t * npdu,
		uint16_t received_bytes);

BACNET_BVLC_FUNCTION bvlc_get_function_code(void);

//uint16_t bvlc_receive(
//    BACNET_ADDRESS * src,   /* returns the source address */
//    uint8_t * npdu, /* returns the NPDU */
//    uint16_t max_npdu,      /* amount of space available in the NPDU  */
//    unsigned timeout);      /* number of milliseconds to wait for a packet */
//
//int bvlc_send_pdu(
//    BACNET_ADDRESS * dest,  /* destination address */
//    BACNET_NPDU_DATA * npdu_data,   /* network information */
//    uint8_t * pdu,  /* any data to be sent - may be null */
//    unsigned pdu_len);

#endif /* MYBVLC_H_ */
