/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef ANALOGVALUE_H_
#define ANALOGVALUE_H_

#include "BacnetObject.h"
#include "bacenum.h"
#include "bacstr.h"
#include "bacdcode.h"
#include "AnalogObject.h"

class AnalogValue : public AnalogObject {
private:


public:
	AnalogValue(const uint32_t objectID, const char* objectName, const char* description = "NOTDEF", BACNET_ENGINEERING_UNITS units = UNITS_PERCENT);
	~AnalogValue();


	void setPresentValue(float value);
	unsigned getCount() const;
	bool getValid_Object_Instance_Number(uint32_t object_id);
	int Object_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata);
};

inline unsigned AnalogValue::getCount() const {
	return 1;
}

inline bool AnalogValue::getValid_Object_Instance_Number(uint32_t object_id){
	return Object_Identifier.instance == object_id;
}

#endif /* ANALOGVALUE_H_ */
