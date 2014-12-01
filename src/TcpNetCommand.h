/*
 *******************************************************************************
 *
 * Purpose: TCP network. Asynchronous commands.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef TCP_NET_COMMAND_H_
#define TCP_NET_COMMAND_H_

/* Internal Includes */
#include "TcpNet.h"
/* External Includes */
#include "Command.h"
/* System Includes */


class TcpNetCommand: public Utils::Command {
public:
	TcpNetCommand(TcpNet& owner): mOwner(owner) {}
	virtual ~TcpNetCommand() {}
protected:
	TcpNetDb& getDb() const {return mOwner.getDb();}
private:
	TcpNet&		mOwner;
};

#endif /* TCP_NET_COMMAND_H_ */
