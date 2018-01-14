/*
 *******************************************************************************
 *
 * Purpose: MQTT implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2018.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Mqtt.h"
#include "JwtGen.h"
#include "Configuration.h"
/* External Includes */
#include "TcpNetConnection.h"
#include "MQTTPacket.h"
/* System Includes */
#include <string>
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

static Utils::JwtGen jwtGen;

Mqtt::Mqtt()
:
	mLog(__FUNCTION__)
{
}

Mqtt::~Mqtt()
{
}

void Mqtt::closeOnConnect(Networking::Buffer& buffer, TcpNetConnection** connection_p)
{
	TcpNetConnection* connection = *connection_p;
	bool resetOnConnect = Utils::Configuration::get().mqtt.resetOnConnect;
	if (connection && resetOnConnect) {
		MQTTPacket_connectData message = MQTTPacket_connectData_initializer;
		if (MQTTDeserialize_connect(&message, static_cast<unsigned char*>(&(buffer[0])), static_cast<std::size_t>(buffer.size()))) {
			*mLog.info() << "MQTT_CONNECT => Reestablish Connection";
			connection->close();
			*connection_p = nullptr;
		}
	}
}

void Mqtt::forceAuth(Networking::Buffer& buffer, TcpNetConnection& connection)
{
	bool forceAuth = Utils::Configuration::get().mqtt.forceAuth;
	if (forceAuth) {
		MQTTPacket_connectData message = MQTTPacket_connectData_initializer;
		if (MQTTDeserialize_connect(&message, static_cast<unsigned char*>(&(buffer[0])), static_cast<std::size_t>(buffer.size()))) {
			if (Utils::Configuration::get().jwt.key.empty()) {
				*mLog.warn() << "mqtt.force-auth is set => provide JWT key value";
			} else {
				// make JWT
				boost::posix_time::ptime time_epoch(boost::gregorian::date(1970,1,1));
				boost::posix_time::ptime time_now = boost::posix_time::second_clock::universal_time();
				uint32_t jwt_exp = (time_now - time_epoch).total_seconds() + Utils::Configuration::get().jwt.expirationSec;
				std::string jwt(jwtGen.get(connection.getFrom()->getValueString(), jwt_exp));
				// set JWT as user-name
				message.username = MQTTString_initializer;
				message.password = MQTTString_initializer;
				message.username.cstring = const_cast<char*>(jwt.c_str());
				message.password.cstring = const_cast<char*>("nopass");
				// encode message
				Networking::Buffer tmp_buffer(MQTTPacket_len(MQTTSerialize_connectLength(&message)));
				Networking::Buffer::size_type len = MQTTSerialize_connect(
						static_cast<unsigned char*>(&(tmp_buffer[0])),
						static_cast<int>(tmp_buffer.size()),
						&message
				);
				if (len > 0) {
					tmp_buffer.resize(len);
					buffer = std::move(tmp_buffer);
					connection.setProtocol(TcpNetProtocol::MQTT);
					connection.setExpiration(jwt_exp);
				} else {
					*mLog.warn() << "Can't encode the MQTT_CONNECT";
				}
			}
		}
	}
}

void Mqtt::closeOnExpire(TcpNetConnection** connection_p)
{
	TcpNetConnection* connection = *connection_p;
	if (connection && connection->getProtocol() == TcpNetProtocol::MQTT && connection->getExpiration() > 0) {
		boost::posix_time::ptime time_epoch(boost::gregorian::date(1970,1,1));
		boost::posix_time::ptime time_now = boost::posix_time::second_clock::universal_time();
		int64_t time_now_epoch = (time_now - time_epoch).total_seconds();
		if (time_now_epoch > 0 && static_cast<uint64_t>(time_now_epoch) > connection->getExpiration()) {
			*mLog.info() << "Connection expired => close";
			connection->close();
			*connection_p = nullptr;
		}
	}
}
