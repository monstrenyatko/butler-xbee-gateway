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
#include "CommandProcessor.h"
#include "Commands.h"
#include "Application.h"
#include "Error.h"
/* External Includes */
/* System Includes */
#include <iostream>


SignalProcessor::SignalProcessor()
:
	mSigSet(mIoService, SIGINT, SIGTERM)
{
}

void SignalProcessor::start(void) {
	Utils::Thread::start();
}

void SignalProcessor::stop(void) {
	Utils::Thread::stop();
}

void SignalProcessor::onStarted() {
	schedule();
}

void SignalProcessor::onStop() {
	try {
		mSigSet.cancel();
		mSigSet.clear();
		mIoService.stop();
		mIoService.reset();
	} catch (boost::system::system_error& e) {
		// ignore
	}
}

void SignalProcessor::schedule() {
	mSigSet.async_wait([this](const boost::system::error_code& a, int b) {
		onSignal(a, b);
	});
}

void SignalProcessor::loop() {
	while (isAlive()) {
		try {
			mIoService.reset();
			mIoService.run_one();
		} catch (boost::system::system_error& e) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}

void SignalProcessor::onSignal(const boost::system::error_code& error, int sigNumber)
{
	if (error==boost::asio::error::operation_aborted) {
		// Signal set is cancelled
	} else {
		// Signal to stop
		std::unique_ptr<Utils::Command> cmd (new CommandApplicationStop);
		Application::get().getProcessor().process(std::move(cmd));
	}
}
