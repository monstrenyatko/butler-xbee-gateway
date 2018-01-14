/*
 *******************************************************************************
 *
 * Purpose: Utils. Configuration loading implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Configuration.h"
#include "ConfigurationImpl.h"
#include "Error.h"
#include "Logger.h"
/* External Includes */
/* System Includes */
#include <fstream>
#include <streambuf>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

namespace Utils {

Configuration*			ConfigurationImpl::mInstance = NULL;
std::mutex				ConfigurationImpl::mMtxInstance;
Utils::Logger			ConfigurationImpl::mLog("Configuration");

Configuration& ConfigurationImpl::get()
throw (Utils::Error)
{
	std::lock_guard<std::mutex> locker(mMtxInstance);
	if (!mInstance)
	{
		mInstance = new Configuration();
		if (!mInstance) {
			throw Utils::Error("Configuration is not initialized");
		}
	}
	return *mInstance;
}

void ConfigurationImpl::destroy()
throw ()
{
	try {
		std::lock_guard<std::mutex> locker(mMtxInstance);
		if (mInstance)
		{
			Configuration* tmp = mInstance;
			mInstance = NULL;
			delete tmp;
		}
	} catch (std::exception& e) {
		// It must not happen
		// We can't use logger When the instance is destroyed => ignore
	}
}

void ConfigurationImpl::load()
throw (Utils::Error)
{
	try {
		*ConfigurationImpl::mLog.info() << "Loading from file, path: " << get().main.configFileName;
		// read and pars configuration file
		boost::property_tree::ptree config;
		boost::property_tree::read_json(get().main.configFileName, config);
		// get parameters
		try {
			{
				std::string value(config.get<std::string>("logger.level",
						LoggerLevel::toString(get().logger.level)));
				boost::to_upper(value);
				get().logger.level = LoggerLevel::fromString(value);
			}
			get().logger.file = config.get<std::string>("logger.file", get().logger.file);
			get().serial.name = config.get<std::string>("serial.name");
			get().serial.baud = config.get<uint32_t>("serial.baud");
			get().tcp.address = config.get<std::string>("tcp.address");
			get().tcp.port = config.get<uint32_t>("tcp.port");
			get().mqtt.resetOnConnect = config.get<bool>("mqtt.reset-on-connect", get().mqtt.resetOnConnect);
			get().mqtt.forceAuth = config.get<bool>("mqtt.force-auth", get().mqtt.forceAuth);
			get().jwt.expirationSec = config.get<uint32_t>("jwt.expiration-sec", get().jwt.expirationSec);
			get().jwt.key = config.get<std::string>("jwt.key", get().jwt.key);
			get().jwt.keyFile = config.get<std::string>("jwt.key-file", get().jwt.keyFile);
			if (!get().jwt.keyFile.empty()) {
				std::ifstream file(get().jwt.keyFile);
				if (!file.is_open()) {
					throw Utils::Error("Can't open file, name: " + get().jwt.keyFile);
				}
				get().jwt.key.assign(
					(std::istreambuf_iterator<char>(file)),
					std::istreambuf_iterator<char>()
				);
				boost::trim(get().jwt.key);
			}
		} catch (const boost::property_tree::ptree_bad_data& e) {
			throw Utils::Error(e, "bad-data [" + e.data<std::string>() + "]");
		}
	} catch (std::exception& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Configuration));
	}
}

Configuration& Configuration::get()
throw (Utils::Error)
{
	return ConfigurationImpl::get();
}

void Configuration::load()
throw (Utils::Error)
{
	return ConfigurationImpl::load();
}

void Configuration::destroy()
throw ()
{
	ConfigurationImpl::destroy();
}

void Configuration::dump() const {
	*ConfigurationImpl::mLog.info() << "Configuration:";
	*ConfigurationImpl::mLog.info() << "logger.level             = " << LoggerLevel::toString(logger.level);
	*ConfigurationImpl::mLog.info() << "logger.file              = " << logger.file;
	*ConfigurationImpl::mLog.info() << "serial.name              = " << serial.name;
	*ConfigurationImpl::mLog.info() << "serial.boud              = " << serial.baud;
	*ConfigurationImpl::mLog.info() << "tcp.address              = " << tcp.address;
	*ConfigurationImpl::mLog.info() << "tcp.port                 = " << tcp.port;
	*ConfigurationImpl::mLog.info() << "mqtt.reset-on-connect    = " << putBool(mqtt.resetOnConnect);
	*ConfigurationImpl::mLog.info() << "mqtt.force-auth          = " << putBool(mqtt.forceAuth);
	*ConfigurationImpl::mLog.info() << "jwt.expiration-sec       = " << jwt.expirationSec;
	*ConfigurationImpl::mLog.info() << "jwt.key                  = " << (jwt.key.empty()     ? "<NA>" : "<*>");
	*ConfigurationImpl::mLog.info() << "jwt.keyFile              = " << (jwt.keyFile.empty() ? "<NA>" : jwt.keyFile);
}

} /* namespace Utils */
