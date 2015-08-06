/*
 *******************************************************************************
 *
 * Purpose: Serial port read/write implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014-2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "SerialPort.h"
#include "SerialPortCommand.h"
#include "Thread.h"
#include "Semaphore.h"
#include "CommandProcessor.h"
#include "Configuration.h"
#include "Error.h"
#include "Application.h"
#include "Router.h"
#include "NetworkingDataUnit.h"
/* External Includes */
/* System Includes */
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <mutex>

/* Forward declaration */
class SerialPortReader;
class SerialPortWriter;
class SerialPortOpener;

#define SERIAL_PORT_READER_BUFFER_SIZE		512
#define SERIAL_PORT_OPENER_ERROR_TM_MS		5000		// 5 sec

///////////////////// TcpNetIoService /////////////////////
class SerialPortIoService: private Utils::Thread {
public:
	SerialPortIoService()
	:
		Utils::Thread(__FUNCTION__),
		mLog(getName())
	{}

	~SerialPortIoService() {
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
		getIoService().stop();
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

///////////////////// SerialPortContext /////////////////////
struct SerialPortContext {
	std::recursive_mutex							mtx;
	Utils::CommandProcessor							processor;
	SerialPortIoService								ioService;
	std::string										portName;
	uint32_t										portBaud;
	struct Serial {
		std::unique_ptr<boost::asio::serial_port>		port;
		std::unique_ptr<SerialPortReader>				portReader;
		std::unique_ptr<SerialPortWriter>				portWriter;

		~Serial() {
			if (port) {
				boost::system::error_code ec;
				port->close(ec);
			}
		}
	};
	std::unique_ptr<Serial>							serial;
	std::unique_ptr<SerialPortOpener>				portOpener;

	SerialPortContext(const std::string& name) : processor(name) {}
};

///////////////////// SerialPortReader /////////////////////
class SerialPortReader {
public:
	SerialPortReader(SerialPortContext& ctx)
	throw (Utils::Error):
		mLog(__FUNCTION__),
		mCtx(ctx),
		mPort(*(mCtx.serial->port))
	{
		schedule();
		*mLog.debug() << "Started";
	}

	~SerialPortReader() {
		boost::system::error_code ec;
		mPort.cancel(ec);
	}
private:
	// Objects
	Utils::Logger					mLog;
	SerialPortContext&				mCtx;
	boost::asio::serial_port&		mPort;
	uint8_t							mBufferRead[SERIAL_PORT_READER_BUFFER_SIZE];

	/**
	 * Function is called on read operation finish.
	 */
	void onReceive(const boost::system::error_code& error, std::size_t qty) {
		if (qty) {
			std::unique_ptr< std::vector<uint8_t> > data
						(new std::vector<uint8_t>(mBufferRead, mBufferRead+qty));
			std::unique_ptr<Networking::DataUnit> unit(new Networking::DataUnitSerial(
					std::move(data),
					std::unique_ptr<Networking::Address>
						(new Networking::AddressSerial(mCtx.portName)),
					std::unique_ptr<Networking::Address>()
			));
			Application::get().getRouter().process(std::move(unit));
		}
		try {
			if (!qty && error) {
				throw Utils::Error(error.message());
			}
			schedule();
		} catch (Utils::Error& e) {
			std::unique_ptr<Utils::Command> cmd (new SerialPortCommandClosed(e.what()));
			Application::get().getProcessor().process(std::move(cmd));
		}
	}

	/**
	 * Schedules next read operation
	 */
	void schedule()
	throw (Utils::Error)
	{
		try {
			try {
				if (!mPort.is_open()) {
					throw Utils::Error("Port is not opened");
				}
				mPort.async_read_some(
						boost::asio::buffer(mBufferRead, SERIAL_PORT_READER_BUFFER_SIZE),
						boost::bind(
								&SerialPortReader::onReceive,
								this, boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred)
				);
			} catch (boost::system::system_error e) {
				throw Utils::Error(e);
			}
		} catch (Utils::Error& e) {
			throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(SerialPortReader));
		}
	}
};

///////////////////// SerialPortWriter /////////////////////
class SerialPortWriter {
public:
	SerialPortWriter(SerialPortContext& ctx)
	throw ():
		mLog(__FUNCTION__),
		mPort(*(ctx.serial->port))
	{}

	void write(std::unique_ptr< std::vector<uint8_t> > data) {
		*mLog.debug() << UTILS_STR_FUNCTION <<", size: " << data->size();
		try {
			try {
				std::size_t qty = boost::asio::write(mPort, boost::asio::buffer(&((*data)[0]), data->size()));
				*mLog.debug() << UTILS_STR_FUNCTION << ", written : " << qty;
				// TODO: process write`s return value
			} catch (boost::system::system_error& e) {
				throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(SerialPortWriter));
			}
		} catch (std::exception& e) {
			*mLog.error() << UTILS_STR_FUNCTION << ", error: " << e.what();
		}
	}
private:
	// Objects
	Utils::Logger					mLog;
	boost::asio::serial_port&		mPort;
};

///////////////////// SerialPortOpener /////////////////////
class SerialPortOpener : private Utils::Thread {
public:
	typedef std::function<bool(void)> Cbk;

