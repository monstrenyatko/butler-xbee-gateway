/*
 *******************************************************************************
 *
 * Purpose: Utils. Application command-line options implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

/* Internal Includes */
#include "Error.h"
#include "Logger.h"
/* External Includes */
/* System Includes */
#include <mutex>

namespace Utils {

class Options {
public:
	/**
	 * Gets the instance
	 */
	static Options& get() throw (Utils::Error);

	/**
	 * Destroy the instance If available
	 */
	static void destroy() throw ();

	/**
	 * Process options and populate 'Configuration'
	 */
	void process(int argc, char* argv[]) throw (Utils::Error);
private:
	static Options*					mInstance;
	static std::mutex				mMtxInstance;
	Utils::Logger					mLog;

	// singleton class
	Options();
	~Options() throw () {};

	// Do not copy
	Options(const Options&);
	Options &operator=(const Options&);
};

} /* namespace Utils */

#endif /* OPTIONS_H_ */
