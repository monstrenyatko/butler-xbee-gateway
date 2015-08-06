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
#include <string>


class CommandApplicationStop: public Utils::Command {
public:
	CommandApplicationStop(const std::string& reason)
	: mReason(reason) {}
	void execute();
private:
	std::string		mReason;
};

#endif /* COMMANDS_H_ */
