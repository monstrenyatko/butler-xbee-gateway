/*
 *******************************************************************************
 *
 * Purpose: Utils. Configuration loading implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Configuration.h"
#include "Error.h"
/* External Includes */
/* System Includes */
#include <boost/property_tree/json_parser.hpp>
#include <iostream>


Configuration::Configuration() {
	try {
		// read and pars configuration file
		boost::property_tree::ptree config;
		boost::property_tree::read_json("config.json", config);
		// get parameters
		try {
			serial.name = config.get<std::string>("serial.name");
			serial.baud = config.get<uint32_t>("serial.baud");
			tcp.address = config.get<std::string>("tcp.address");
			tcp.port = config.get<uint32_t>("tcp.port");
		} catch (const boost::property_tree::ptree_bad_data& e) {
			throw Utils::Error(e, "bad-data [" + e.data<std::string>() + "]");
		}
		// print
		dump();
	} catch (std::exception& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Configuration));
	}
}

Configuration::~Configuration() throw () {
}

void Configuration::dump() {
	std::cout<<"CONFIGURATION"<<std::endl;
	std::cout<<"\t"<<"serial.name = "<<serial.name<<std::endl;
	std::cout<<"\t"<<"serial.boud = "<<serial.baud<<std::endl;
	std::cout<<"\t"<<"tcp.address = "<<tcp.address<<std::endl;
	std::cout<<"\t"<<"tcp.port    = "<<tcp.port<<std::endl;
}
