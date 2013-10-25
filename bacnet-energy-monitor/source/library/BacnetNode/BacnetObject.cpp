/*
 * Device.cpp
 *
 *  Created on: 10 de Fev de 2013
 *      Author: mgf
 */

#include "BacnetObject.h"

BACNET_OBJECT_TYPE BacnetObject::getObjectType() const {
	return Object_Type;
}

BACNET_CHARACTER_STRING* BacnetObject::getObjectName() {
	return &Object_Name;
}

BACNET_OBJECT_ID* BacnetObject::getObjectIdentifier() {
	return &Object_Identifier;
}

BacnetObject::BacnetObject(uint32_t objectID, BACNET_OBJECT_TYPE objectType,const char* objectName) {
	Object_Identifier.instance = objectID;
	Object_Identifier.type = objectType;
	characterstring_init_ansi(&Object_Name, objectName);
	Object_Type = objectType;
}



