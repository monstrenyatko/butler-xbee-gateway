/*
 *******************************************************************************
 *
 * Purpose: TCP network. Connection.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef TCP_NET_CONNECTION_H_
#define TCP_NET_CONNECTION_H_

/* Internal Includes */
#include "NetworkingAddress.h"
#include "Error.h"
#include "IdGen.h"
/* External Includes */
/* System Includes */
#include <mutex>
#include <queue>
#include <boost/asio.hpp>

#define TCP_READER_BUFFER_SIZE 512
class TcpNet;


/**
 * TCP network connection.
 */
class TcpNetConnection {
public:
	TcpNetConnection(TcpNet&, std::unique_ptr<Networking::Address>,
			std::unique_ptr<Networking::AddressTcp>) throw (Utils::Error);
	~TcpNetConnection();

	Utils::Id getId() const {return mId;}
	const Networking::Address* getFrom() const {return mFrom.get();}
	const Networking::AddressTcp* getTo() const {return mTo.get();}

	void send(std::unique_ptr<Networking::Buffer> buffer);
private:
	static Utils::IdGen									mIdGen;
	std::mutex											mMtx;
	enum State {
		STATE_NEW,
		STATE_CONNECTED,
		STATE_READING,
		STATE_READING_WRITING,
		STATE_DESTROYING,
		STATE_DESTROYED,
	}													mState;
	Utils::Id											mId;
	TcpNet&												mOwner;
	boost::asio::ip::tcp::socket						mSocket;
	boost::asio::ip::tcp::resolver::iterator			mEndPoint;
	std::unique_ptr<Networking::Address>				mFrom;
	std::unique_ptr<Networking::AddressTcp>				mTo;
	static const std::size_t							mBufferReadSize = TCP_READER_BUFFER_SIZE;
	uint8_t												mBufferRead[mBufferReadSize];
	std::queue< std::unique_ptr<Networking::Buffer> >	mWriteQueue;

	void setState(State);
	State getState() const {return mState;}
	bool isAlive() const {return mState<STATE_DESTROYING;}
	bool isWriteReady() const {return mState==STATE_READING;}
	void cancel();
	void destroy();
	void scheduleConnect(boost::asio::ip::tcp::resolver::iterator) throw (Utils::Error);
	void scheduleRead() throw (Utils::Error);
	void scheduleWrite(std::size_t shift = 0) throw (Utils::Error);

	void onConnect(const boost::system::error_code&);
	void onRead(const boost::system::error_code&, std::size_t);
	void onWrite(const boost::system::error_code&, std::size_t);
};

#endif /* TCP_NET_CONNECTION_H_ */
