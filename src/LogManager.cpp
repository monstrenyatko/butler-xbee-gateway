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

std::string LogManager::getTime()
{
	boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
	return boost::posix_time::to_iso_extended_string(now);
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
		<< getTime()
		<< LOG_MANAGER_LINE_DELIMITER
		<< UTILS_STR_FUNCTION << ", error: " << e.what() << std::endl;
	}
}

LogManager::LogManager()
:
	mState(State::INIT),
	mOsCoutDef(std::cout.rdbuf()),
	mOsCoutErr(std::cerr.rdbuf()),
	mOsFileDef(mFileDef.rdbuf())
{
}

LogManager::~LogManager()
throw (Utils::Error)
{
	std::lock_guard<std::recursive_mutex> locker(mMtx);
	try {
		setState(State::EXIT);
		flushBuffer();
		mOsCoutDef.flush();
		mOsCoutErr.flush();
		mOsFileDef.flush();
	} catch (std::exception& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(LoggerManager));
	}
}

void LogManager::start() {
	std::lock_guard<std::recursive_mutex> locker(mMtx);
	prepareOs();
	setState(State::OK);
	flushBuffer();
}

void LogManager::log(LoggerLevel::Type logLevel, const std::string& name, const std::string& value) {
	std::lock_guard<std::recursive_mutex> locker(mMtx);
	switch(getState()) {
		case State::OK:
		case State::EXIT:
		{
			log(getLogLevelCurrent(), logLevel, getTime(), name, value);
		}
			break;
		default:
		{
			mBuffer.push(Record(logLevel, getTime(), name, value));
		}
			break;
	}
}

void LogManager::rotate()
throw ()
{
	std::lock_guard<std::recursive_mutex> locker(mMtx);
	try
	{
		log(LoggerLevel::INFO, "LogManager", "Rotating");
		{
			reopenFile();
		}
	} catch (std::exception& e) {
		log(LoggerLevel::ERROR, "LogManager", UTILS_STR_FUNCTION + ", error: " + e.what());
	}
}

void LogManager::log(LoggerLevel::Type logLevelCurrent, LoggerLevel::Type logLevel,
		const std::string& time, const std::string& name, const std::string& value)
{
	if (logLevelCurrent >= logLevel) {
		// Format string
		std::stringstream ss;
		ss
		<< LOG_MANAGER_LINE_PREFIX
		<< time
		<< LOG_MANAGER_LINE_DELIMITER
		<< boost::format("[%-5s]") % LoggerLevel::toString(logLevel)
		<< " : "
		<< name
		<< LOG_MANAGER_LINE_DELIMITER
		<< value;
		// Output
		log(logLevel, ss.str());
	}
}

void  LogManager::log(LoggerLevel::Type logLevel, const std::string& value)
{
	getOs(logLevel) << value << std::endl;
}

std::ostream& LogManager::getOs(LoggerLevel::Type logLevel) {
	switch (logLevel) {
		case LoggerLevel::ERROR:	return mFileDef.is_open() ? mOsFileDef : mOsCoutErr;
		default:					return mFileDef.is_open() ? mOsFileDef : mOsCoutDef;
	}
}

void LogManager::prepareOs() {
	try {
		openFile();
	} catch (std::exception& e) {
		log(LoggerLevel::ERROR, "LogManager", UTILS_STR_FUNCTION + ", error: " + e.what());
	}
}

LoggerLevel::Type LogManager::getLogLevelCurrent() {
	LoggerLevel::Type res = LoggerLevel::TRACE;
	try {
		res = Utils::Configuration::get().logger.level;
	} catch (std::exception& e) {
		log(res, LoggerLevel::ERROR, getTime(), "LogManager", UTILS_STR_FUNCTION + ", error: " + e.what());
	}
	return res;
}

void LogManager::setState(State::Type state) {
	mState = state;
}

void LogManager::flushBuffer() {
	while(!mBuffer.empty()) {
		log(getLogLevelCurrent(), mBuffer.front().logLevel,
			mBuffer.front().time, mBuffer.front().name, mBuffer.front().value);
		mBuffer.pop();
	}
}

void LogManager::openFile() {
	try {
		std::string name(Utils::Configuration::get().logger.file);
		if (!name.empty()) {
			mFileDef.open(name, std::ofstream::out | std::ofstream::app);
		}
	} catch (std::exception& e) {
		log(LoggerLevel::ERROR, "LogManager", UTILS_STR_FUNCTION + ", error: " + e.what());
	}
}

void LogManager::reopenFile() {
	try {
		if (mFileDef.is_open()) {
			mFileDef.close();
		}
		openFile();
	} catch (std::exception& e) {
		log(LoggerLevel::ERROR, "LogManager", UTILS_STR_FUNCTION + ", error: " + e.what());
	}
}

} /* namespace Utils */
