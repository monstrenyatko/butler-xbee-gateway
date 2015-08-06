/*
 *******************************************************************************
 *
 * Purpose: Utils. Asynchronous commands invoker implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef UTILS_COMMAND_PROCESSOR_H_
#define UTILS_COMMAND_PROCESSOR_H_

/* Internal Includes */
#include "Thread.h"
#include "Command.h"
#include "Semaphore.h"
#include "Logger.h"
/* External Includes */
/* System Includes */
#include <string>
#include <queue>
#include <mutex>


namespace Utils {

/**
 * Asynchronous Command invoker class.
 */
class CommandProcessor: private Utils::Thread {
public:
	CommandProcessor(const std::string&);

	~CommandProcessor() throw ();

	/**
	 * Starts the processor.
	 */
	void start(void);

	/**
	 * Stops the processor.
	 */
	void stop(void);

	/**
	 * Adds new command for processing.
	 */
	void process(std::unique_ptr<Command>);

private:
	// Objects
	Utils::Logger					mLog;
	std::queue<Command*>			mQueue;
	Utils::Semaphore				mSem;
	std::mutex						mMtx;

	// Do not copy
	CommandProcessor(const CommandProcessor&);
	CommandProcessor &operator=(const CommandProcessor&);

	// Internal
	void onStop();
	void loop();
	void _stop(void);
};

} /* namespace Utils */

#endif /* UTILS_COMMAND_PROCESSOR_H_ */
