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
#include <fstream>
#include <queue>

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


	/**
	 *	Starts logger.
	 *	Must be called when configuration is read.
	 */
	void start();

	/**
	 * Reopens log file
	 */
	void rotate() throw ();
private:
	struct State {
		typedef enum {
			INIT,
			OK,
			EXIT
		} Type;
	};

	struct Record {
		LoggerLevel::Type		logLevel;
		std::string				time;
		std::string				name;
		std::string				value;

		Record(LoggerLevel::Type logLevel_, const std::string& time_,
				const std::string& name_, const std::string& value_)
		: logLevel(logLevel_), time(time_), name(name_), value(value_) {}
	};

	static LogManager*			mInstance;
	static std::mutex			mMtxInstance;
	std::recursive_mutex		mMtx;
	State::Type					mState;
	std::queue<Record>			mBuffer;
	std::fstream				mFileDef;
	std::ostream				mOsCoutDef;
	std::ostream				mOsCoutErr;
	std::ostream				mOsFileDef;

	/**
	 * Constructor
	 */
	LogManager();

	/**
	 * Destructor
	 */
	~LogManager() throw (Utils::Error);

	void log(LoggerLevel::Type logLevelCurrent, LoggerLevel::Type logLevel,
			const std::string& time, const std::string& name, const std::string& value);
	void log(LoggerLevel::Type logLevel, const std::string& value);
	std::ostream& getOs(LoggerLevel::Type logLevel);
	void prepareOs();
	LoggerLevel::Type getLogLevelCurrent();
	void setState(State::Type state);
	State::Type getState() {return mState;}
	static std::string getTime();
	void flushBuffer();
	void openFile();
	void reopenFile();

	// Do not copy
	LogManager(const LogManager&);
	LogManager &operator=(const LogManager&);
};

} /* namespace Utils */

#endif /* LOG_MANAGER_H_ */
