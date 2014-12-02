XBEE GATEWAY
============

About
=====
- It is a gateway between XBee® ZB and TCP network.
- Gateway assumes that sensor uses MQTT (http://mqtt.org) protocol over XBee.
- MQTT traffic is routed to TCP socket of the public MQTT server (test.mosquitto.org:1883).
- Gateway maintains separate TCP connection for each sensor.

XBee Network
============
- Install XCTU (`http://www.digi.com/products/wireless-wired-embedded-solutions/zigbee-rf-modules/xctu`).
- Use XCTU for installing the firmware to the XBee devices.
- XBee® ZB network must have only one coordinator.

XBee Coordinator
----------------
- Coordinator must have the firmware with `ZigBee Coordinator API` functional set.
- Use XCTU to set `AP` (API mode) option to 0x2 (escapes).
- Use XCTU to set `BD` (Baud Rate) to 57600.

XBee End Devices and Routers
----------------------------
- Devices must have the firmware with `ZigBee End Device` or `ZigBee Router` functional set.
- Use XCTU to set `PAN ID`. It must be equal with Coordinator.
- Use XCTU to set `DH` and `DL` to 0x0 (send data to Coordinator).

Prepare build environment
=========================

Cmake
-----
Install Cmake (http://www.cmake.org) with version 2.8 or later.

C++
---
Install the compiler with C++11 support.

Boost
-----
Install Boost (http://www.boost.org) library with version 1.45 or later.

Building
========

UNIX like OS
------------
- Create a `build directory` for out of source compilation:
```sh
mkdir build
```
- Go to the `build directory`:
```sh
cd build
```
- Use `cmake` to prepare the build:
```sh
cmake <path to sources>
```
- Use `make` to start the build process:
```sh
make
```
- The resulting binary file is located in `<build directory>/bin/`

UNIX like OS + Eclipse
-------------------------
- Create a `build directory` for out of source compilation:
```sh
mkdir build
```
- Go to the `build directory`:
```sh
cd build
```
- Use `cmake` to prepare the build:
```sh
cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug <path to sources>
```
- Import the project from the `build directory` (`File -> Import -> General -> Existing Projects into Workspace`)
- Use `Project -> Build Project` to start the build process

Configuration
=============
To be implemented

Usage
=====
- Connect XBee Coordinator to PC where the XBeeGateway is installed.
- Launch the XBeeGateway.
