/*
 *******************************************************************************
 *
 * Purpose: MQTT implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2018.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef MQTT_H_
#define MQTT_H_

/* Internal Includes */
#include "Logger.h"
#include "NetworkingDefs.h"
#include "NetworkingAddress.h"
/* External Includes */
/* System Includes */
#include <string>


/* Forward declaration */
class TcpNetConnection;

/**
 * Provides MQTT protocol related functionality
 */
class Mqtt {
public:
	/**
	 * Constructor
	 */
	Mqtt();

	/**
	 * Destructor
	 */
	~Mqtt();

	/**
	 * Closes connection on CONNECT message.
	 */
	void closeOnConnect(Networking::Buffer& buffer, TcpNetConnection** connection);

	/**
	 * Adds authentication to CONNECT message.
	 */
	void forceAuth(Networking::Buffer& buffer, TcpNetConnection& connection);

	/**
	 * Closes expired connections.
	 */
	void closeOnExpire(TcpNetConnection** connection);
private:
	// Objects
	Utils::Logger									mLog;

	// Do not copy
	Mqtt(const Mqtt&);
	Mqtt &operator=(const Mqtt&);
};

#endif /* MQTT_H_ */
