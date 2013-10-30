Bacnet-Energy-Monitor
=====================

This project develops a object oriented approach to the bacnet stack (*BacnetNode library*) with the aim of deploying an energy monitor device that can respond to the bacnet protocol. The arduino platform is the hardware being targeted.

It uses [BACnet Protocol Stack](http://sourceforge.net/projects/bacnet/) and redesigns the objects using OO. 

An application to run in arduino, read a temperature sensor and populate an analog value object is provided. A version that runs on linux is also available for debugging purposes. This software is being developed using a TDD approach and unit tests are provided in folder *source/test*. These use the [CUTE C++ unit testing framework](http://cute-test.com/)

A section with drivers contains the bacnet stack code necessary for BACNET/IP communication using arduino's WI5100 ethernet shield in folder *source/drivers/atmega328p* and linux in folder *source/drivers/linux*.

This project is developed using Eclipse and the toolchain is configured to compile in this platform.