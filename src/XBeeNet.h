/*
 *******************************************************************************
 *
 * Purpose: XBee network implementation
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef XBEE_NET_H_
#define XBEE_NET_H_

/* Internal Includes */
#include "Logger.h"
/* External Includes */
#include "NetworkingDefs.h"
/* System Includes */
#include <stdint.h>
#include <vector>
#include <memory>

/* Forward declaration */
namespace Networking {class Address;}
struct XBeeNetContext;

/**
 * XBee (ZigBee) network
 */
class XBeeNet {
public:
	/**
	 * Constructor
	 */
	XBeeNet();

	/**
	 * Destructor
	 */
	~XBeeNet();

	/**
	 * Starts the processing
	 */
	void start();

	/**
	 * Stops the processing
	 */
	void stop();

	/**
	 * Processes the buffer received from XBee network
	 *
	 * @param buffer XBee network buffer chunk in API 2 mode
	 */
	void from(std::unique_ptr< std::vector<uint8_t> > buffer) throw ();

	/**
	 * Sends data to XBee network
	 *
	 * @param from sender address
	 * @param to recipient address
	 * @param buffer data to be sent
	 */
	void to(const Networking::Address* from, const Networking::Address* to,
			std::unique_ptr<Networking::Buffer> buffer) throw ();
private:
	// Objects
	Utils::Logger				mLog;
	XBeeNetContext*				mCtx;

	// Do not copy
	XBeeNet(const XBeeNet&);
	XBeeNet &operator=(const XBeeNet&);

	// Methods
	void onFrom(std::unique_ptr< std::vector<uint8_t> > buffer);
	void onTo(std::unique_ptr<Networking::Address>, std::unique_ptr<Networking::Address>,
			std::unique_ptr<Networking::Buffer>);
	void onFrame(std::unique_ptr<std::vector<uint8_t> >);
};

#endif /* XBEE_NET_H_ */
