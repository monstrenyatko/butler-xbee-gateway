/*
 *******************************************************************************
 *
 * Purpose: Serial port read/write implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014-2015.
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
#include <memory>


/* Forward declaration */
struct SerialPortContext;

class SerialPort {
public:
	/**
	 * Constructor
	 */
	SerialPort() throw ();

	/**
	 * Destructor
	 */
	virtual ~SerialPort() throw ();

	/**
	 * Starts the processing
	 */
	void start() throw (Utils::Error);

	/**
	 * Stops the processing
	 */
	void stop() throw ();

	/**
	 * Writes data to serial
	 *
	 * @param buffer data to be written
	 */
	void  write(std::unique_ptr< std::vector<uint8_t> > buffer) throw ();
private:
	// Objects
	SerialPortContext*				mCtx;

	// Do not copy
	SerialPort(const SerialPort&);
	SerialPort &operator=(const SerialPort&);

	// Internal
};

#endif /* SERIAL_PORT_H_ */
