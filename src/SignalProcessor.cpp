/*
 *******************************************************************************
 *
 * Purpose: Processor of the system signals.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "SignalProcessor.h"
#include "Application.h"
#include "Error.h"
#include "Thread.h"
#include "Logger.h"
#include "LogManager.h"
/* External Includes */
/* System Includes */
#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>


///////////////////// SignalListener /////////////////////
class SignalListener: private Utils::Thread {
public:
	SignalListener():
		Utils::Thread(__FUNCTION__),
		mLog(getName()),
		mSigSet(mIoService, SIGHUP, SIGINT, SIGTERM)
	{}

	~SignalListener() {
		stop();
	}

	void start(void) {
		Utils::Thread::start();
	}

	void stop(void) {
		Utils::Thread::stop();
	}
private:
	// Objects
	Utils::Logger									mLog;
	boost::asio::io_service							mIoService;
	boost::asio::signal_set							mSigSet;

	// Methods
	void onStarted() {
		schedule();
	}

	void onStop() {
		boost::system::error_code ec;
		mSigSet.cancel(ec);
		mSigSet.clear(ec);
		mIoService.stop();
	}

	void schedule() {
		mSigSet.async_wait([this](const boost::system::error_code& a, int b) {
			onSignal(a, b);
		});
	}

	void loop() {
		boost::asio::io_service::work work(mIoService);
		while (isAlive()) {
			boost::system::error_code ec;
			mIoService.reset();
			if (!mIoService.run(ec)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
		*mLog.debug() << UTILS_STR_FUNCTION << ", done";
	}

	void onSignal(const boost::system::error_code& error, int sigNumber)
	{
		if (error == boost::asio::error::operation_aborted) {
			// Signal set is cancelled
		} else {
			*mLog.debug() << UTILS_STR_FUNCTION << ", value: " << sigNumber;
			bool reschedule = true;
			switch(sigNumber) {
				case SIGHUP:
				{
					Utils::LogManager::get().rotate();
				}
					break;
				case SIGINT:
				case SIGTERM:
				{
					// Do not catch signals anymore
					reschedule = false;
					// Signal to stop
					Application::get().stop("Signal to stop");
				}
					break;
				default:
					*mLog.warn() << UTILS_STR_FUNCTION << ", not handled, value: " << sigNumber;
					break;
			}
			if (reschedule) {
				schedule();
			}
		}
	}
};

///////////////////// SignalProcessor /////////////////////
SignalProcessor::SignalProcessor()
:
	mSignalListener(new SignalListener)
{
}

SignalProcessor::~SignalProcessor()
throw()
{
	stop();
	delete mSignalListener;
}

void SignalProcessor::start(void) {
	mSignalListener->start();
}

void SignalProcessor::stop(void) {
	mSignalListener->stop();
}
