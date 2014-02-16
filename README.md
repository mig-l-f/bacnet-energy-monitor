Bacnet-Energy-Monitor
=====================

# Description

This project develops a object oriented approach to the bacnet stack (*BacnetNode library*) with the aim of deploying an energy monitor device that can respond to the bacnet protocol. The arduino platform is the hardware being targeted.

It uses [BACnet Protocol Stack](http://sourceforge.net/projects/bacnet/) and redesigns the objects using OO. 

## BacnetNode library

Is the c++ OO redesign of bacnet-stack for the application layer. Currently the device and analog value objects are implemented. This library is being developed using a TDD approach and unit tests are provided in folder *source/test*. These use the [CUTE C++ unit testing framework](http://cute-test.com/)

A bacnet node is the aggregation of the several bacnet objects in one entity. This entity must at least be composed of a device object and has to implement the read property handler.

Currently a node with 2 analog values is implemented, named BacnetNode2Thermos with the purpose of encapsulating 2 thermometers.

## Bacnet stack

As described above the open source [BACnet Protocol Stack](http://sourceforge.net/projects/bacnet/) by Steve Karg is used. Two drivers for the arduino platform have been developed:

* A driver for BACNET/IP communication using the RFM12B radio and the [Jeelabs library](http://jeelabs.net/pub/docs/jeelib/md_intro_rf12.html) has physical layer and uIP to handle the TCP/IP stack is in folder *source/drivers/uIP_Rfm12b*

* A driver for BACNET/IP communication using arduino's WI5100 ethernet shield is in folder *source/drivers/atmega328p*

## Aplications

Two main applications have been developed to make use of the BacnetNode and Bacnet stack both to run on the Arduino Uno platform:

* The *ArduinoBacnetAppRfm12b* application that uses the bacnet-stack with the rfm12b/uip driver to handle incoming packets from the RFM12B driver and hand them to the bacnet entity to deal with.

* The *ArduinoBacnetApp* application that uses the bacnet-stack with the Arduino ethernet shield driver.

There are also other experimental and debugging applications.

## Notes

This project is developed using Eclipse and uses the avr-gcc toolchain to compile in this platform.