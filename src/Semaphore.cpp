/*
 *******************************************************************************
 *
 * Purpose: Utils. Classic semaphore implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Semaphore.h"
/* External Includes */
/* System Includes */

namespace Utils {

Semaphore::Semaphore(uint32_t initialCount, uint32_t maximumCount)
:
	mCountMax(maximumCount),
	mCount(initialCount)
{
}

void Semaphore::wait()
{
	std::unique_lock<std::mutex> locker(mMtx);
	mCond.wait(locker, [this]{ return mCount > 0; });
	mCount--;
}

bool Semaphore::try_wait()
{
	std::lock_guard<std::mutex> locker(mMtx);
	bool res = (mCount > 0);
	if (res) {
		mCount--;
	}
	return res;
}

void Semaphore::post()
{
	std::lock_guard<std::mutex> locker(mMtx);
	if (mCount < mCountMax) {
		mCount++;
		mCond.notify_one();
	}
}

} /* namespace Utils */
