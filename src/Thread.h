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

#ifndef UTILS_THREAD_H_
#define UTILS_THREAD_H_

/* Internal Includes */
#include "Atomic.h"
/* External Includes */
/* System Includes */
#include <thread>


namespace Utils {

class Thread {
public:
	virtual ~Thread();
protected:
	/**
	 * Please inherit
	 */
	Thread();
	/**
	 * Starts the thread
	 */
	void start(void);

	/**
	 * Stops the thread
	 */
	void stop(void);

	/**
	 * Check if thread could be working.
	 * See loop() function.
	 *
	 * @returns true  - if thread started;
	 *          false - if thread is not started or must be stopped;
	 */
	bool isAlive() {return mIsAlive;}
private:
	// Objects
	Utils::atomic_bool				mIsAlive;
	std::thread						mWorker;

	// Do not copy
	Thread(const Thread&);
	Thread &operator=(const Thread&);

	// Internal
	/**
	 * Function is called by thread.
	 *
	 * Common implementation is:
	 * while(isAlive()) {
	 *   <actions>
	 * }
	 */
	virtual void loop() = 0;

	/**
	 * Function is called just before starting the thread
	 */
	virtual void onStart() {}

	/**
	 * Function is called after starting the thread
	 */
	virtual void onStarted() {}

	/**
	 * Function is called on stopping the thread just before join
	 */
	virtual void onStop() {}
};

} /* namespace Utils */

#endif /* UTILS_THREAD_H_ */
