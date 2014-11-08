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

#ifndef SIGNALPROCESSOR_H_
#define SIGNALPROCESSOR_H_

/* Internal Includes */
#include "Thread.h"
/* External Includes */
/* System Includes */
#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>

class SignalProcessor: private Utils::Thread {
public:
	/**
	 * Constructor
	 */
	SignalProcessor();

	/**
	 * Starts the processor.
	 */
	void start(void);

	/**
	 * Stops the processor.
	 */
	void stop(void);
private:
	// Objects
	boost::asio::io_service							mIoService;
	boost::asio::signal_set							mSigSet;

	// Do not copy
	SignalProcessor(const SignalProcessor&);
	SignalProcessor &operator=(const SignalProcessor&);

	// Internal
	void schedule();
	void loop();
	void onStarted();
	void onStop();
	void onSignal(const boost::system::error_code&, int sigNumber);
};

#endif /* SIGNALPROCESSOR_H_ */
