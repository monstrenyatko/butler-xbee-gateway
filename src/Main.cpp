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

//#include "XBeeNet.h"
//#include "XBeeFrame.h"
//class TestThread: public Utils::Thread {
//public:
//	void start(void)
//	{
//		Utils::Thread::start();
//	}
//
//	void stop(void) {
//		Utils::Thread::stop();
//	}
//
//	void loop() {
//		while (isAlive()) {
//			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//			std::unique_ptr<XBeeBuffer> newData (new XBeeBuffer);
//			*newData = {1,2,3,4,5,6,7,8,9,10,0x11,12,0x13};
//			Application::get().getXBeeNet().to(0x0013A20040C04E4F,std::move(newData));
//		}
//	}
//};

int main() {
	//TestThread testThread;
	try {
		Application::initialize();
		//testThread.start();
		Application::get().run();
		//testThread.stop();
		Application::destroy();
	} catch (std::exception& e) {
		std::cerr<<"main(), Error: "<<e.what()<<std::endl;
	}
	std::cout<<"EXIT"<<std::endl;
	return 0;
}
