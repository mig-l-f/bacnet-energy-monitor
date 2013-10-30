/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "socketWrapper.h"
#include "socket.h"

extern "C"{

uint8_t socket_func(SOCKET s, uint8_t protocol, uint16_t port, uint8_t flag){
	return socket(s, protocol, port, flag);
}

void close_func(SOCKET s){
	close(s);
}
uint8_t connect_func(SOCKET s, uint8_t * addr, uint16_t port){
	return connect(s, addr, port);
}
void disconnect_func(SOCKET s){
	disconnect(s);
}
uint8_t listen_func(SOCKET s){
	return listen(s);
}
uint16_t send_func(SOCKET s, const uint8_t * buf, uint16_t len){
	return send(s, buf, len);
}
int16_t recv_func(SOCKET s, uint8_t * buf, int16_t len){
	return recv(s, buf, len);
}
uint16_t peek_func(SOCKET s, uint8_t *buf){
	return peek(s, buf);
}
uint16_t sendto_func(SOCKET s, const uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t port){
	return sendto(s, buf, len, addr, port);
}
uint16_t recvfrom_func(SOCKET s, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t *port){
	return recvfrom(s, buf, len, addr, port);
}

uint16_t igmpsend_func(SOCKET s, const uint8_t * buf, uint16_t len){
	return igmpsend(s, buf, len);
}

}// extern C



