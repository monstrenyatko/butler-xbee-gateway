/*
 *******************************************************************************
 *
 * Purpose: Main function.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Application.h"
#include "Logger.h"
#include "LogManager.h"
#include "Configuration.h"
#include "Options.h"
#include "Version.h"
/* External Includes */
/* System Includes */


int main(int argc, char* argv[]) {
	Utils::Logger log("Main");
	try {
		*log.info() << "START";
		*log.info() << "Version: " << Version::value;
		Utils::Options::get().process(argc, argv);
		Utils::Configuration::get().load();
		Utils::Configuration::get().loadEnv();
		Utils::Configuration::get().dump();
		Utils::LogManager::get().start();
		Application::initialize();
		Application::get().run();
		Application::destroy();
		Utils::Options::destroy();
	} catch (Utils::Error& e) {
		if (e.getCode() == Utils::ErrorCode::OK) {
			*log.info() << e.what();
		} else {
			*log.error() << e.what();
		}
	} catch (std::exception& e) {
		*log.error() << e.what();
	}
	*log.info() << "EXIT";
	// It was the last log => flush everything in logger
	Utils::Configuration::destroy();
	Utils::LogManager::destroy();
	return 0;
}
