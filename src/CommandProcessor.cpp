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

/* Internal Includes */
#include "CommandProcessor.h"
#include "Error.h"
/* External Includes */
/* System Includes */
#include <iostream>
#include <assert.h>


namespace Utils {

CommandProcessor::CommandProcessor()
:
	mSem(0, 1)
{
}

CommandProcessor::~CommandProcessor() throw () {
	while (!mQueue.empty()) {
		delete mQueue.front();
		mQueue.pop();
	}
}

void CommandProcessor::stop(void) {
	Utils::Thread::stop();
}

void CommandProcessor::start(void)
{
	Utils::Thread::start();
}

void CommandProcessor::onStop() {
	mSem.post();
}

void
CommandProcessor::loop(void)
{
	while (isAlive()) {
		// wait a new command
		mSem.wait();
		mMtx.lock();
		//process all commands
		while (!mQueue.empty()) {
			std::unique_ptr<Command> cmd(mQueue.front());
			mQueue.pop();
			// release mutex to allow adding new commands
			mMtx.unlock();
			try {
				cmd->execute();
			} catch (std::exception &e) {
				std::cerr<<UTILS_STR_CLASS_FUNCTION(CommandProcessor)
						<<", Command::execute, error" << e.what() << std::endl;
			}
			mMtx.lock();
		}
		mMtx.unlock();
	}
}

void
CommandProcessor::process(std::unique_ptr<Command> command)
{
	assert(command.get());
	if (command.get()) {
		std::lock_guard<std::mutex> locker(mMtx);
		mQueue.push(command.get());
		command.release();
		mSem.post();
	}
}

} /* namespace Utils */
