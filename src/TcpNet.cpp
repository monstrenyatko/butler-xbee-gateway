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

/* Internal Includes */
#include "TcpNet.h"
#include "TcpNetDb.h"
#include "TcpNetConnection.h"
#include "TcpNetCommand.h"
#include "NetworkingAddress.h"
#include "Application.h"
#include "CommandProcessor.h"
#include "Memory.h"
#include "Thread.h"
/* External Includes */
/* System Includes */
#include <boost/asio.hpp>
#include <assert.h>

///////////////////// TcpNetIoService /////////////////////
class TcpNetIoService: private Utils::Thread {
public:
	TcpNetIoService()
	:
		Utils::Thread(__FUNCTION__),
		mLog(getName())
	{}

	~TcpNetIoService() {
		stop();
	}

	void start(void) {
		Utils::Thread::start();
	}

	void stop(void) {
		Utils::Thread::stop();
	}

	boost::asio::io_service& getIoService() {return mIoService;}
private:
	// Objects
	Utils::Logger									mLog;
	boost::asio::io_service							mIoService;

	// Methods
	void onStop() {
		mIoService.stop();
	}

	void loop() {
		boost::asio::io_service::work work(getIoService());
		while (isAlive()) {
			boost::system::error_code ec;
			getIoService().reset();
			if (!getIoService().run(ec)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
		*mLog.debug() << UTILS_STR_FUNCTION << ", done";
	}
};

///////////////////// TcpNetContext /////////////////////
struct TcpNetContext {
	Utils::CommandProcessor							processor;
	TcpNetIoService									ioService;
	TcpNetDb										db;
	TcpNetContext(const std::string& name) : processor(name) {}
};

///////////////////// TcpNetCommands /////////////////////
class TcpNetCommandSend: public TcpNetCommand {
public:
	typedef std::function<void(std::unique_ptr<Networking::Address>,
			std::unique_ptr<Networking::Address>,
			std::unique_ptr<Networking::Buffer>)> Cbk;
	TcpNetCommandSend(TcpNet& owner, Cbk cbk,
			const Networking::Address* from, const Networking::Address* to,
			std::unique_ptr<Networking::Buffer> buffer)
	:
		TcpNetCommand(owner),
		mCbk(cbk),
		mFrom(from->clone()),
		mTo(to->clone()),
		mData(std::move(buffer))
	{}

	void execute() {
		mCbk(std::move(mFrom), std::move(mTo), std::move(mData));
	}
private:
	Cbk											mCbk;
	std::unique_ptr<Networking::Address>		mFrom;
	std::unique_ptr<Networking::Address>		mTo;
	std::unique_ptr<Networking::Buffer>			mData;
};

///////////////////// TcpNet /////////////////////
TcpNet::TcpNet()
:
	mLog(__FUNCTION__),
	mCtx(new TcpNetContext(mLog.getName()))
{
}

TcpNet::~TcpNet() {
	delete mCtx;
}

void TcpNet::start() {
	mCtx->processor.start();
	mCtx->ioService.start();
}

void TcpNet::stop() {
	mCtx->processor.stop();
	mCtx->ioService.stop();
}

void TcpNet::send(const Networking::Address* from, const Networking::Address* to,
			std::unique_ptr<Networking::Buffer> buffer)
throw ()
{
	assert(from);
	assert(to);
	assert(to->getOrigin()==Networking::Origin::TCP);
	assert(buffer.get());

	std::unique_ptr<Utils::Command> cmd (new TcpNetCommandSend(*this,
		[this] (std::unique_ptr<Networking::Address> a, std::unique_ptr<Networking::Address> b,
				std::unique_ptr<Networking::Buffer> c) {
				onSend(std::move(a), std::move(b), std::move(c));
		},
		from,
		to,
		std::move(buffer)
	));
	mCtx->processor.process(std::move(cmd));
}

///////////////////// TcpNet::Internal /////////////////////
void TcpNet::onSend(std::unique_ptr<Networking::Address> from, std::unique_ptr<Networking::Address> to,
		std::unique_ptr<Networking::Buffer> buffer)
{
	*mLog.debug() << UTILS_STR_FUNCTION << ", size:" << buffer->size();
	try {
		TcpNetConnection* connection = mCtx->db.get(*from, *to);
		if (connection && isMqttConnect(*buffer)) {
			*mLog.info() << "Reestablish Connection";
			connection->close();
			connection = NULL;
		}
		if (!connection) {
			*mLog.info() << "Connecting, " << from->toString() << " <-> " << to->toString();
			// prepare parameters
			std::unique_ptr<Networking::AddressTcp> to_ =
					Utils::dynamic_unique_ptr_cast<Networking::AddressTcp, Networking::Address>(to);
			assert(to_.get());
			// create new
			std::unique_ptr<TcpNetConnection> t(new TcpNetConnection
					(*this, std::move(from), std::move(to_)));
			connection = t.get();
			mCtx->db.put(std::move(t));
		}
		// send
		connection->send(std::move(buffer));
		*mLog.debug() << UTILS_STR_FUNCTION << ", push to ID: " << connection->getId();
	} catch (Utils::Error& e) {
		*mLog.error() << UTILS_STR_FUNCTION << ", error: " << e.what();
	}
}

bool TcpNet::isMqttConnect(const Networking::Buffer& buffer) const {
	uint32_t size = buffer.size();
	if (size > 1) {
		uint8_t maxLengthCursor = 4;
		uint8_t cursor = 0;
		uint8_t header = buffer[cursor++];
		// check header
		*mLog.debug() << UTILS_STR_FUNCTION << ", MQTT Header" <<  Utils::putByte(header);
		if ((((header & 0xF0) >> 4) & 0x0F) == 1) {
			*mLog.debug() << UTILS_STR_FUNCTION << ", MQTT Connect header" <<  Utils::putByte(header);
			// skip length: skip all bytes with most significant bit == '1' and next one with most significant bit == '0'
			do {
				if (cursor > maxLengthCursor ) {
					*mLog.debug() << UTILS_STR_FUNCTION << "Max length cursor value reached";
					return false;
				}
			} while ((buffer[cursor++] & 128) != 0);
			*mLog.debug() << UTILS_STR_FUNCTION << ", length size: " << (int)cursor;
			// skip 2 bytes header length
			cursor+=2;
			if ((int)size > (cursor + 6)) {
				*mLog.debug() << UTILS_STR_FUNCTION << ", MQTT Connect protocol name:" << buffer[cursor];
				*mLog.debug() << UTILS_STR_FUNCTION << ", MQTT Connect protocol name:" << buffer[cursor+1];
				*mLog.debug() << UTILS_STR_FUNCTION << ", MQTT Connect protocol name:" << buffer[cursor+2];
				*mLog.debug() << UTILS_STR_FUNCTION << ", MQTT Connect protocol name:" << buffer[cursor+3];
				if (buffer[cursor] == 'M' && buffer[cursor+1] == 'Q') {
					if (buffer[cursor+2] == 'T') {
						return (buffer[cursor+3] == 'T');
					} else if (buffer[cursor+2] == 'I') {
						*mLog.debug() << UTILS_STR_FUNCTION << ", MQTT Connect protocol name:" << buffer[cursor+4];
						*mLog.debug() << UTILS_STR_FUNCTION << ", MQTT Connect protocol name:" << buffer[cursor+5];
						return (buffer[cursor+3] == 's' && buffer[cursor+4] == 'd' && buffer[cursor+5] == 'p');
					}
				}
			}
		}
	}

	return false;
}

///////////////////// TcpNet::Internal Interface /////////////////////
boost::asio::io_service& TcpNet::getIo() const {
	return mCtx->ioService.getIoService();
}

Utils::CommandProcessor& TcpNet::getProcessor() const {
	return mCtx->processor;
}

TcpNetDb& TcpNet::getDb() const {
	return mCtx->db;
}
