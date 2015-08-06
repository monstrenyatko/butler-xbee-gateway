/*
 *******************************************************************************
 *
 * Purpose: Routing between networks
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014-2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Router.h"
#include "NetworkingDataUnit.h"
#include "CommandProcessor.h"
#include "Error.h"
#include "Application.h"
#include "Configuration.h"
#include "XBeeNet.h"
#include "TcpNet.h"
#include "SerialPort.h"
/* External Includes */
/* System Includes */


///////////////////// RouterContext /////////////////////
struct RouterContext {
	Utils::CommandProcessor							processor;
	RouterContext(const std::string& name) : processor(name) {}
};

///////////////////// RouterCommands /////////////////////
class RouterCommands: public Utils::Command {
public:
	virtual ~RouterCommands() {}
};

class RouterCommandsProcess: public RouterCommands {
public:
	typedef std::function<void(std::unique_ptr<Networking::DataUnit>)> Cbk;
	RouterCommandsProcess(Cbk cbk, std::unique_ptr<Networking::DataUnit> data)
	:
		mCbk(cbk),
		mData(std::move(data))
	{}

	void execute() {
		mCbk(std::move(mData));
	}
private:
	Cbk											mCbk;
	std::unique_ptr<Networking::DataUnit>		mData;
};

///////////////////// Router /////////////////////
Router::Router()
:
	mLog(__FUNCTION__),
	mCtx(new RouterContext(mLog.getName()))
{}

Router::~Router() {
	delete mCtx;
}

void Router::start() {
	mCtx->processor.start();
}

void Router::stop() {
	mCtx->processor.stop();
}

void Router::process(std::unique_ptr<Networking::DataUnit> unit) {
	std::unique_ptr<Utils::Command> cmd (new RouterCommandsProcess(
		[this] (std::unique_ptr<Networking::DataUnit> a) {
				onProcess(std::move(a));
		},
		std::move(unit)
	));
	mCtx->processor.process(std::move(cmd));
}

///////////////////// Router::Internal /////////////////////
void Router::onProcess(std::unique_ptr<Networking::DataUnit> unit) {
	try {
		try {
			switch(unit->getOrigin()) {
				case Networking::Origin::SERIAL:
				{
					Networking::DataUnitSerial* u = dynamic_cast<Networking::DataUnitSerial*>(unit.get());
					if (!u) {
						throw Utils::Error("Wrong unit type");
					}
					Application::get().getXBeeNet().from(u->popData());
				}
					break;
				case Networking::Origin::XBEE_ENCODER:
				{
					Networking::DataUnitXBeeEncoder* u = dynamic_cast<Networking::DataUnitXBeeEncoder*>(unit.get());
					if (!u) {
						throw Utils::Error("Wrong unit type");
					}
					Application::get().getSerial().write(u->popData());
				}
					break;
				case Networking::Origin::XBEE:
				{
					Networking::DataUnitXBee* u = dynamic_cast<Networking::DataUnitXBee*>(unit.get());
					if (!u) {
						throw Utils::Error("Wrong unit type");
					}
					Networking::AddressTcp to(
						{
							Utils::Configuration::get().tcp.address,
							Utils::Configuration::get().tcp.port
						}
					);
					Application::get().getTcpNet().send(u->getFrom(), &to, u->popData());
				}
					break;
				case Networking::Origin::TCP:
				{
					Networking::DataUnitTcp* u = dynamic_cast<Networking::DataUnitTcp*>(unit.get());
					if (!u) {
						throw Utils::Error("Wrong unit type");
					}
					Application::get().getXBeeNet().to(u->getFrom(), u->getTo(), u->popData());
				}
					break;
				default:
					throw Utils::Error("Origin is not implemented");
			}
		} catch (std::exception& e) {
			throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Router));
		}
	} catch (std::exception& e) {
		*mLog.error() << UTILS_STR_FUNCTION << ", error: "<<e.what();
	}
}
