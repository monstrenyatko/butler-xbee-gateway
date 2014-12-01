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

/* Internal Includes */
#include "TcpNetDb.h"
#include "TcpNetConnection.h"
#include "Error.h"
/* External Includes */
/* System Includes */
#include <assert.h>
#include <iostream>


///////////////////// TcpNetDb /////////////////////
TcpNetDb::TcpNetDb() {
}

TcpNetDb::~TcpNetDb() {
	for (auto i: mConnections) {
		try {
			destroy(i);
		} catch (std::exception&) {}
	}
}

TcpNetConnection* TcpNetDb::get(const Networking::Address& from, const Networking::Address& to) {
	for (auto i: mConnections) {
		if (from.isEqual(*(i->getFrom()))
			&& to.isEqual(*(i->getTo())))
		{
			return i;
		}
	}
	return NULL;
}

void TcpNetDb::put(std::unique_ptr<TcpNetConnection> connection) {
	assert(connection.get());
	std::cout<< UTILS_STR_CLASS_FUNCTION(TcpNetDb)<<", Id:"<<connection->getId()<<std::endl;
	for (auto it = mConnections.begin(); it!=mConnections.end(); it++) {
		if (connection->getFrom()->isEqual(*((*it)->getFrom()))
			&& connection->getTo()->isEqual(*((*it)->getTo())))
		{
			// replace old by new one
			destroy(*it);
			*it = connection.release();
			return;
		}
	}
	// add new one
	mConnections.push_back(connection.get());
	connection.release();
}

void TcpNetDb::destroy(Utils::Id id) {
	for (auto it = mConnections.begin(); it!=mConnections.end(); it++) {
		if (id == (*it)->getId()) {
			// remove
			destroy(*it);
			mConnections.erase(it);
			break;
		}
	}
}

///////////////////// TcpNetDb::Internal /////////////////////
void TcpNetDb::destroy(TcpNetConnection* connection) {
	std::cout<< UTILS_STR_CLASS_FUNCTION(TcpNetDb)<<", Id:"<<connection->getId()<<std::endl;
	delete connection;
}
