/*
 *******************************************************************************
 *
 * Purpose: Main loop implementation and Global objects container.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014-2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Application.h"
#include "CommandProcessor.h"
#include "SignalProcessor.h"
#include "SerialPort.h"
#include "XBeeNet.h"
#include "TcpNet.h"
#include "Router.h"
/* External Includes */
/* System Includes */


Application* Application::mInstance = NULL;

void Application::initialize()
throw (Utils::Error)
{
	if(mInstance) {
		throw Utils::Error("Application has been initialized");
	}
	mInstance = new Application();
}

void Application::destroy()
throw (Utils::Error)
{
	try {
		delete mInstance;
	} catch (Utils::Error& e) {
		mInstance = NULL;
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Application));
	} catch (std::exception& e) {
		mInstance = NULL;
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Application));
	}
}

Application& Application::get()
throw (Utils::Error)
{
	if (!mInstance) {
		throw Utils::Error("Application is not initialized");
	}
	return *mInstance;
}

Application::Application()
throw (Utils::Error)
:
	mLog(__FUNCTION__),
	mSem(0, 1),
	mProcessor(NULL),
	mSignalProcessor(NULL),
	mSerial(NULL),
	mXBeeNet(NULL),
	mTcpNet(NULL),
	mRouter(NULL)
{
	std::unique_ptr<Utils::CommandProcessor> ptrProcessor;
	std::unique_ptr<SignalProcessor> ptrSignalProcessor;
	std::unique_ptr<SerialPort> ptrSerial;
	std::unique_ptr<XBeeNet> ptrXBeeNet;
	std::unique_ptr<TcpNet> ptrTcpNet;
	std::unique_ptr<Router> ptrRouter;

	// initialize objects in exception-save mode
	try {
		*mLog.info() << "INITIALIZATION";
		ptrProcessor.reset(new Utils::CommandProcessor(mLog.getName()));
		ptrSignalProcessor.reset(new SignalProcessor);
		ptrSerial.reset(new SerialPort());
		ptrXBeeNet.reset(new XBeeNet());
		ptrTcpNet.reset(new TcpNet());
		ptrRouter.reset(new Router);
	} catch (std::exception& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Application));
	}

	// no exceptions from this point => destructor will control deallocations
	mProcessor = ptrProcessor.release();
	mSignalProcessor = ptrSignalProcessor.release();
	mSerial = ptrSerial.release();
	mXBeeNet = ptrXBeeNet.release();
	mTcpNet = ptrTcpNet.release();
	mRouter = ptrRouter.release();
}

Application::~Application()
throw (Utils::Error)
{
	// stop all services
	*mLog.info() << "STOP";
	try {
		mRouter->stop();
		mTcpNet->stop();
		mXBeeNet->stop();
		mSerial->stop();
		mSignalProcessor->stop();
		mProcessor->stop();
	} catch (std::exception& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Application));
	}

	// clean objects
	*mLog.info() << "DESTROY";
	try {
		delete mRouter;
		delete mTcpNet;
		delete mXBeeNet;
		delete mSerial;
		delete mSignalProcessor;
		delete mProcessor;
	} catch (std::exception& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Application));
	}
}

void Application::run() throw () {
	bool started = false;
	// start all services
	*mLog.info() << "START";
	try {
		mProcessor->start();
		mSignalProcessor->start();
		mSerial->start();
		mXBeeNet->start();
		mTcpNet->start();
		mRouter->start();
		started = true;
	} catch (std::exception& e) {
		*mLog.error() << UTILS_STR_FUNCTION << ", starting, error: " << e.what();
	}
	if (started) {
		*mLog.info() << "PROCESSING";
		// wait request to finish
		mSem.wait();
		*mLog.info() << "FINISHING";
	}
}

void Application::stop(const std::string& reason) throw () {
	*mLog.debug() << UTILS_STR_FUNCTION << ", reason: " << reason;
	mSem.post();
}
