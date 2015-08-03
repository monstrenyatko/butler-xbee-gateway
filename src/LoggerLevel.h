/*
 *******************************************************************************
 *
 * Purpose: Utils. Logger Level enumeration.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef LOGGER_LEVEL_H_
#define LOGGER_LEVEL_H_

/* Internal Includes */
#include "Error.h"
/* External Includes */
/* System Includes */
#include <string>


namespace Utils {

struct LoggerLevel {
	typedef enum {
		ERROR,
		WARN,
		INFO,
		DEBUG,
		TRACE,
		QTY
	} Type;

	static std::string toString(LoggerLevel::Type logLevel) {
		switch(logLevel) {
			case ERROR:		return "ERROR";
			case WARN:		return "WARN";
			case INFO:		return "INFO";
			case DEBUG:		return "DEBUG";
			case TRACE:		return "TRACE";
			default:		return "UNKNW";
		}
	}

	static LoggerLevel::Type fromString(const std::string& logLevel)
	throw (Utils::Error)
	{
		if		(logLevel == "ERROR") return ERROR;
		else if	(logLevel == "WARN") return WARN;
		else if	(logLevel == "INFO") return INFO;
		else if	(logLevel == "DEBUG") return DEBUG;
		else if	(logLevel == "TRACE") return TRACE;
		else throw Utils::Error(UTILS_STR_CLASS_FUNCTION(LoggerLevel)
				+ ", wrong value [" + logLevel + "]");
	}
};

} /* namespace Utils */

#endif /* LOGGER_LEVEL_H_ */
