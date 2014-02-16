/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2005 Steve Karg

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to:
 The Free Software Foundation, Inc.
 59 Temple Place - Suite 330
 Boston, MA  02111-1307, USA.

 As a special exception, if other files instantiate templates or
 use macros or inline functions from this file, or you compile
 this file and link it with other works to produce a work based
 on this file, this file does not by itself cause the resulting
 work to be covered by the GNU General Public License. However
 the source code for this file must still be made available in
 accordance with section (3) of the GNU General Public License.

 This exception does not invalidate any other reasons why a work
 based on this file might be covered by the GNU General Public
 License.
 -------------------------------------------
####COPYRIGHTEND####*/

#include <stdint.h>     /* for standard integer types uint8_t etc. */
#include <stdbool.h>    /* for the standard bool type. */
#include <string.h>
#include "bacdcode.h"
#include "bacint.h"
#include "bip.h"
#include "bvlc-arduino.h"
#include "uip.h"

//#include "net.h"        /* custom per port */
#ifdef PRINT_ENABLE
#include <stdio.h>      /* for standard i/o, like printing */
#endif

/** @file bip.c  Configuration and Operations for BACnet/IP */

static struct uip_udp_conn *BIP_Socket = NULL; /* Pointer to current socket */
//static uint8_t BIP_Socket = MAX_SOCK_NUM;
/* port to use - stored in network byte order */
static uint16_t BIP_Port = 0;   /* this will force initialization in demos */
/* IP Address - stored in network byte order */
//static struct in_addr BIP_Address;
static uip_ipaddr_t BIP_Address;//[4] = {0, 0, 0, 0};
/* Broadcast Address - stored in network byte order */
//static struct in_addr BIP_Broadcast_Address;
static uip_ipaddr_t BIP_Broadcast_Address;//[4] = {0, 0, 0, 0};

///** Converter from uint8_t[4] type address to uint32_t
// *
// */
//uint32_t convertBIP_Address2uint32(uint8_t* bip_address){
//	return (uint32_t)((bip_address[0]*2^24) + (bip_address[1]*2^16) +
//			          (bip_address[2]*2^8) + bip_address[3]);
//}
///** Convert from uint32_t IPv4 address to uint8_t[4] address
// *
// */
//void convertUint32Address_2_uint8Address(uint32_t ip, uint8_t * address){
//	address[0] = (uint8_t)(ip>>24);
//	address[1] = (uint8_t)(ip>>16);
//	address[2] = (uint8_t)(ip>>8);
//	address[3] = (uint8_t)(ip>>0);
//}

/** Setter for the BACnet/IP socket handle.
 *
 * @param sock_fd [in] Handle for the BACnet/IP socket.
 */
void bip_set_socket(struct uip_udp_conn* sock_fd)
{
    BIP_Socket = sock_fd;
}

/** Getter for the BACnet/IP socket handle.
 *
 * @return The handle to the BACnet/IP socket.
 */
struct uip_udp_conn* bip_socket(
    void)
{
    return BIP_Socket;
}

bool bip_valid(
    void)
{
    //return (BIP_Socket < MAX_SOCK_NUM);
	return (BIP_Socket != NULL);
}

void bip_set_addr(uip_ipaddr_t * net_address)
{       /* in network byte order */
    uip_ipaddr_copy(&BIP_Address, net_address);
}

/* returns network byte order */
uip_ipaddr_t* bip_get_addr(void)
{
    return &BIP_Address;
}

void bip_set_broadcast_addr(uip_ipaddr_t * net_address)
{       /* in network byte order */
	uip_ipaddr_copy(&BIP_Broadcast_Address, net_address);
}

/* returns network byte order */
uip_ipaddr_t* bip_get_broadcast_addr(void)
{
    return &BIP_Broadcast_Address;
}


void bip_set_port(uint16_t port)
{       /* in network byte order */
    BIP_Port = port;
}

/* returns network byte order */
uint16_t bip_get_port(void)
{
    return BIP_Port;
}

static int bip_decode_bip_address(
    BACNET_ADDRESS * bac_addr,
    uip_ipaddr_t *address,    /* in network format */
    uint16_t * port)
{       /* in network format */
    int len = 0;

    if (bac_addr) {
    	uip_ipaddr(address, bac_addr->mac[0], bac_addr->mac[1], bac_addr->mac[2], bac_addr->mac[3]);
        //memcpy(address, &bac_addr->mac[0], 4);
        memcpy(port, &bac_addr->mac[4], 2);
        len = 6;
    }

    return len;
}

/** Function to send a packet out the BACnet/IP socket (Annex J).
 * @ingroup DLBIP
 *
 * @param dest [in] Destination address (may encode an IP address and port #).
 * @param npdu_data [in] The NPDU header (Network) information (not used).
 * @param pdu [in] Buffer of data to be sent - may be null (why?).
 * @param pdu_len [in] Number of bytes in the pdu buffer.
 * @return Number of bytes sent on success, negative number on failure.
 */
