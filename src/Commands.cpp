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
#include "SerialPort.h"
#include "XBeeNet.h"
/* External Includes */
/* System Includes */
#include <iostream>
#include <iomanip>


void CommandApplicationStop::execute() {
	Application::get().stop();
}

void CommandSerialInput::execute() {
	Application::get().getXBeeNet().from(std::move(mData));
}

void CommandSerialOutput::execute() {
	Application::get().getSerial().write(&((*mData)[0]), mData->size());
}

void CommandSerialClose::execute() {
	std::cerr<<UTILS_STR_CLASS_FUNCTION(CommandSerialClose)<<", cause:"<<mData<<std::endl;
	// stop
	std::unique_ptr<Utils::Command> cmd (new CommandApplicationStop);
	Application::get().getProcessor().process(std::move(cmd));
}
