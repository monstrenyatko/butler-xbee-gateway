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

namespace Networking {

typedef std::vector<uint8_t> Buffer;

namespace Origin {
	enum Type {
		// from serial
		SERIAL,
		// from XBee network
		XBEE_NET,
		// from XBee encoder for sending to XBee network
		XBEE_ENCODER,
		// from TCP network
		TCP
	};
}

} /* namespace Networking */

#endif /* NETWORKING_DEFS_H_ */