int bip_send_pdu(
    BACNET_ADDRESS * dest,      /* destination address */
    BACNET_NPDU_DATA * npdu_data,       /* network information */
    uint8_t * pdu,      /* any data to be sent - may be null */
    unsigned pdu_len)
{       /* number of bytes of data */
//    struct sockaddr_in bip_dest;
    uint8_t mtu[MAX_MPDU] = { 0 };
    int mtu_len = 0;
    int bytes_sent = 0;
    /* addr and port in host format */
    uip_ipaddr_t address;//[] = {0,0,0,0};
    uint16_t port = 0;

    (void) npdu_data;
    /* assumes that the driver has already been initialized */
    if (BIP_Socket == NULL) {
        return 0;
    }

    mtu[0] = BVLL_TYPE_BACNET_IP;
    if ((dest->net == BACNET_BROADCAST_NETWORK) || ((dest->net > 0) &&
            (dest->len == 0)) || (dest->mac_len == 0)) {
        /* broadcast */
    	uip_ipaddr_copy(&address, &BIP_Broadcast_Address);
        port = BIP_Port;
        mtu[1] = BVLC_ORIGINAL_BROADCAST_NPDU;
#ifdef PRINT_ENABLE
        fprintf(stderr,"Send Broadcast NPDU to %d.%d.%d.%d:%d\n", uip_ipaddr1(&address), uip_ipaddr2(&address)
                                                   ,uip_ipaddr3(&address), uip_ipaddr4(&address), port);
#endif
    } else if (dest->mac_len == 6) {
        bip_decode_bip_address(dest, &address, &port);
        mtu[1] = BVLC_ORIGINAL_UNICAST_NPDU;
#ifdef PRINT_ENABLE
        fprintf(stderr,"Send Unicast NPDU to %d.%d.%d.%d:%d\n", uip_ipaddr1(&address), uip_ipaddr2(&address)
                											,uip_ipaddr3(&address), uip_ipaddr4(&address), port);
#endif
    } else {
        /* invalid address */
        return -1;
    }
//    bip_dest.sin_addr.s_addr = address.s_addr;
//    bip_dest.sin_port = port;
//    memset(&(bip_dest.sin_zero), '\0', 8);
    mtu_len = 2;
    mtu_len +=
        encode_unsigned16(&mtu[mtu_len],
        (uint16_t) (pdu_len + 4 /*inclusive */ ));
    memcpy(&mtu[mtu_len], pdu, pdu_len);
    mtu_len += pdu_len;

#ifdef PRINT_ENABLE
        fprintf(stderr,"Sending MTU with size %d\n", mtu_len);
#endif

    uint8_t* dataToSend = (uint8_t*)uip_appdata; // uIP buffer
    memset(dataToSend, 0, mtu_len); /* Clean buffer */
    memcpy(dataToSend, mtu, mtu_len);
    /* Send the packet */
    uip_udp_send(mtu_len);

    return mtu_len;
}

/** Implementation of the receive() function for BACnet/IP; receives one
 * packet, verifies its BVLC header, and removes the BVLC header from
 * the PDU data before returning.
 *
 * @param src [out] Source of the packet - who should receive any response.
 * @param pdu [out] A buffer to hold the PDU portion of the received packet,
 * 					after the BVLC portion has been stripped off.
 * @param max_pdu [in] Size of the pdu[] buffer.
 * @param timeout [in] The number of milliseconds to wait for a packet.
 * @return The number of octets (remaining) in the PDU, or zero on failure.
 */
