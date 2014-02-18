/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "JeeLib.h"
#include "Arduino.h"

#define myNodeID 10 //node ID of tx (range 0-30)
#define network 210 //network group (can be in the range 1-250).
#define freq RF12_433MHZ //Freq of RF12B can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ. Match freq to module
#define arduinoNodeID 30


// Send character through RFM12B
void put(unsigned char c){
	rf12_sendNow(0, &c, sizeof(c));
}

//Verify if something has arrived at RFM12B
unsigned char poll(char* c){
	if (rf12_recvDone()){
		if (rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0) {
			int node_id = (rf12_hdr & 0x1F);                 //extract nodeID from payload
			if (node_id == arduinoNodeID) { //check data is coming from node with the corrct ID
				*c = *(char*)rf12_data;
				return 1;
			}
		}
	}
	return 0;
}


void setup() {
	Serial.begin(38400);

	rf12_initialize(myNodeID,freq,network); //Initialize RFM12 with settings defined above

}

void loop() {
	if(Serial.available()){
		char c = Serial.read();
		put(c);
	}
	char c1;
	if(poll(&c1)){
		Serial.print(c1);
	}

}


int main(void){
	init();

	setup();

	for(;;){
		loop();
	}
}
