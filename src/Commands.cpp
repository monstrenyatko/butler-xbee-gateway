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

/* Internal Includes */
#include "Commands.h"
#include "Error.h"
#include "Application.h"
#include "CommandProcessor.h"
/* External Includes */
/* System Includes */
#include <iostream>


void CommandApplicationStop::execute() {
	Application::get().stop();
}

void CommandSerialInput::execute() {
	std::cout<<UTILS_STR_CLASS_FUNCTION(CommandSerialInput)<<", data.size:"<<mData->size()<<std::endl;
	for (uint8_t i: *mData) {
		std::cout << "[" << i << "]";
	}
	if (!mData->empty()) {
		std::cout << std::endl;
	}
}

void CommandSerialClose::execute() {
	std::cerr<<UTILS_STR_CLASS_FUNCTION(CommandSerialClose)<<", cause:"<<mData<<std::endl;
	// stop
	std::unique_ptr<Utils::Command> cmd (new CommandApplicationStop);
	Application::get().getProcessor().process(std::move(cmd));
}
