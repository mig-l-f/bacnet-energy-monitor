/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include <avr/io.h>
#include <stdio.h>
#include "Arduino.h"
#include "arduino_cpp.h"
#include "JeeLib.h"
//#include "BacnetNode2Thermos.h"
#include "AveragingNode.h"
#include "OneWire.h"
#include "DallasTemperature.h"

extern "C"{
	#include "verbose.h"
	#include "npdu.h"
	#include "handlers.h"
	#include "datalink.h"
	#include "uip.h"
	#include "slipdev.h"
}

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
static FILE uart_output = {0};

//RFM12B vars
#define myNodeID 30 //node ID of Rx (range 0-30)
#define network 210 //network group (can be in the range 1-250).
#define freq RF12_433MHZ //Freq of RF12B can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ. Match freq to module
const int emonTx_NodeID=10; //ID of sender

// uIP vars
static struct uip_udp_conn *udp_connection = NULL;

// bacnet library
static uint8_t PDUBuffer[MAX_MPDU];
//static BacnetNode2Thermos* node = new BacnetNode2Thermos();
AveragingNode* node = new AveragingNode();
//#define ONE_WIRE_BUS 2
//OneWire sensors(ONE_WIRE_BUS);
//byte sensorsAddr[8];
//byte present;
//byte data[12];
float temperature;

void uart_init(){
#ifdef __AVR_ATmega328P__
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Turn on transmission and reception
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); //Use 8-bit character sizes

	//Set Baud
	UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8 - bits of the baud rate value into the high byte of the UBRR register
	UBRR0L = BAUD_PRESCALE; // Load lower 8 - bits of the baud rate value into the low byte of the UBRR register
#else
	// set baud rate
	UBRR1H = 0;
	UBRR1L = 103;

	// enable uart RX and TX
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	// set 8N1 frame format
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
#endif
}

int uart_putchar(char c, FILE *stream){
#ifdef __AVR_ATmega328P__
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
#else
	// wait for empty receive buffer
	while ((UCSR1A & (1<<UDRE1))==0);
	// send
	UDR1 = c;
#endif
}

void init_rfm12b(void){
	rf12_initialize(myNodeID,freq,network); //Initialize RFM12 with settings defined above
}

void writeToRfm12b(unsigned char c){
	rf12_sendNow(0, &c, sizeof(c));
}

unsigned char readFromRfm12b(char* c){
	if (rf12_recvDone()){
		if (rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0) {
			int node_id = (rf12_hdr & 0x1F);                 //extract nodeID from payload
			if (node_id == emonTx_NodeID) { //check data is coming from node with the corrct ID
				*c = *(char*)rf12_data;
//				Serial.print("Received character:");Serial.println(*c);
				return 1;
			}
		}
	}
	return 0;
}

// Callback for TCP connections
void serialip_appcall(void){}

void uip_udp_received_data_callback(void){
	uint16_t pdu_len = 0;
	BACNET_ADDRESS src = { 0 };
	pdu_len = datalink_receive(&src, &PDUBuffer[0], sizeof(PDUBuffer), 0);
	if (pdu_len) {
		VERBOSE_STRING(PSTR("--- Received Request ---\n"));
	    npdu_handler(&src, &PDUBuffer[0], pdu_len);
    }
}

void setup(void){
	//Set Serial Output
	uart_init();
	fdev_setup_stream (&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &uart_output;
	stderr = &uart_output;

	fprintf(stderr,"Doing Setup...");

	// RADIO Connection
	init_rfm12b();
	slipdev_init();
	attach_functs(readFromRfm12b, writeToRfm12b);

	// uIP Stack
	uip_init();

	uip_ipaddr_t ipaddr;
	uip_ipaddr(ipaddr, 192, 168, 5, 2);
	uip_sethostaddr(ipaddr);
	uip_ipaddr(ipaddr, 255, 255, 255, 0);
	uip_setnetmask(ipaddr);

	if (udp_connection != NULL){
		uip_udp_remove(udp_connection);
	}
	// remote address 0.0.0.0 and port 0 allow for any incoming ip and any port.
	// Filter done upon reception
	uip_ipaddr(ipaddr, 0, 0, 0, 0);
	udp_connection = uip_udp_new(&ipaddr, HTONS(0));
	if (udp_connection != 0){
		uip_udp_bind(udp_connection, HTONS(47808));
	}

	// Bacnet Stack
	datalink_init(NULL);
	fprintf(stderr, "END\n");
}


void loop(void){
	uip_len = slipdev_poll();
	if(uip_len > 0) {
		uip_input();

		// input processing as generated data to send
		if (uip_len > 0) slipdev_send();
	}

	// Read data from sensors
//	sensors.reset();
//	sensors.select(sensorsAddr); // Select Sensor
//	sensors.write(0x44, 1); //issue sensor to start conversion
//	delay(850);
//	present = sensors.reset();
//	sensors.select(sensorsAddr);
//	sensors.write(0xBE); // issue read scratchpad command
//	//Receive 9 bytes
//	for (uint8_t i = 0; i < 9; i++){
//		data[i] = sensors.read();
//	}
//	//convert to degrees
//	//temperature = ( (data[1] << 8) + data[0])*0.0625;
	temperature = 25.0;
	node->setPresentValue(temperature);
	node->makeNewMeasurement(temperature);
//	node->getAnalogObjectFromList(0)->setPresentValue(temperature);
//	node->getAnalogObjectFromList(1)->setPresentValue(temperature+1.0);
}

int main(
    void)
{

    init();

#if defined(USBCON)
    USBDevice.attach();
#endif

    setup();

    for (;;) {
    	loop();
    }
}




