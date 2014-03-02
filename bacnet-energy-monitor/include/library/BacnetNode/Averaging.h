/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef AVERAGING_H_
#define AVERAGING_H_

#include "BacnetObject.h"
extern "C"{
	#include "bacenum.h"
	#include "bacdcode.h"
}

class Averaging : public BacnetObject {
private:

public:
	Averaging(const uint32_t objectID, const char* objectName);
	~Averaging();
	unsigned getCount() const;
	bool getValid_Object_Instance_Number(uint32_t object_id);
	int Object_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata);
};

inline unsigned Averaging::getCount() const{
	return 1;
}

inline bool Averaging::getValid_Object_Instance_Number(uint32_t object_id){
	return Object_Identifier.instance == object_id;
}

#endif /* AVERAGING_H_ */
