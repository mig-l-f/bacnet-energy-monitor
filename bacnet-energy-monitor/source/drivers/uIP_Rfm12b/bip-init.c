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
#include "bacdcode.h"
#include "bip.h"
#include "uip.h"

#ifdef PRINT_ENABLE
#include <stdio.h>
#endif


/** @file linux/bip-init.c  Initializes BACnet/IP interface (Linux). */

bool BIP_Debug = false;

/* gets an IP address by name, where name can be a
   string that is an IP address in dotted form, or
   a name that is a domain name
   returns 0 if not found, or
   an IP address in network byte order */
long bip_getaddrbyname(
    const char *host_name)
{
//    struct hostent *host_ent;
//
//    if ((host_ent = gethostbyname(host_name)) == NULL)
//        return 0;
//
//    return *(long *) host_ent->h_addr;
	return 0;
}

/** Gets the local IP address and local broadcast address from the system,
 *  and saves it into the BACnet/IP data structures.
 *
 * @param ifname [in] The named interface to use for the network layer.
 *        Eg, for Linux, ifname is eth0, ath0, arc0, and others.
 */
void bip_set_interface(
    char *ifname)
{

	uip_ipaddr_t ipaddr;
	uip_gethostaddr(&ipaddr);
	//convert ipaddr to uint8_t addresses
	uint8_t local_address[] = {uip_ipaddr1(&ipaddr), uip_ipaddr2(&ipaddr), uip_ipaddr3(&ipaddr), uip_ipaddr4(&ipaddr)};
    bip_set_addr(local_address);

#ifdef PRINT_ENABLE
        fprintf(stderr, "Interface: %s\n", ifname);
        fprintf(stderr, "IP Address: %d.%d.%d.%d\n", local_address[0],local_address[1],
        											 local_address[2],local_address[3]);
#endif

    uip_getnetmask(&ipaddr);
    uint8_t netmask[] = {uip_ipaddr1(&ipaddr), uip_ipaddr2(&ipaddr), uip_ipaddr3(&ipaddr), uip_ipaddr4(&ipaddr)};
    uint8_t broadcast_address[] = {0, 0, 0, 0 };
    uint8_t invertedNetmask[] = {0,0,0,0};
    for(int i = 0; i < 4; i++){ //FIXME: IPv4 ?
    	invertedNetmask[i] = ~netmask[i];
    	broadcast_address[i] = (local_address[i] | invertedNetmask[i]);
    }

    bip_set_broadcast_addr(broadcast_address);
#ifdef PRINT_ENABLE
    fprintf(stderr, "IP Broadcast Address: %d.%d.%d.%d\n",broadcast_address[0],
      		broadcast_address[1],broadcast_address[2], broadcast_address[3]);
	fprintf(stderr,"bip_set_interface, intf=%s\n",ifname);
#endif
}

/** Initialize the BACnet/IP services at the given interface.
 * @ingroup DLBIP
 * -# Gets the local IP address and local broadcast address from the system,
 *  and saves it into the BACnet/IP data structures.
 * -# Opens a UDP socket
 * -# Configures the socket for sending and receiving
 * -# Configures the socket so it can send broadcasts
 * -# Binds the socket to the local IP address at the specified port for
 *    BACnet/IP (by default, 0xBAC0 = 47808).
 *
 * @note For Linux, ifname is eth0, ath0, arc0, and others.
 *
 * @param ifname [in] The named interface to use for the network layer.
 *        If NULL, the "eth0" interface is assigned.
 * @return True if the socket is successfully opened for BACnet/IP,
 *         else False if the socket functions fail.
 */
bool bip_init(char *ifname)
{
#ifdef PRINT_ENABLE
    fprintf(stderr,"Bip-Init..\n");
#endif

    if (ifname)
        bip_set_interface(ifname);
    else
        bip_set_interface("sl0");

    /* assumes that the driver has already been initialized */
    struct uip_udp_conn *c = uip_udp_new(NULL, HTONS(0)); /* Connection from any Remote coming from any port*/
    if (c != 0){
    	//Sets the local port where we are listening
    	uip_udp_bind(c, HTONS(47808));
    	bip_set_socket(c);
    	bip_set_port(47808);
    	return true;
    }
    else{
    	/* Socket not open */
		#ifdef PRINT_ENABLE
    		fprintf(stderr,"Socket Nao Aberto\n");
		#endif
    	bip_set_socket(NULL);
    }

    /* Allow us to use the same socket for sending and receiving */
    /* This makes sure that the src port is correct when sending */
//    sockopt = 1;
//    status =
//        setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &sockopt,
//        sizeof(sockopt));
//    if (status < 0) {
//        close(sock_fd);
//        bip_set_socket(-1);
//        return status;
//    }
    /* allow us to send a broadcast */
//    status =
//        setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &sockopt,
//        sizeof(sockopt));
//    if (status < 0) {
//        close(sock_fd);
//        bip_set_socket(-1);
//        return false;
//    }
    /* bind the socket to the local port number and IP address */
//    sin.sin_family = AF_INET;
//    sin.sin_addr.s_addr = htonl(INADDR_ANY);
//    sin.sin_port = bip_get_port();
//    memset(&(sin.sin_zero), '\0', sizeof(sin.sin_zero));
//    status =
//        bind(sock_fd, (const struct sockaddr *) &sin, sizeof(struct sockaddr));
//    if (status < 0) {
//        close(sock_fd);
//        bip_set_socket(-1);
//        return false;
//    }

    return false;
}

/** Cleanup and close out the BACnet/IP services by closing the socket.
 * @ingroup DLBIP
  */
void bip_cleanup(
    void)
{
	struct uip_udp_conn* conn = NULL;

    if (bip_valid()) {
        conn = bip_socket();
        uip_udp_remove(conn);
    }
    bip_set_socket(NULL);

    return;
}

/** Get the netmask of the BACnet/IP's interface via an ioctl() call.
 * @param netmask [out] The netmask, in host order.
 * @return 0 on success, else the error from the ioctl() call.
 */
int bip_get_local_netmask(
    uint8_t *netmask)
{
	uip_ipaddr_t ipaddr;
	uip_getnetmask(&ipaddr);
	netmask[0] = uip_ipaddr1(&ipaddr);
	netmask[1] = uip_ipaddr2(&ipaddr);
	netmask[2] = uip_ipaddr3(&ipaddr);
	netmask[3] = uip_ipaddr4(&ipaddr);
    return 0;
}
