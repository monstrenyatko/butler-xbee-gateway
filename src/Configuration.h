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

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

/* Internal Includes */
/* External Includes */
/* System Includes */
#include <stdint.h>
#include <string>

class Configuration {
public:
	Configuration();

	~Configuration() throw ();

	void dump();

	struct Serial {
		std::string		name;
		uint32_t		baud;

	} serial = {"/dev/serial", 57600};

	struct Tcp {
		std::string		address;
		uint32_t		port;
	} tcp = {"localhost", 1883};
};


#endif /* CONFIGURATION_H_ */
