/*
 *******************************************************************************
 *
 * Purpose: TCP network. Database
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef TCP_NET_DB_H_
#define TCP_NET_DB_H_

/* Internal Includes */
#include "Id.h"
/* External Includes */
/* System Includes */
#include <list>
#include <memory>


/* Forward declaration */
namespace Networking {class Address;}
class TcpNetConnection;

/**
 * TCP network database to store available connections.
 */
class TcpNetDb {
public:
	/**
	 * Constructor
	 */
	TcpNetDb();

	/**
	 * Destructor
	 */
	~TcpNetDb();

	/**
	 * Lookup a connection
	 *
	 * @param from sender address
	 * @param to recipient address
	 * @return connection if available
	 */
	TcpNetConnection* get(const Networking::Address& from, const Networking::Address& to);

	/**
	 * Put a new connection.
	 *
	 * @param connection the connection for storing.
	 */
	void put(std::unique_ptr<TcpNetConnection> connection);

	/**
	 * Destroys connection.
	 *
	 * @param id the connection identifier
	 */
	void destroy(Utils::Id id);
private:
	std::list<TcpNetConnection*>			mConnections;

	void destroy(TcpNetConnection*);
};

#endif /* TCP_NET_DB_H_ */
