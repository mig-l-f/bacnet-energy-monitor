/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#if defined(__AVR_ATmega328P__)
	#include <avr/pgmspace.h>

/* Variables to be optimized in AVR*/
	const char VENDOR_NAME[] PROGMEM = "FreeEnergy";
	const char MODEL_NAME[] PROGMEM = "MyFreeEnergy";
	const char FIRMWARE_REV[] PROGMEM = "1.0";
	const char SOFTWARE_VERSION[] PROGMEM = "0.0.1";
	PGM_P const string_table[] PROGMEM = {VENDOR_NAME, MODEL_NAME, FIRMWARE_REV, SOFTWARE_VERSION};
#else

	const char VENDOR_NAME[]="FreeEnergy";
	const char MODEL_NAME[]="MyFreeEnergy";
	const char FIRMWARE_REV[]="1.0";
	const char SOFTWARE_VERSION[]="0.0.1";
#endif

/* Variables not optimized*/
	static const uint16_t VENDOR_IDENTIFIER = 700;
	const uint8_t MAX_BACNET_OBJECTS_PER_DEVICE=3;

#endif /* CONSTANTS_H_ */
