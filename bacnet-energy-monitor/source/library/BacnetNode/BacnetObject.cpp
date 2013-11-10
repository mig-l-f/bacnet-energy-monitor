/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
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

bool BacnetObject::isObject(uint32_t& objectID, BACNET_OBJECT_TYPE objectType){
	if((Object_Identifier.instance == objectID) & (Object_Identifier.type == objectType))
		return true;
	else
		return false;
}


