/*
 * TestDeviceObject.h
 *
 *  Created on: 19 de Set de 2013
 *      Author: mgf
 */

#ifndef TESTDEVICEOBJECT_H_
#define TESTDEVICEOBJECT_H_

#include "cute.h"
#include "DeviceObject.h"
#include "bacdef.h"
#include "Constants.h"

class TestDeviceObject{
public:
	TestDeviceObject();
	~TestDeviceObject();

	// Tests
	void nothing();
	void testDeviceObjectIdentifier();
	void testDeviceObjectName();
	void testDeviceObjectType();
	void testSystemStatus();
	void testVendorIDs();
	void testModelName();
	void testFirmwareSoftwareRevision();
	void testBacnetProtocolRevision();
	void testProtocolObjectTypesSupported();
	void testObjectList();
	void testMaxAPDULength();
	void testSegmentationSupported();
	void testApduTimeout();
	void testNumberOfApduRetries();
	void testDatabaseRevision();

	void testDeviceCountFunction();
	void testObject_Valid_Object_Instance_Number();
	void testReadPropertyObjectName();
	void testReadPropertyObjectID();
	void testReadPropertyObjectType();
	void testReadPropertySystemStatus();
	void testReadPropertyVendorName();
	void testReadPropertyVendorID();
	void testReadPropertyModelName();
	void testReadPropertyFirmwareRevision();
	void testReadPropertyApplicationSoftwareVersion();
	void testReadPropertyProtocolVersion();
	void testReadPropertyProtocolRevision();
//	void testReadPropertyServicesSupported(); NOTE: Initialized in BacnetNode and tested there
	void testReadPropertyObjectTypesSupported(); //Bug 23
	void testReadPropertyObjectListCount();
	void testReadPropertyObjectListALL();
	void testReadPropertyObjectListIndexExists();
	void testReadPropertyObjectListIndexNonExists();
	void testReadPropertyMaxAPDUlength();
	void testReadPropertySegmentationSupported();
	void testReadPropertyAPDUTimeout();
	void testReadPropertyAPDURetries();
	void testReadPropertyDeviceAddressBinding(); //NOTA: Deve ser testado dps de um I-am. por isso no BacnetNode
	void testReadPropertyDatabaseRevision();
	void testReadPropertyUnimplementedProperty();

private:
	DeviceObject* device;
	uint32_t deviceNumber;
	const char* deviceName;
	BACNET_DEVICE_STATUS deviceStatus;
	void createAPDU(BACNET_READ_PROPERTY_DATA& rpdata, BACNET_APPLICATION_DATA_VALUE& appDataValueIN,
					BACNET_PROPERTY_ID property);
};


#endif /* TESTDEVICEOBJECT_H_ */
