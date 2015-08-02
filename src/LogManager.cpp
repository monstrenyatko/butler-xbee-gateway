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

/* Internal Includes */
#include "LogManager.h"
/* External Includes */
/* System Includes */
#include <boost/date_time.hpp>
#include "boost/format.hpp"


namespace Utils {

#define LOG_MANAGER_LINE_PREFIX ">>>> "
#define LOG_MANAGER_LINE_DELIMITER " - "
LogManager*			LogManager::mInstance = NULL;
std::mutex			LogManager::mMtxInstance;

std::ostream& putTime(std::ostream& out)
{
	boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
	out << boost::posix_time::to_iso_extended_string(now);
	return out;
}

LogManager& LogManager::get()
throw (Utils::Error)
{
	std::lock_guard<std::mutex> locker(mMtxInstance);
	if (!mInstance)
	{
		mInstance = new LogManager();
		if (!mInstance) {
			throw Utils::Error("LoggerManager is not initialized");
		}
	}
	return *mInstance;
}

void LogManager::destroy()
throw ()
{
	try {
		std::lock_guard<std::mutex> locker(mMtxInstance);
		if (mInstance)
		{
			LogManager* tmp = mInstance;
			mInstance = NULL;
			delete tmp;
		}
	} catch (std::exception& e) {
		std::cerr
		<< LOG_MANAGER_LINE_PREFIX
		<< putTime
		<< LOG_MANAGER_LINE_DELIMITER
		<< UTILS_STR_FUNCTION << ", error: " << e.what() << std::endl;
	}
}

LogManager::LogManager()
:
	mStream(std::cout.rdbuf())
{

}

LogManager::~LogManager()
throw (Utils::Error)
{
	try {
		mStream.flush();
	} catch (std::exception& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(LoggerManager));
	}
}

void LogManager::log(LoggerLevel::Type logLevel, const std::string& name, const std::string& value) {
	std::lock_guard<std::mutex> locker(mMtx);
	// Format string
	std::stringstream ss;
	ss
	<< LOG_MANAGER_LINE_PREFIX
	<< putTime
	<< LOG_MANAGER_LINE_DELIMITER
	<< boost::format("[%-5s]") % LoggerLevel::toString(logLevel)
	<< " : "
	<< name
	<< LOG_MANAGER_LINE_DELIMITER
	<< value;
	// Output
	mStream << ss.str() << std::endl;
}

} /* namespace Utils */
