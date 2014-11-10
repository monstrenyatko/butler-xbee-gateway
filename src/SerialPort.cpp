/*
 *******************************************************************************
 *
 * Purpose: Serial port read/write implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "SerialPort.h"
#include "Thread.h"
#include "CommandProcessor.h"
#include "Commands.h"
#include "Error.h"
#include "Application.h"
/* External Includes */
/* System Includes */
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


#define SERIAL_PORT_READER_BUFFER_SIZE 512

///////////////////// SerialPortReader /////////////////////
class SerialPortReader : private Utils::Thread {
public:
	SerialPortReader(boost::asio::serial_port& port)
	throw ():
			mPort(port)
	{
	}

	void onStarted() {
		schedule();
	}

	void onStop() {
		try {
			mPort.cancel();
			mPort.close();
			mPort.get_io_service().stop();
			mPort.get_io_service().reset();
		} catch (boost::system::system_error& e) {
			// ignore
		}
	}

	void start()
	throw (Utils::Error)
	{
		Utils::Thread::start();
	}

	void stop() {
		Utils::Thread::stop();
	}
private:
	// Objects
	boost::asio::serial_port&		mPort;
	uint8_t							mBufferRead[SERIAL_PORT_READER_BUFFER_SIZE];

	// Methods
	/**
	 * Function is called on read operation finish.
	 */
	void onReceive(const boost::system::error_code& error, std::size_t qty) {
		if (qty) {
			std::unique_ptr< std::vector<uint8_t> > data
						(new std::vector<uint8_t>(mBufferRead,mBufferRead+qty));
			std::unique_ptr<Utils::Command> cmd (new CommandSerialInput(std::move(data)));
			Application::get().getProcessor().process(std::move(cmd));
		}
		try {
			if (!qty && error) {
				throw Utils::Error(error.message());
			}
			schedule();
		} catch (Utils::Error& e) {
			std::unique_ptr<Utils::Command> cmd (new CommandSerialClose(e.what()));
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

	/**
	 * Thread function to power up the Serial
	 */
	void loop() {
		while (isAlive()) {
			try {
				mPort.get_io_service().reset();
				mPort.get_io_service().run_one();
			} catch (boost::system::system_error& e) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	}
};

///////////////////// SerialPortContext /////////////////////
struct SerialPortContext {
	boost::asio::io_service							ioService;
	std::unique_ptr<boost::asio::serial_port>		port;
	std::string										portName;
	uint32_t										portBaud;
	std::unique_ptr<SerialPortReader>				portReader;
};

///////////////////// SerialPort /////////////////////
SerialPort::SerialPort(const std::string& name, uint32_t baud)
throw ():
		mCtx(new SerialPortContext)
{
	mCtx->port.reset(new boost::asio::serial_port(mCtx->ioService));
	mCtx->portName = name;
	mCtx->portBaud = baud;
	mCtx->portReader.reset(new SerialPortReader(*mCtx->port));
}

SerialPort::~SerialPort()
throw ()
{
	stop();
	delete mCtx;
}

void SerialPort::start()
throw (Utils::Error)
{
	try {
		try {
			// Prepare
			mCtx->port->open(mCtx->portName);
			mCtx->port->set_option(boost::asio::serial_port_base::baud_rate(mCtx->portBaud));
			mCtx->port->set_option(boost::asio::serial_port_base::character_size(8));
			mCtx->port->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
			mCtx->port->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
			mCtx->port->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
			// Start reading
			mCtx->portReader->start();
		} catch (boost::system::system_error e) {
			throw Utils::Error(e);
		}
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(SerialPort));
	}
}

void SerialPort::stop()
throw ()
{
	mCtx->portReader->stop();
}

std::size_t SerialPort::write(const uint8_t* data, uint32_t size)
throw (Utils::Error)
{
	try {
		return boost::asio::write(*mCtx->port, boost::asio::buffer(data, size));
	} catch (boost::system::system_error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(SerialPort));
	}
}
