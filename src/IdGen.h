/*
 *******************************************************************************
 *
 * Purpose: Utils. Unique ID generator.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef UTILS_ID_GEN_H_
#define UTILS_ID_GEN_H_

/* Internal Includes */
#include "Id.h"
#include "Atomic.h"
/* External Includes */
/* System Includes */


namespace Utils {

/**
 * Thread-Safe Unique ID Generator.
 */
class IdGen {
public:
	/**
	 * Constructor
	 */
	IdGen(): mId(0) {}

	/**
	 * Generates next unique ID value.
	 *
	 * @return unique ID
	 */
	Id get() { return mId++;}
private:
	atomic_uint32_t mId;
};

} /* namespace Utils */

#endif /* UTILS_ID_GEN_H_ */
