/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "Averaging.h"

Averaging::Averaging(const uint32_t objectID, const char* objectName):BacnetObject(objectID, OBJECT_AVERAGING, objectName){}

Averaging::~Averaging(){}

int Averaging::Object_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata){
	int apdu_len = 0;   /* return value */
	uint8_t *apdu = NULL;

	if ((rpdata == NULL) || (rpdata->application_data == NULL) || (rpdata->application_data_len == 0)) {
		return 0;
	}
	if (rpdata->array_index != BACNET_ARRAY_ALL){ //Object does not have array properties
		rpdata->error_class = ERROR_CLASS_PROPERTY;
		rpdata->error_code = ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY;
		return BACNET_STATUS_ERROR;
	}
	apdu = rpdata->application_data;
	switch (rpdata->object_property) {
		case PROP_OBJECT_NAME:
			apdu_len = encode_application_character_string(&apdu[0], &Object_Name);
			break;
		case PROP_OBJECT_IDENTIFIER:
			apdu_len = encode_application_object_id(&apdu[0], Object_Identifier.type, Object_Identifier.instance);
			break;
		case PROP_OBJECT_TYPE:
			apdu_len = encode_application_enumerated(&apdu[0], Object_Identifier.type);
			break;
		default:
			apdu_len = 0;
			break;
	}
	return apdu_len;
}
