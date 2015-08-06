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
#include <assert.h>


namespace Utils {

CommandProcessor::CommandProcessor(const std::string& name)
:
	Utils::Thread(name + "-CmP"),
	mLog(getName()),
	mSem(0, 1)
{
}

CommandProcessor::~CommandProcessor() throw () {
	_stop();
	while (!mQueue.empty()) {
		delete mQueue.front();
		mQueue.pop();
	}
}

void CommandProcessor::stop(void) {
	*mLog.debug() << UTILS_STR_FUNCTION;
	_stop();
}

void CommandProcessor::_stop(void) {
	Utils::Thread::stop();
}

void CommandProcessor::start(void)
{
	*mLog.debug() << UTILS_STR_FUNCTION;
	Utils::Thread::start();
}

void CommandProcessor::onStop() {
	mSem.post();
}

void CommandProcessor::loop(void)
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
				*mLog.error() << UTILS_STR_FUNCTION
						<<", Command::execute, error: " << e.what();
			}
			mMtx.lock();
		}
		mMtx.unlock();
	}
	*mLog.debug() << UTILS_STR_FUNCTION << ", done";
}

void CommandProcessor::process(std::unique_ptr<Command> command)
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
