/*
 *******************************************************************************
 *
 * Purpose: Utils. Threads helper.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Thread.h"
/* External Includes */
/* System Includes */


namespace Utils {

Thread::Thread(const std::string& name)
:
	mName(name),
	mIsAlive(false)
{
}

Thread::~Thread() {
}

void Thread::start(void) {
	bool expected = false;
	if (mIsAlive.compare_exchange_strong(expected, true)) {
		onStart();
		// start loop
		mWorker = std::thread(&Thread::loop, this);
		// loop is started
		onStarted();
	}
}

void Thread::stop(void) {
	bool expected = true;
	if (mIsAlive.compare_exchange_strong(expected, false)) {
		onStop();
		mWorker.join();
	}
}

} /* namespace Utils */
