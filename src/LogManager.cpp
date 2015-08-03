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
#include "Configuration.h"
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
	mOsDef(std::cout.rdbuf()),
	mOsErr(std::cerr.rdbuf())
{
}

LogManager::~LogManager()
throw (Utils::Error)
{
	try {
		mOsDef.flush();
		mOsErr.flush();
	} catch (std::exception& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(LoggerManager));
	}
}

void LogManager::log(LoggerLevel::Type logLevel, const std::string& name, const std::string& value) {
	LoggerLevel::Type currentLevel = LoggerLevel::TRACE;
	try {
		currentLevel = Utils::Configuration::get().logger.level;
	} catch (std::exception& e) {
		log(currentLevel, logLevel, "LogManager", UTILS_STR_FUNCTION + ", error: " + e.what());
	}
	log(currentLevel, logLevel, name, value);
}

void LogManager::log(LoggerLevel::Type logLevelCurrent, LoggerLevel::Type logLevel,
						const std::string& name, const std::string& value)
{
	if (logLevelCurrent >= logLevel) {
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
		getOs(logLevel) << ss.str() << std::endl;
	}
}

std::ostream& LogManager::getOs(LoggerLevel::Type logLevel) {
	switch (logLevel) {
		case LoggerLevel::ERROR:	return mOsErr;
		default:					return mOsDef;
	}
}

} /* namespace Utils */
