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

#ifndef CONFIGURATION_IMPL_H_
#define CONFIGURATION_IMPL_H_

/* Internal Includes */
#include "Logger.h"
/* External Includes */
/* System Includes */
#include <mutex>


namespace Utils {

/* Forward declaration */
class Configuration;

/**
 * Configuration factory. Makes instance and loading.
 * Allowed to use only from 'Configuration' class.
 */
class ConfigurationImpl {
private:
	/**
	 * Gets the instance of the 'Configuration'
	 */
	static Configuration& get() throw (Utils::Error);

	/**
	 * Load configuration to the 'Configuration' instance
	 */
	static void load() throw (Utils::Error);

	/**
	 * Read configuration from environment
	 */
	static void loadEnv() throw (Utils::Error);

	/**
	 * Destroy the instance of the 'Configuration' If available
	 */
	static void destroy() throw ();

	static Configuration*			mInstance;
	static std::mutex				mMtxInstance;
	static Utils::Logger				mLog;

	// static class
	ConfigurationImpl();
	~ConfigurationImpl() throw ();

	friend class Configuration;
};

} /* namespace Utils */

#endif /* CONFIGURATION_IMPL_H_ */
