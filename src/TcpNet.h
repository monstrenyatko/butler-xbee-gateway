/*
 *******************************************************************************
 *
 * Purpose: TCP network implementation
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef TCP_NET_H_
#define TCP_NET_H_

/* Internal Includes */
/* External Includes */
#include "NetworkingDefs.h"
/* System Includes */
#include <memory>


/* Forward declaration */
namespace boost {namespace asio {class io_service;}}
namespace Networking {class Address;}
namespace Utils {class CommandProcessor;}
struct TcpNetContext;
class TcpNetConnection;
class TcpNetCommand;
class TcpNetDb;

/**
 * TCP network
 */
class TcpNet {
public:
	/**
	 * Constructor
	 */
	TcpNet();

	/**
	 * Destructor
	 */
	~TcpNet();

	/**
	 * Starts the processing
	 */
	void start();

	/**
	 * Stops the processing
	 */
	void stop();

	/**
	 * Sends data to XBee network
	 *
	 * @param from sender address
	 * @param to recipient address
	 * @param buffer data to be sent
	 */
	void send(const Networking::Address* from, const Networking::Address* to,
			std::unique_ptr<Networking::Buffer> buffer) throw ();
private:
	// Objects
	TcpNetContext*				mCtx;

	// Do not copy
	TcpNet(const TcpNet&);
	TcpNet &operator=(const TcpNet&);

	// Methods
	void onSend(std::unique_ptr<Networking::Address>, std::unique_ptr<Networking::Address>,
			std::unique_ptr<Networking::Buffer>);

	// Internal
	friend class TcpNetCommand;
	friend class TcpNetConnection;
	boost::asio::io_service& getIo() const;
	Utils::CommandProcessor& getProcessor() const;
	TcpNetDb& getDb() const;
};

#endif /* TCP_NET_H_ */
