/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#include "datalink.h"
#include "npdu.h"
#include "handlers.h"
#include "txbuf.h"
#include "iam.h"
#include "BacnetNode.h"
#include "stack.h"
#include "verbose.h"

#include "w5100Wrapper.h"
#include "Arduino.h"
#include "arduino_cpp.h"
#include "OneWire.h"
#include "DallasTemperature.h"

static FILE uart_output = {0};

/* local version override */
static uint8_t Ethernet_MAC_Address[MAX_MAC_LEN] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
uint8_t ipAddress[] = {192, 168, 0, 185};
uint8_t gateway[] = {192, 168, 0, 1};
uint8_t netmask[] = {255, 255, 255, 0};
BacnetNode* node = new BacnetNode();

//Temperature sensor
#define ONE_WIRE_BUS 2
OneWire sensors(ONE_WIRE_BUS);
byte sensorsAddr[8];
byte present;
byte data[12];
float temperature;

static int uart_putchar (char c, FILE *stream)
{
    Serial.write(c) ;
    return 0 ;
}

void setup(){
	Serial.begin(9600);
	fdev_setup_stream (&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &uart_output;
	stderr = &uart_output;

	VERBOSE_STRING(PSTR("Starting BACNET application.."));

	//INIT W5100
    init_func(CW5100Class_new());
	setMACAddress_func(CW5100Class_new(), Ethernet_MAC_Address);
	setIPAddress_func(CW5100Class_new(), ipAddress);
	setGatewayIp_func(CW5100Class_new(), gateway);
	setSubnetMask_func(CW5100Class_new(), netmask);

	// Searching for Temperature sensor
	sensors.search(sensorsAddr);
	if (OneWire::crc8( sensorsAddr, 7) != sensorsAddr[7]){
		VERBOSE_STRING(PSTR("... Failed\n"));
	}else{
		VERBOSE_STRING(PSTR("... Completed\n"));
	}
}


static uint8_t PDUBuffer[MAX_MPDU];
int main(
    void)
{
	uint16_t pdu_len = 0;
	BACNET_ADDRESS src = { 0 }; /* source address */

    init();

    setup();

    datalink_init(NULL);


    for (;;) {
        /* other tasks */
//    	uint16_t sSize = stack_size();
//    	uint16_t freeStack = freeRam();
//    	uint16_t hSize = heapSize();
//    	fprintf(stderr,"StackSize: %d  HeapSize: %d Free: %d\n", sSize, hSize, freeStack);

        /* BACnet handling */
        pdu_len = datalink_receive(&src, &PDUBuffer[0], sizeof(PDUBuffer), 0);
        if (pdu_len) {
        	VERBOSE_STRING(PSTR("--- Received Request ---\n"));
        	npdu_handler(&src, &PDUBuffer[0], pdu_len);
        }

        /*Update sensor values*/
        sensors.reset();
        sensors.select(sensorsAddr); // Select Sensor
        sensors.write(0x44, 1); //issue sensor to start conversion
        delay(850);
        present = sensors.reset();
        sensors.select(sensorsAddr);
        sensors.write(0xBE); // issue read scratchpad command
        //Receive 9 bytes
        for (uint8_t i = 0; i < 9; i++){
        	data[i] = sensors.read();
        }
        //convert to degrees
        temperature = ( (data[1] << 8) + data[0])*0.0625;
        node->getAnalogObjectFromList(0)->setPresentValue(temperature);
    }
}