uint16_t bip_receive(
    BACNET_ADDRESS * src,       /* source address */
    uint8_t * pdu,      /* PDU data */
    uint16_t max_pdu,   /* amount of space available in the PDU  */
    unsigned timeout)
{
    int received_bytes = 0;
	uint8_t * receivedDataBuffer;
    uint16_t pdu_len = 0;       /* return value */
    //uint8_t src_addr[] = {0,0,0,0};
    uip_ipaddr_t src_addr;
    uint16_t src_port = 0;
    uint16_t i = 0;
    int function = 0;

    received_bytes = uip_datalen();
    if (received_bytes > 0){
    	receivedDataBuffer = (uint8_t*)uip_appdata;
    	memcpy(pdu, receivedDataBuffer, received_bytes);
    	struct uip_udpip_hdr * udp_header = (struct uip_udpip_hdr *)&uip_buf[UIP_LLH_LEN];
    	//fill source addres
    	uip_ipaddr_copy(&src_addr, udp_header->srcipaddr);
    	src_port = ntohs(udp_header->srcport);
    } else {
    	return 0;
    }

    /* the signature of a BACnet/IP packet */
    if (pdu[0] != BVLL_TYPE_BACNET_IP){
        return 0;
    }

    if (bvlc_for_non_bbmd(&src_addr, &src_port, pdu, received_bytes) > 0) {
        /* Handled, usually with a NACK. */
#ifdef PRINT_ENABLE
        fprintf(stderr, "BIP: BVLC discarded!\n");
#endif
        return 0;
    }

    function = bvlc_get_function_code();        /* aka, pdu[1] */
    if ((function == BVLC_ORIGINAL_UNICAST_NPDU) ||
        (function == BVLC_ORIGINAL_BROADCAST_NPDU)) {
        /* ignore messages from me */
        if ((uip_ipaddr_cmp(&src_addr, &BIP_Address)) && (src_port == BIP_Port)) {
            pdu_len = 0;
#if 0
            fprintf(stderr, "BIP: src is me. Discarded!\n");
#endif
        } else {
            /* data in src->mac[] is in network format */
            src->mac_len = 6;
            memcpy(&src->mac[0], &src_addr, 4);
            memcpy(&src->mac[4], &src_port, 2);
#ifdef PRINT_ENABLE
            fprintf(stderr,"BIP receive from %d.%d.%d.%d\n",src->mac[0], src->mac[1],
            												src->mac[2], src->mac[3]);
#endif
            /* FIXME: check destination address */
            /* see if it is broadcast or for us */
            /* decode the length of the PDU - length is inclusive of BVLC */
            (void) decode_unsigned16(&pdu[2], &pdu_len);
            /* subtract off the BVLC header */
            pdu_len -= 4;
            if (pdu_len < max_pdu) {
#if 0
                fprintf(stderr, "BIP: NPDU[%hu]:", pdu_len);
#endif
                /* shift the buffer to return a valid PDU */
                for (i = 0; i < pdu_len; i++) {
                    pdu[i] = pdu[4 + i];
#if 0
                    fprintf(stderr, "%02X ", pdu[i]);
#endif
                }
#if 0
                fprintf(stderr, "\n");
#endif
            }
            /* ignore packets that are too large */
            /* clients should check my max-apdu first */
            else {
                pdu_len = 0;
#ifdef PRINT_ENABLE
                fprintf(stderr, "BIP: PDU too large. Discarded!.\n");
#endif
            }
        }
    } else if (function == BVLC_FORWARDED_NPDU) {
        memcpy(&src_addr, &pdu[4], 4);
        memcpy(&src_port, &pdu[8], 2);
        if ((uip_ipaddr_cmp(&src_addr, &BIP_Address)) && (src_port == BIP_Port)) {
            /* ignore messages from me */
            pdu_len = 0;
        } else {
            /* data in src->mac[] is in network format */
            src->mac_len = 6;
            memcpy(&src->mac[0], &src_addr, 4);
            memcpy(&src->mac[4], &src_port, 2);
            /* FIXME: check destination address */
            /* see if it is broadcast or for us */
            /* decode the length of the PDU - length is inclusive of BVLC */
            (void) decode_unsigned16(&pdu[2], &pdu_len);
            /* subtract off the BVLC header */
            pdu_len -= 10;
            if (pdu_len < max_pdu) {
                /* shift the buffer to return a valid PDU */
                for (i = 0; i < pdu_len; i++) {
                    pdu[i] = pdu[4 + 6 + i];
                }
            } else {
                /* ignore packets that are too large */
                /* clients should check my max-apdu first */
                pdu_len = 0;
            }
        }
    }

    return pdu_len;
}

void bip_get_my_address(
    BACNET_ADDRESS * my_address)
{
    int i = 0;

    if (my_address) {
        my_address->mac_len = 6;
        //memcpy(&my_address->mac[0], &BIP_Address, 4);
        my_address->mac[0] = uip_ipaddr1(BIP_Address);
        my_address->mac[1] = uip_ipaddr2(BIP_Address);
        my_address->mac[2] = uip_ipaddr3(BIP_Address);
        my_address->mac[3] = uip_ipaddr4(BIP_Address);
        memcpy(&my_address->mac[4], &BIP_Port, 2);
        my_address->net = 0;    /* local only, no routing */
        my_address->len = 0;    /* no SLEN */
        for (i = 0; i < MAX_MAC_LEN; i++) {
            /* no SADR */
            my_address->adr[i] = 0;
        }
    }

    return;
}

void bip_get_broadcast_address(
    BACNET_ADDRESS * dest)
{       /* destination address */
    int i = 0;  /* counter */

    if (dest) {
        dest->mac_len = 6;
        //memcpy(&dest->mac[0], &BIP_Broadcast_Address, 4);
        dest->mac[0] = uip_ipaddr1(BIP_Broadcast_Address);
        dest->mac[1] = uip_ipaddr2(BIP_Broadcast_Address);
        dest->mac[2] = uip_ipaddr3(BIP_Broadcast_Address);
        dest->mac[3] = uip_ipaddr4(BIP_Broadcast_Address);
        memcpy(&dest->mac[4], &BIP_Port, 2);
        dest->net = BACNET_BROADCAST_NETWORK;
        dest->len = 0;  /* no SLEN */
        for (i = 0; i < MAX_MAC_LEN; i++) {
            /* no SADR */
            dest->adr[i] = 0;
        }
    }

    return;
}
