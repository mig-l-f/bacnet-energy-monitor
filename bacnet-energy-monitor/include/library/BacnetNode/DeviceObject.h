/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef DEVICEOBJECT_H_
#define DEVICEOBJECT_H_

//#include "bacdef.h"
#include "bacstr.h"
#include "bacenum.h"
#include "handlers.h"
#include "verbose.h"

#include "apdu.h"
#include "handlers.h"
#include "string.h"
#include "Constants.h"
#include "BacnetObject.h"
#if defined(__AVR_ATmega328P__)
	#include <avr/pgmspace.h>
#endif

class DeviceObject: public BacnetObject {

private:

	BACNET_DEVICE_STATUS System_Status;
	BACNET_CHARACTER_STRING Vendor_Name;
	uint16_t Vendor_Identifier;
	BACNET_CHARACTER_STRING Model_Name;
	BACNET_CHARACTER_STRING Firmware_Revision;
	BACNET_CHARACTER_STRING Application_Software_Version;
	//BACNET_OBJECT_ID* Object_List[MAX_BACNET_OBJECTS_PER_DEVICE]; //TODO Refactor para OS_Keylist
	BACNET_OBJECT_ID Object_List[MAX_BACNET_OBJECTS_PER_DEVICE];
	uint32_t Max_APDU_Length_Accepted;
	BACNET_SEGMENTATION Segmentation_Supported;
	uint16_t APDU_Timeout;
//	uint8_t Number_Of_APDU_Retries;
	void InitializeVendorProperties();
	void InitializeObjectList();
	void InitializeAPDUSettings();
	//FIXME Device_Address_Binding not implemented
	uint32_t Database_Revision;
	BACNET_BIT_STRING Protocol_Object_Types_Supported;
	uint8_t numberOfObjectsInList;

public:

	DeviceObject(const uint32_t objectID = -999, const char* objectName = "NOTDEF",
			     BACNET_DEVICE_STATUS deviceStatus = STATUS_NON_OPERATIONAL);
	~DeviceObject();

	uint16_t getTypeFromObjectList(uint8_t index) const;
	uint32_t getInstanceFromObjectList(uint8_t index) const;
	bool addObjectIdToObjectList(BACNET_OBJECT_ID & objectId);
	bool isWithinLimits(int32_t& low_limit, int32_t& high_limit);
	//Bacnet Functions
	virtual unsigned getCount() const;
	virtual bool getValid_Object_Instance_Number(uint32_t object_id);
	virtual int Object_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata);
};


#endif /* DEVICEOBJECT_H_ */
