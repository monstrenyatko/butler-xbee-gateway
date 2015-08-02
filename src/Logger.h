/*
 *******************************************************************************
 *
 * Purpose: Utils. Logger endpoint implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef LOGGER_H_
#define LOGGER_H_

/* Internal Includes */
#include "LoggerLevel.h"
/* External Includes */
/* System Includes */
#include <stdint.h>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>


namespace Utils {

/* Forward declaration */
class Logger;

/**
 * Logging stream implementation
 */
class LogStream: public std::ostringstream {
public:
	/**
	 * Destructor
	 */
	~LogStream() {flush();}

	/**
	 * Get the logger name
	 *
	 * @return the logger name
	 */
	const std::string& getName() const {return mName;}

	/**
	 * Get the stream log level
	 *
	 * @return the stream log level
	 */
	LoggerLevel::Type getLevel() const {return mLevel;}

	/**
	 * Finish the log line
	 */
	LogStream& flush();

	inline friend LogStream& operator<<(LogStream& logStream, LogStream& (*m)(LogStream&)) {
		return m(logStream);
	}
private:
	const std::string				mName;
	const LoggerLevel::Type			mLevel;

	/**
	 * Constructor
	 *
	 * @param name the logger name
	 * @param level the stream log level
	 */
	LogStream(const std::string& name, LoggerLevel::Type level)
	: mName(name), mLevel(level) {}

	// Do not copy
	LogStream(const LogStream&);
	LogStream &operator=(const LogStream&);

	friend class Logger;
};

class Logger: public std::ostringstream {
public:
	/**
	 * Constructor
	 *
	 * @param name the logger name
	 */
	Logger(const std::string &name)
		: mName(name) {}

	/* Logging stream generators */

	std::unique_ptr<LogStream> trace() const
		{return std::move(std::unique_ptr<LogStream>(new LogStream(getName(), LoggerLevel::TRACE)));}

	std::unique_ptr<LogStream> debug() const
		{return std::move(std::unique_ptr<LogStream>(new LogStream(getName(), LoggerLevel::DEBUG)));}

	std::unique_ptr<LogStream> info() const
		{return std::move(std::unique_ptr<LogStream>(new LogStream(getName(), LoggerLevel::INFO)));}

	std::unique_ptr<LogStream> warn() const
		{return std::move(std::unique_ptr<LogStream>(new LogStream(getName(), LoggerLevel::WARN)));}

	std::unique_ptr<LogStream> error() const
		{return std::move(std::unique_ptr<LogStream>(new LogStream(getName(), LoggerLevel::ERROR)));}

	/**
	 * Get the logger name
	 *
	 * @return the logger name
	 */
	const std::string& getName() const {return mName;}
private:
	std::string				mName;
};

/* Log Stream Manipulators */

/**
 * Force the log line to finish
 */
inline LogStream& endLog(LogStream& os) {
	return os.flush();
}

/**
 * Print Array content (helper class)
 */
class __iom__put_array {
public:
	inline explicit __iom__put_array(const std::vector<uint8_t>& data)
		: mData(data) {}

	inline friend std::ostream& operator<<(std::ostream& os, const __iom__put_array& data)
	{
		// save flags
		std::ios::fmtflags f(os.flags());
		// print
		for (auto i = data.mData.begin(); i!=data.mData.end(); i++) {
			os << std::uppercase << std::hex << std::setw(2) << std::setfill('0')
				<< (int)(*i);
			if (i+1 != data.mData.end()) {
				os << " ";
			}
		}
		// revert flags
		os.flags(f);
		return os;
	}
private:
	const std::vector<uint8_t>&		mData;
};

/**
 * Print Array content in HEX mode with 'XX' format
 */
inline __iom__put_array putArray(const std::vector<uint8_t>& data) {
	return __iom__put_array(data);
};

/**
 * Overload operator '<<' for custom manipulators.
 * If the 'os' is not an instance of 'LogStream' Do nothing.
 */
inline std::ostream& operator<<(std::ostream& os, LogStream& (*m)(LogStream&)) {
	LogStream* logStream = dynamic_cast<LogStream*>(&os);
	if (logStream) {
		return m(*logStream);
	} else {
		return os;
	}
}

} /* namespace Utils */

#endif /* LOGGER_H_ */
