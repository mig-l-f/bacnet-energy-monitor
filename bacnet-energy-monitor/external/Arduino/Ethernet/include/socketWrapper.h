/*
 * socketWrapper.h
 *
 *  Created on: 27 de Mai de 2013
 *      Author: mgf
 */

#ifndef SOCKETWRAPPER_H_
#define SOCKETWRAPPER_H_

#include <avr/pgmspace.h>

typedef uint8_t SOCKET;

#ifdef __cplusplus
extern "C"{
#endif

extern uint8_t socket_func(SOCKET s, uint8_t protocol, uint16_t port, uint8_t flag); // Opens a socket(TCP or UDP or IP_RAW mode)
extern void close_func(SOCKET s); // Close socket
extern uint8_t connect_func(SOCKET s, uint8_t * addr, uint16_t port); // Establish TCP connection (Active connection)
extern void disconnect_func(SOCKET s); // disconnect the connection
extern uint8_t listen_func(SOCKET s);	// Establish TCP connection (Passive connection)
extern uint16_t send_func(SOCKET s, const uint8_t * buf, uint16_t len); // Send data (TCP)
extern int16_t recv_func(SOCKET s, uint8_t * buf, int16_t len);	// Receive data (TCP)
extern uint16_t peek_func(SOCKET s, uint8_t *buf);
extern uint16_t sendto_func(SOCKET s, const uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t port); // Send data (UDP/IP RAW)
extern uint16_t recvfrom_func(SOCKET s, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t *port); // Receive data (UDP/IP RAW)

extern uint16_t igmpsend_func(SOCKET s, const uint8_t * buf, uint16_t len);


#ifdef __cplusplus
}
#endif

#endif /* SOCKETWRAPPER_H_ */
