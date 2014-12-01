/*
 *******************************************************************************
 *
 * Purpose: Main loop implementation and Global objects container.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef SRC_APPLICATION_H_
#define SRC_APPLICATION_H_

/* Internal Includes */
#include "Error.h"
#include "Semaphore.h"
/* External Includes */
/* System Includes */

/* Forward declaration */
namespace Utils {
	class CommandProcessor;
}
class SerialPort;
class SignalProcessor;
class XBeeNet;
class TcpNet;
class Router;

class Application {
public:
	/**
	 * Initializes the object.
	 * Must be called only once or after destroy().
	 */
	static void initialize() throw (Utils::Error);

	/**
	 * Destroys the object.
	 */
	static void destroy() throw ();

	/**
	 * Gets the access to the object.
	 * Could be used between calls to initialize() and destroy().
	 *
	 * @returns instance of the object.
	 */
	static Application& get() {return *mInstance;}

	/**
	 * Run function for Main thread.
	 * Will be blocked until stop() is called.
	 */
	void run() throw ();

	/**
	 * Stops the run() cycle.
	 */
	void stop() throw ();

	// Available services
	Utils::CommandProcessor&	getProcessor() {return *mProcessor;}
	SerialPort&					getSerial() {return *mSerial;}
	XBeeNet&					getXBeeNet() {return *mXBeeNet;}
	TcpNet&						getTcpNet() {return *mTcpNet;}
	Router&						getRouter() {return *mRouter;}
private:
	// Objects
	static Application*				mInstance;
	Utils::Semaphore				mSem;
	// Services
	Utils::CommandProcessor*		mProcessor;
	SignalProcessor*				mSignalProcessor;
	SerialPort*						mSerial;
	XBeeNet*						mXBeeNet;
	TcpNet*							mTcpNet;
	Router*							mRouter;

	// Do not copy
	Application(const Application&);
	Application &operator=(const Application&);

	// Internal
	Application() throw (Utils::Error);
	~Application() throw ();
};

#endif /* SRC_APPLICATION_H_ */
