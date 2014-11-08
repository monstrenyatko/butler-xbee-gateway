/*
 *******************************************************************************
 *
 * Purpose: Utils. Interface for asynchronous commands.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef UTILS_COMMAND_H_
#define UTILS_COMMAND_H_

/* Internal Includes */
/* External Includes */
/* System Includes */


namespace Utils {

/**
 * Interface of the Asynchronous Command.
 */
class Command {
public:
	Command() {
	};

	virtual ~Command() {
	};

	/**
	 * Function is called by CommandProcessor.
	 */
	virtual void execute() = 0;
protected:
};

} /* namespace Utils */

#endif /* UTILS_COMMAND_H_ */
