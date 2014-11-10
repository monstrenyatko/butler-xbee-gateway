/*
 *******************************************************************************
 *
 * Purpose: Main function.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Application.h"
#include "SerialPort.h"
#include "CommandProcessor.h"
/* External Includes */
/* System Includes */
#include <iostream>
#include <unistd.h>


int main() {
	try {
		Application::initialize();
		Application::get().run();
		Application::destroy();
	} catch (std::exception& e) {
		std::cerr<<"main(), Error: "<<e.what()<<std::endl;
	}
	std::cout<<"EXIT"<<std::endl;
	return 0;
}
