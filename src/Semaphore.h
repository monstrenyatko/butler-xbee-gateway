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

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

/* Internal Includes */
/* External Includes */
/* System Includes */
#include <stdint.h>
#include <limits>
#include <mutex>
#include <condition_variable>

namespace Utils {

/**
 * Classic semaphore implementation.
 */
class Semaphore {
public:
	/**
	 * Constructor.
	 *
	 * @param initialCount Initial semaphore value.
	 * @param maximumCount Maximum semaphore value.
	 */
	explicit Semaphore(uint32_t initialCount = 0, uint32_t maximumCount = std::numeric_limits<uint32_t>::max());

	/**
	 * If counter is not zero Decrements semaphore or Blocks the caller otherwise.
	 */
	void wait();

	/**
	 * If counter is not zero Decrements semaphore or returns immediately wo blocking the caller otherwise.
	 *
	 * @returns true - if counter was decremented successfully.
	 */
	bool try_wait();

	/**
	 * Increments semaphore counter and Unblocks a waiting caller.
	 */
	void post();

private:
	const uint32_t mCountMax;
	uint32_t mCount;
	std::mutex mMtx;
	std::condition_variable mCond;
};

} /* namespace Utils */

#endif /* SEMAPHORE_H_ */
