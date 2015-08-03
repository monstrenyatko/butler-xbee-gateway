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
#include "LoggerLevel.h"
#include "Error.h"
/* External Includes */
/* System Includes */
#include <mutex>
#include <string>
#include <ostream>

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
	std::ostream				mOsDef;
	std::ostream				mOsErr;

	/**
	 * Constructor
	 */
	LogManager();

	/**
	 * Destructor
	 */
	~LogManager() throw (Utils::Error);

	void log(LoggerLevel::Type logLevelCurrent, LoggerLevel::Type logLevel,
							const std::string& name, const std::string& value);
	std::ostream& getOs(LoggerLevel::Type logLevel);

	// Do not copy
	LogManager(const LogManager&);
	LogManager &operator=(const LogManager&);
};

} /* namespace Utils */

#endif /* LOG_MANAGER_H_ */
