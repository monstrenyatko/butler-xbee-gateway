/*
 *******************************************************************************
 *
 * Purpose: Serial port asynchronous commands interfaces.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef SERIAL_PORT_COMMAND_H_
#define SERIAL_PORT_COMMAND_H_

/* Internal Includes */
#include "Error.h"
#include "Command.h"
/* External Includes */
/* System Includes */
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

/* Forward declaration */
struct SerialPortContext;

class SerialPortCommand: public Utils::Command {
public:
	SerialPortCommand() {}
	virtual ~SerialPortCommand() {}
};

class SerialPortCommandWrite: public SerialPortCommand {
public:
	SerialPortCommandWrite(std::unique_ptr< std::vector<uint8_t> > data)
	:
		mData(std::move(data))
	{}
	void execute();
private:
	std::unique_ptr< std::vector<uint8_t> >		mData;
};

class SerialPortCommandClosed: public SerialPortCommand {
public:
	SerialPortCommandClosed(const std::string& cause)
	:
		mCause(cause) {};
	void execute();
private:
	std::string									mCause;
};

#endif /* SERIAL_PORT_COMMAND_H_ */
