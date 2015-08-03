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
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

namespace Utils {

#define CONFIG_FILE		"config.json"

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
		// It mast not happen
		// We can't use logger When the instance is destroyed => ignore
	}
}

void ConfigurationImpl::load()
throw (Utils::Error)
{
	try {
		*ConfigurationImpl::mLog.info() << "Loading from file, path: " << CONFIG_FILE;
		// read and pars configuration file
		boost::property_tree::ptree config;
		boost::property_tree::read_json(CONFIG_FILE, config);
		// get parameters
		try {
			{
				std::string value(config.get<std::string>("logger.level"));
				boost::to_upper(value);
				get().logger.level = LoggerLevel::fromString(value);
			}
			get().serial.name = config.get<std::string>("serial.name");
			get().serial.baud = config.get<uint32_t>("serial.baud");
			get().tcp.address = config.get<std::string>("tcp.address");
			get().tcp.port = config.get<uint32_t>("tcp.port");
		} catch (const boost::property_tree::ptree_bad_data& e) {
			throw Utils::Error(e, "bad-data [" + e.data<std::string>() + "]");
		}
	} catch (std::exception& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Configuration));
	}
}

const Configuration& Configuration::get()
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
	*ConfigurationImpl::mLog.info() << "logger.level = " << LoggerLevel::toString(logger.level);
	*ConfigurationImpl::mLog.info() << "serial.name = " << serial.name;
	*ConfigurationImpl::mLog.info() << "serial.boud = " << serial.baud;
	*ConfigurationImpl::mLog.info() << "tcp.address = " << tcp.address;
	*ConfigurationImpl::mLog.info() << "tcp.port    = " << tcp.port;
}

} /* namespace Utils */
