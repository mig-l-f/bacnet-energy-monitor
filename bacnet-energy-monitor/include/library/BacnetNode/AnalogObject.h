/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef ANALOGOBJECT_H_
#define ANALOGOBJECT_H_

#include "BacnetObject.h"

extern "C" {
	#include "bacenum.h"
	#include "wp.h"
}

class AnalogObject : public BacnetObject{
protected:
	float Present_Value;
	BACNET_CHARACTER_STRING Description;
	BACNET_BIT_STRING Status_Flags;
	BACNET_EVENT_STATE Event_State;
	BACNET_RELIABILITY Reliability;
	bool Out_Of_Service;
	BACNET_ENGINEERING_UNITS Units;

public:
	AnalogObject(const uint32_t objectID, BACNET_OBJECT_TYPE objectType, const char* objectName,
					const char* description = "NOTDEF", BACNET_ENGINEERING_UNITS units = UNITS_PERCENT);
	~AnalogObject();

	virtual void setPresentValue(float value) = 0;
	virtual unsigned getCount() const = 0;
	virtual bool getValid_Object_Instance_Number(uint32_t object_id) = 0;
	virtual int Object_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata) = 0;
	virtual bool Object_Write_Property(BACNET_WRITE_PROPERTY_DATA * wpdata);
};


#endif /* ANALOGOBJECT_H_ */
