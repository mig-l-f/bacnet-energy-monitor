/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include "bacdef.h"
#include "bacstr.h"
#include "rp.h"
#include "apdu.h"
#include "string.h"

class BacnetObject {
	private:

	protected:
		BACNET_OBJECT_ID Object_Identifier;
		BACNET_CHARACTER_STRING Object_Name;
		BACNET_OBJECT_TYPE Object_Type;

	public:
		BacnetObject(uint32_t objectID, BACNET_OBJECT_TYPE objectType,const char* objectName);
		virtual ~BacnetObject() {};
		BACNET_OBJECT_ID* getObjectIdentifier();
//		BACNET_CHARACTER_STRING* getObjectName();
//		BACNET_OBJECT_TYPE getObjectType() const;
		bool isObject(uint32_t& objectID, BACNET_OBJECT_TYPE objectType);

		virtual unsigned getCount() const = 0;
		virtual bool getValid_Object_Instance_Number(uint32_t object_id) = 0;
		virtual int Object_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata) = 0;

};

#endif /* DEVICE_H_ */
