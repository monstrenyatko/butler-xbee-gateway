/*
 *******************************************************************************
 *
 * Purpose: Serial port read/write implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

/* Internal Includes */
#include "Error.h"
/* External Includes */
/* System Includes */
#include <stdint.h>
#include <string>
#include <vector>


/* Forward declaration */
struct SerialPortContext;

class SerialPort {
public:
	SerialPort(const std::string& name, uint32_t baud) throw ();
	virtual ~SerialPort() throw ();
	void start() throw (Utils::Error);
	void stop() throw ();
	std::size_t write(const uint8_t* data, uint32_t size) throw (Utils::Error);
private:
	// Objects
	SerialPortContext*				mCtx;

	// Do not copy
	SerialPort(const SerialPort&);
	SerialPort &operator=(const SerialPort&);

	// Internal
};

#endif /* SERIAL_PORT_H_ */
