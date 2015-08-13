/*
 *******************************************************************************
 *
 * Purpose: Networking. The common definitions
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef NETWORKING_DEFS_H_
#define NETWORKING_DEFS_H_

/* Internal Includes */
/* External Includes */
/* System Includes */
#include <vector>
#include <stdint.h>
#include <ostream>

namespace Networking {

typedef std::vector<uint8_t> Buffer;

namespace Origin {
	enum Type {
		// from serial
		SERIAL,
		// from XBee network
		XBEE,
		// from XBee encoder for sending to XBee network
		XBEE_ENCODER,
		// from TCP network
		TCP
	};

	inline std::string toString(Type type) {
		switch (type) {
			case SERIAL:			return "SERIAL";
			case XBEE:				return "XBEE";
			case XBEE_ENCODER:		return "XBEE_ENCODER";
			case TCP:				return "TCP";
			default:				return "UNKNW";
		}
	}

	inline std::ostream& operator<<(std::ostream& os, Type obj) {
		return os << toString(obj);
	}
}

} /* namespace Networking */

#endif /* NETWORKING_DEFS_H_ */
