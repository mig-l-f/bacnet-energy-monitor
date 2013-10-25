/*
 * AnalogValue.h
 *
 *  Created on: 24 de Set de 2013
 *      Author: mgf
 */

#ifndef ANALOGVALUE_H_
#define ANALOGVALUE_H_

#include "BacnetObject.h"
#include "bacenum.h"
#include "bacstr.h"
#include "bacdcode.h"

class AnalogValue : public BacnetObject {
private:
	float Present_Value;
	BACNET_CHARACTER_STRING Description;
	BACNET_EVENT_STATE Event_State;
	BACNET_RELIABILITY Reliability;
	bool Out_Of_Service;
	BACNET_ENGINEERING_UNITS Units;
	BACNET_BIT_STRING Status_Flags;

public:
	AnalogValue(const uint32_t objectID, const char* objectName, const char* description = "NOTDEF", BACNET_ENGINEERING_UNITS units = UNITS_PERCENT);
	~AnalogValue();
	float getPresentValue();
	const char* getDescription();
	bool setDescription(const char* description);
	BACNET_BIT_STRING getStatusFlags();
	BACNET_EVENT_STATE getEventState();
	void setEventState(BACNET_EVENT_STATE state);
	BACNET_RELIABILITY getReliability();
	bool setReliability(BACNET_RELIABILITY reliability);
	bool isOutOfService();
	BACNET_ENGINEERING_UNITS getUnits();
	void setPresentValue(float value);

	//FIXME: To be handled
	virtual unsigned getCount() const;
	virtual bool getValid_Object_Instance_Number(uint32_t object_id);
	virtual int Object_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata);
};

inline unsigned AnalogValue::getCount() const {
	return 1;
}

inline bool AnalogValue::getValid_Object_Instance_Number(uint32_t object_id){
	return getObjectIdentifier()->instance == object_id;
}

#endif /* ANALOGVALUE_H_ */
