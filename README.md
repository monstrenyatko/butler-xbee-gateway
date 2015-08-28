BUTLER-XBEE-GATEWAY
===================

About
=====
- It is a gateway between `XBee® ZigBee` and `TCP` network for `BUTLER` smart house framework.
- Gateway extracts data from `XBee® ZigBee` frames and sends to the `TCP` Server.
- Gateway maintains separate `TCP` connection for each `XBee® ZigBee` device.

In default configuration/example:
- Gateway assumes that sensor uses `MQTT`(http://mqtt.org) protocol over `XBee® ZigBee`.
- `MQTT` traffic is routed to `TCP` socket of the configured `MQTT Broker`.
- `Arduino` board is used as sensor (See https://github.com/monstrenyatko/butler-arduino-sensor)

Purpose
=======
Usually microcontrollers like `Arduino` doesn't have network stack.
If you use a `MQTT` library to communicate with broker you still need network stack.
The only way to communicate with world is a `Serial`. `Serial` is simple and really popular.
A lot of RF modules do work via `Serial` interface.
Unfortunately on other side you get `Serial` as well in case of `XBee® ZigBee`.
`MQTT` broker doesn't support `Serial` connection directly and we need to pack data from `Serial` to `TCP`:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<b>`Arduino` --> `Serial` --> `RF` <---z---> `RF` --> `Serial` --> `xbee-gateway` --> `TCP` --> `MQTT Broker`</b>

Usage
=====
- Connect `XBee® ZigBee Coordinator` to PC where the application is installed.
- Launch the application.
- Do not forget to power ON all nodes with `XBee® ZigBee` wireless interfaces.

XBee® ZigBee Network Configuration
==================================
- Install `XCTU`(http://www.digi.com/products/wireless-wired-embedded-solutions/zigbee-rf-modules/xctu).
- Use `XCTU` for installing the firmware to the `XBee®` devices.
- `XBee® ZigBee` network must have only one `Coordinator`.
- `XBee® ZigBee Coordinator` must be connected to the `PC` were you plan to run the application.
- All other devices in the network must be `XBee® ZigBee End Device` or `XBee® ZigBee Router`.

XBee® ZigBee Coordinator
------------------------
- Coordinator must have the firmware with `ZigBee Coordinator API` functional set.
- Use XCTU to set `AP` (API mode) option to 0x2 (escapes).
- Use XCTU to set `BD` (Baud Rate) to `57600`.

XBee® ZigBee End Device and XBee® ZigBee Router
-----------------------------------------------
- Devices must have the firmware with `ZigBee End Device` or `ZigBee Router` functional set.
- Use XCTU to set `PAN ID`. It must be equal with Coordinator.
- Use XCTU to set `DH` and `DL` to 0x0 (send data to Coordinator).

Prepare build environment
=========================

Cmake
-----
Install `Cmake`(http://www.cmake.org) with version `2.8` or later.

C++
---
Install the compiler with `C++11` support.

Boost
-----
Install `Boost`(http://www.boost.org) library with version `1.45` or later.

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
- Use `Cmake` to prepare the build:
```sh
cmake <path to sources>
```
- Use `make` to start the build process:
```sh
make
```
- The resulting binary file is located in `<build directory>/bin/`

UNIX like OS + Eclipse
----------------------
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

Raspberry Pi (cross compilation)
--------------------------------
- Download or build your own `GCC` based toolchain for `ARM Linux` with hardware floating point support.
<br/>See http://crosstool-ng.org
- Install all dependencies to the toolchain `rootfs`.<br/>
A lot of prebuilt libraries are available on official repository (http://archive.raspbian.org/raspbian).
Just unpack files to `rootfs`.
- Create a `build directory` for out of source compilation:
```sh
mkdir build
```
- Go to the `build directory`:
```sh
cd build
```
- Configure environment:
```sh
export RASPBERRY_PI_TOOLCHAIN_PREFIX=arm-raspberry-linux-gnueabihf-
export RASPBERRY_PI_TOOLCHAIN=$HOME/raspberrypi/toolchain
export RASPBERRY_PI_ROOTFS=$HOME/raspberrypi/rootfs
```
- Use `Cmake` to prepare the build:
```sh
cmake -D CMAKE_TOOLCHAIN_FILE=build/cmake/Toolchain/raspberrypi.cmake -D CMAKE_BUILD_TYPE=Release \
      -D PACKAGE_SYSTEM_NAME=raspbian <path to sources>
```
- Use `make` to start the build process:
```sh
make
```
- The resulting binary file is located in `<build directory>/bin/`
- Use `make` to build `deb` package:
```sh
make package
```
- The resulting `deb` file is located in `<build directory>`

Configuration
=============
Configuration file is done in `JSON`(http://www.json.org) format.
######Example:
```json
{
	"logger":{
		"level":"INFO"
	},
	"serial":{
		"name":"/dev/usbserial",
		"baud": 57600
	},
	"tcp":{
		"address":"test.mosquitto.org",
		"port": 1883
	}
}
```

Logger
------
Application logger settings.
##### Block name
`logger`
##### Parameters:
###### level (String)
Set verbose level.
<table>
	<tr>
		<td><b>Value</b></td>
		<td><b>Description</b></td>
	</tr>
	<tr>
		<td>"ERROR"</td>
		<td>Prints only errors</td>
	</tr>
	<tr>
		<td>"WARN"</td>
		<td>ERROR level + warnings</td>
	</tr>
	<tr>
		<td>"INFO"</td>
		<td>WARN level + processing information</td>
	</tr>
	<tr>
		<td>"DEBUG"</td>
		<td>INFO level + debug information</td>
	</tr>
	<tr>
		<td>"TRACE"</td>
		<td>DEBUG level + messages content</td>
	</tr>
</table>

Serial
------
Serial port settings.
##### Block name
`serial`
##### Parameters:
###### name (String)
Path to serial port device like `"/dev/usbserial"`.
###### baud (Number)
Serial port baud rate like `57600`.

TCP
---
`TCP` connection settings.
##### Block name
`tcp`
##### Parameters:
###### address (String)
Server address like `"test.mosquitto.org"`
###### port (Number)
Server port like `1883`