	SerialPortOpener(Cbk cbk)
	throw ():
		Utils::Thread(__FUNCTION__),
		mLog(getName()),
		mCbk(cbk),
		mSem(0, 1),
		mDone(false)
	{}

	~SerialPortOpener() {
		stop();
	}

	void onStop() {
		mSem.post();
	}

	void start()
	throw (Utils::Error)
	{
		Utils::Thread::start();
	}

	void stop() {
		Utils::Thread::stop();
	}

	bool isDone() const {return mDone;}
private:
	// Objects
	Utils::Logger					mLog;
	Cbk								mCbk;
	Utils::Semaphore				mSem;
	bool							mDone;

	/**
	 * Thread function to retry until open is done
	 */
	void loop() {
		while (isAlive() && !isDone()) {
			try {
				if (mCbk()) {
					setDone();
				} else {
					*mLog.debug() << UTILS_STR_FUNCTION << ", retry in "
						<< SERIAL_PORT_OPENER_ERROR_TM_MS << " ms";
					idle(SERIAL_PORT_OPENER_ERROR_TM_MS);
				}
			} catch (std::exception& e) {
				*mLog.error() << UTILS_STR_FUNCTION << ", error: " << e.what();
			}
		}
		*mLog.debug() << UTILS_STR_FUNCTION << ", done";
	}

	void setDone() {mDone = true;}

	void idle(uint32_t timeMs) {
		mSem.wait(timeMs);
	}
};

///////////////////// SerialPortCommands /////////////////////
void SerialPortCommandWrite::execute() {
	Application::get().getSerial().onWrite(std::move(mData));
}

void SerialPortCommandClosed::execute() {
	Application::get().getSerial().onClosed(mCause);
}

///////////////////// SerialPort /////////////////////
SerialPort::SerialPort()
throw ()
:
	mLog(__FUNCTION__),
	mCtx(new SerialPortContext(mLog.getName()))
{
}

SerialPort::~SerialPort()
throw ()
{
	delete mCtx;
}

void SerialPort::start()
throw (Utils::Error)
{
	std::lock_guard<std::recursive_mutex> locker(mCtx->mtx);
	try {
			// Start processor
			mCtx->processor.start();
			// Start Port processing
			mCtx->ioService.start();
			startOpener();
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(SerialPort));
	}
}

void SerialPort::stop()
throw ()
{
	std::lock_guard<std::recursive_mutex> locker(mCtx->mtx);
	mCtx->processor.stop();
	mCtx->ioService.stop();
	mCtx->portOpener.reset();
	mCtx->serial.reset();
}

void SerialPort::write(std::unique_ptr< std::vector<uint8_t> > buffer)
throw ()
{
	std::unique_ptr<Utils::Command> cmd (new SerialPortCommandWrite(
			std::move(buffer)));
	mCtx->processor.process(std::move(cmd));
}

void SerialPort::onWrite(std::unique_ptr< std::vector<uint8_t> > buffer)
throw ()
{
	std::lock_guard<std::recursive_mutex> locker(mCtx->mtx);
	if (mCtx->serial && mCtx->serial->portWriter) {
		mCtx->serial->portWriter->write(std::move(buffer));
	} else {
		*mLog.debug() << UTILS_STR_FUNCTION << ", writer is not available => skip";
	}
}

void SerialPort::startOpener()
throw ()
{
	mCtx->portOpener.reset(new SerialPortOpener(
			[this] (void) -> bool {
				return onOpen();
			}
	));
	mCtx->portOpener->start();
}

bool SerialPort::onOpen()
throw ()
{
	std::lock_guard<std::recursive_mutex> locker(mCtx->mtx);
	bool res = false;
	try {
		try {
			// Prepare
			mCtx->portName = Utils::Configuration::get().serial.name;
			mCtx->portBaud = Utils::Configuration::get().serial.baud;
			*mLog.info() << "Opening port: " << mCtx->portName
					<< " at " << mCtx->portBaud;
			mCtx->serial.reset(new SerialPortContext::Serial);
			mCtx->serial->port.reset(new boost::asio::serial_port(mCtx->ioService.getIoService()));
			mCtx->serial->port->open(mCtx->portName);
			mCtx->serial->port->set_option(boost::asio::serial_port_base::baud_rate(mCtx->portBaud));
			mCtx->serial->port->set_option(boost::asio::serial_port_base::character_size(8));
			mCtx->serial->port->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
			mCtx->serial->port->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
			mCtx->serial->port->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
			*mLog.info() << "Port is opened";
			mCtx->serial->portReader.reset(new SerialPortReader(*mCtx));
			mCtx->serial->portWriter.reset(new SerialPortWriter(*mCtx));
			res = true;
		} catch (boost::system::system_error e) {
			throw Utils::Error(e);
		}
	} catch (Utils::Error& e) {
		*mLog.error() << UTILS_STR_FUNCTION << ", error: " << e.what();
	}
	return res;
}

void SerialPort::onClosed(const std::string& cause)
throw ()
{
	std::lock_guard<std::recursive_mutex> locker(mCtx->mtx);
	try {
		*mLog.debug() << UTILS_STR_FUNCTION << ", cause: " << cause;
		*mLog.info() << "Renew port";
		startOpener();
	} catch (Utils::Error& e) {
		*mLog.error() << UTILS_STR_FUNCTION << ", error: " << e.what();
	}
}
