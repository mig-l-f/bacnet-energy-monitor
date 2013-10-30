/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include <stdio.h>

#include "BacnetNode.h"
#include "datalink.h"
#include "dlenv.h"

static uint8_t PDUBuffer[MAX_MPDU];
float temperature;

void setup(){
	fprintf(stdout,"Starting BACNET application...\n");
}

int main(void){
	BacnetNode* node = new BacnetNode();
	uint16_t pdu_len = 0;
	BACNET_ADDRESS src = { 0 }; /* source address */

	setup();
	dlenv_init();
	//datalink_init(NULL);

	for(;;){
		/* other tasks */
		/* BACnet handling */
		pdu_len = datalink_receive(&src, &PDUBuffer[0], sizeof(PDUBuffer), 0);
		if (pdu_len) {
			fprintf(stdout,"--- Received Request ---\n");
			npdu_handler(&src, &PDUBuffer[0], pdu_len);
		}

		/*Temperature sensor emulation*/
		temperature = 15 + (float)rand()/((float)RAND_MAX/(25-15));
		node->getAnalogObjectFromList(0)->setPresentValue(temperature);
	}
}




