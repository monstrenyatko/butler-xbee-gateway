/*
 *******************************************************************************
 *
 * Purpose: Utils. Logger implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef LOG_MANAGER_H_
#define LOG_MANAGER_H_

/* Internal Includes */
#include "Logger.h"
#include "Error.h"
/* External Includes */
/* System Includes */
#include <ostream>
#include <mutex>
#include <string>

namespace Utils {

/**
 * Processes the log lines
 */
class LogManager {
public:
	/**
	 * Gets the instance
	 */
	static LogManager& get() throw (Utils::Error);

	/**
	 * Destroy the instance If available
	 */
	static void destroy() throw ();

	/**
	 * Handle the log line
	 * @param logLevel the line log level
	 * @param name the logger name
	 * @param value the log line content
	 */
	void log(LoggerLevel::Type logLevel, const std::string& name, const std::string& value);
private:
	static LogManager*			mInstance;
	static std::mutex			mMtxInstance;
	std::mutex					mMtx;
	std::ostream				mStream;

	/**
	 * Constructor
	 */
	LogManager();

	/**
	 * Destructor
	 */
	~LogManager() throw (Utils::Error);
};

} /* namespace Utils */

#endif /* LOG_MANAGER_H_ */
