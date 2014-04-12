/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "AnalogObject.h"

AnalogObject::AnalogObject(const uint32_t objectID, BACNET_OBJECT_TYPE objectType, const char* objectName,
							const char* description, BACNET_ENGINEERING_UNITS units, float high_limit, float low_limit):BacnetObject(objectID, objectType, objectName){
	Present_Value = (float)-1.0;
	characterstring_init_ansi(&Description, description);
	bitstring_init(&Status_Flags);
	bitstring_set_bit(&Status_Flags, STATUS_FLAG_FAULT, false);
	bitstring_set_bit(&Status_Flags, STATUS_FLAG_IN_ALARM, false);
	bitstring_set_bit(&Status_Flags, STATUS_FLAG_OUT_OF_SERVICE, false);
	bitstring_set_bit(&Status_Flags, STATUS_FLAG_OVERRIDDEN, false);
	Event_State = EVENT_STATE_NORMAL;
	Reliability = RELIABILITY_NO_FAULT_DETECTED;
	Out_Of_Service = false;
	Units = units;
	high_limit_ = high_limit;
	low_limit_ = low_limit;
}

AnalogObject::~AnalogObject(){}

bool AnalogObject::Object_Write_Property(BACNET_WRITE_PROPERTY_DATA * wpdata){
	wpdata->error_class = ERROR_CLASS_PROPERTY;
	wpdata->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
	return false;
}
