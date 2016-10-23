/*
 *******************************************************************************
 *
 * Purpose: TCP network. Connection.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014-2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */


/* Internal Includes */
#include "TcpNetConnection.h"
#include "TcpNet.h"
#include "TcpNetCommand.h"
#include "TcpNetDb.h"
/* External Includes */
#include "Application.h"
#include "Router.h"
#include "NetworkingDataUnit.h"
#include "CommandProcessor.h"
/* System Includes */
#include <boost/lexical_cast.hpp>


///////////////////// TcpNetCommands /////////////////////
class TcpNetCommandConnectionDestroy: public TcpNetCommand {
public:
	TcpNetCommandConnectionDestroy(TcpNet& owner, Utils::Id id)
	:
		TcpNetCommand(owner),
		mId(id)
	{}

	void execute() {
		getDb().destroy(mId);
	}
private:
	Utils::Id		mId;
};

///////////////////// TcpNetConnection /////////////////////
Utils::IdGen TcpNetConnection::mIdGen;

TcpNetConnection::TcpNetConnection(TcpNet& owner,
		std::unique_ptr<Networking::Address> from,
		std::unique_ptr<Networking::AddressTcp> to)
throw (Utils::Error)
:
	mLog(__FUNCTION__),
	mState (STATE_NEW),
	mId(mIdGen.get()),
	mOwner(owner),
	mSocket(mOwner.getIo()),
	mFrom(std::move(from)),
	mTo(std::move(to))
{
	try {
		try {
			boost::asio::ip::tcp::resolver resolver(mOwner.getIo());
			mEndPoint = resolver.resolve(
			boost::asio::ip::tcp::resolver::query(mTo->get().host,
					boost::lexical_cast<std::string>(mTo->get().port)));
			scheduleConnect(mEndPoint);
		} catch (boost::system::system_error& e) {
			throw Utils::Error(e);
		}
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(TcpNetConnection));
	}
}

TcpNetConnection::~TcpNetConnection() {
	cancel();
	// be sure that nothing is working
	std::lock_guard<std::mutex> locker(mMtx);
	setState(STATE_DESTROYED);
}

void TcpNetConnection::send(std::unique_ptr<Networking::Buffer> buffer) {
	std::lock_guard<std::mutex> locker(mMtx);
	if (!isAlive()) return;
	try {
		mWriteQueue.push(std::move(buffer));
		if (isWriteReady()) {
			scheduleWrite();
		}
	} catch (Utils::Error& e) {
		*mLog.error() << UTILS_STR_FUNCTION << ", error: " << e.what();
		destroy();
	}
}

///////////////////// TcpNetConnection::Internal /////////////////////
void TcpNetConnection::setState(State v) {
	mState = v;
}

void TcpNetConnection::scheduleConnect(boost::asio::ip::tcp::resolver::iterator endpoint)
throw (Utils::Error)
{
	try {
		try {
			if (endpoint == boost::asio::ip::tcp::resolver::iterator()) {
				// There are no more End-Points to try
				throw Utils::Error("End-Point is not available");
			}
			mSocket.async_connect(
				endpoint->endpoint(),
				[this](const boost::system::error_code& a) {
							onConnect(a);
				}
			);
		} catch (boost::system::system_error e) {
			throw Utils::Error(e);
		}
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(TcpNetConnection));
	}
}

void TcpNetConnection::scheduleRead()
throw (Utils::Error)
{
	try {
		try {
			mSocket.async_read_some(
				boost::asio::buffer(mBufferRead, mBufferReadSize),
				[this](const boost::system::error_code& a, std::size_t b) {
					onRead(a, b);
				}
			);
		} catch (boost::system::system_error e) {
			throw Utils::Error(e);
		}
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(TcpNetConnection));
	}
}

void TcpNetConnection::scheduleWrite(std::size_t shift)
throw (Utils::Error)
{
	if (mWriteQueue.empty()) return;
	assert(shift<mWriteQueue.front()->size());
	try {
		try {
			mSocket.async_write_some(
				boost::asio::buffer(mWriteQueue.front()->data()+shift, mWriteQueue.front()->size()-shift),
				[this](const boost::system::error_code& a, std::size_t b) {
					onWrite(a, b);
				}
			);
			setState(STATE_READING_WRITING);
		} catch (boost::system::system_error e) {
			throw Utils::Error(e);
		}
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(TcpNetConnection));
	}
}

void TcpNetConnection::cancel() {
	// cancel everything
	boost::system::error_code ec;
	mSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
	mSocket.close(ec);
}

void TcpNetConnection::destroy() {
	*mLog.info() << "Destroying, " << mFrom->toString() << " <-> " << mTo->toString();
	setState(STATE_DESTROYING);
	cancel();
	std::unique_ptr<Utils::Command> cmd
		(new TcpNetCommandConnectionDestroy(mOwner, mId));
	mOwner.getProcessor().process(std::move(cmd));
}

///////////////////// TcpNetConnection::Internal Asynchronous /////////////////////
void TcpNetConnection::onConnect(const boost::system::error_code& error)
{
	std::lock_guard<std::mutex> locker(mMtx);
	if (!isAlive()) return;
	try {
		if (!mSocket.is_open()) {
			// try next
			scheduleConnect(++mEndPoint);
		} else if (error) {
			boost::system::error_code ec;
			// socket could be opened
			mSocket.close(ec);
			// try next
			scheduleConnect(++mEndPoint);
		} else {
			setState(STATE_CONNECTED);
			scheduleRead();
			setState(STATE_READING);
			scheduleWrite();
		}
	} catch (Utils::Error& e) {
		*mLog.error() << UTILS_STR_FUNCTION << ", error: " << e.what();
		destroy();
	}
}

void TcpNetConnection::onRead(const boost::system::error_code& error, std::size_t qty) {
	std::lock_guard<std::mutex> locker(mMtx);
	if (!isAlive()) return;
	try {
		if (qty) {
			*mLog.debug() << UTILS_STR_FUNCTION << ", size: " << qty;
			std::unique_ptr<Networking::Buffer> data
						(new Networking::Buffer(mBufferRead, mBufferRead+qty));
			std::unique_ptr<Networking::DataUnit> unit(new Networking::DataUnitTcp(
					std::move(data),
					mTo->clone(),	// To -> From
					mFrom->clone()	// From -> To
			));
			Application::get().getRouter().process(std::move(unit));
		}
		if (error == boost::asio::error::eof || error) {
			*mLog.error() << UTILS_STR_FUNCTION << ", error: " << error.message();
			destroy();
		} else {
			scheduleRead();
		}
	} catch (Utils::Error& e) {
		*mLog.error() << UTILS_STR_FUNCTION << ", error: " << e.what();
		destroy();
	}
}

void TcpNetConnection::onWrite(const boost::system::error_code& error, std::size_t qty) {
	std::lock_guard<std::mutex> locker(mMtx);
	if (!isAlive()) return;
	assert(qty<=mWriteQueue.front()->size());
	try {
		if (error == boost::asio::error::eof || error) {
			destroy();
		} else {
			setState(STATE_READING);
			// calculate remaining bytes in current buffer
			std::size_t shift = mWriteQueue.front()->size()-qty;
			if (!shift) {
				// buffer is fully sent => go to next buffer
				mWriteQueue.pop();
			}
			scheduleWrite(shift);
		}
	} catch (Utils::Error& e) {
		*mLog.error() << UTILS_STR_FUNCTION << ", error: " << e.what();
		destroy();
	}
}
