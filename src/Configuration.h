/*
 *******************************************************************************
 *
 * Purpose: Utils. Configuration implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

/* Internal Includes */
#include "LoggerLevel.h"
/* External Includes */
/* System Includes */
#include <stdint.h>
#include <string>

namespace Utils {

/* Forward declaration */
class ConfigurationImpl;

class Configuration {
public:
	/**
	 * Gets the instance
	 */
	static Configuration& get() throw (Utils::Error);

	/**
	 * Destroy the instance If available
	 */
	static void destroy() throw ();

	/**
	 * Load configuration
	 */
	static void load() throw (Utils::Error);

	/**
	 * Log the current configuration
	 */
	void dump() const;

	struct Main {
		std::string		configFileName;
	} main = {"config.json"};

	struct Logger {
		LoggerLevel::Type	level;
	} logger = {LoggerLevel::TRACE};

	struct Serial {
		std::string		name;
		uint32_t		baud;

	} serial = {"/dev/serial", 57600};

	struct Tcp {
		std::string		address;
		uint32_t		port;
	} tcp = {"localhost", 1883};
private:
	Configuration() throw () {};
	~Configuration() throw () {};

	// Do not copy
	Configuration(const Configuration&);
	Configuration &operator=(const Configuration&);

	friend class ConfigurationImpl;
};

} /* namespace Utils */

#endif /* CONFIGURATION_H_ */
