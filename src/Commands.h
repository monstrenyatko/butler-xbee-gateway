/*
 *******************************************************************************
 *
 * Purpose: Asynchronous commands implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

/* Internal Includes */
#include "Command.h"
/* External Includes */
/* System Includes */
#include <memory>
#include <vector>
#include <string>


class CommandApplicationStop: public Utils::Command {
public:
	CommandApplicationStop() {}
	void execute();
private:
};

class CommandSerialInput: public Utils::Command {
public:
	CommandSerialInput(std::unique_ptr< std::vector<uint8_t> > data)
	:mData(std::move(data)) {};
	void execute();
private:
	std::unique_ptr< std::vector<uint8_t> >		mData;
};

class CommandSerialClose: public Utils::Command {
public:
	CommandSerialClose(const std::string& cause)
	:mData(cause) {};
	void execute();
private:
	std::string		mData;
};

#endif /* COMMANDS_H_ */
