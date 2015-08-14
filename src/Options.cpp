/*
 *******************************************************************************
 *
 * Purpose: Utils. Application command-line options implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Options.h"
#include "Configuration.h"
#include "Error.h"
#include "Logger.h"
/* External Includes */
/* System Includes */
#include <boost/program_options.hpp>


namespace Utils {

Options*				Options::mInstance = NULL;
std::mutex				Options::mMtxInstance;

Options::Options()
:
	mLog(__FUNCTION__)
{
}

Options& Options::get()
throw (Utils::Error)
{
	std::lock_guard<std::mutex> locker(mMtxInstance);
	if (!mInstance)
	{
		mInstance = new Options();
		if (!mInstance) {
			throw Utils::Error("Options are not initialized");
		}
	}
	return *mInstance;
}

void Options::destroy()
throw ()
{
	try {
		std::lock_guard<std::mutex> locker(mMtxInstance);
		if (mInstance)
		{
			Options* tmp = mInstance;
			mInstance = NULL;
			delete tmp;
		}
	} catch (std::exception& e) {
		// It must not happen
		// We can't use logger When the instance is destroyed => ignore
	}
}

void Options::process(int argc, char* argv[])
throw (Utils::Error)
{
	boost::program_options::options_description desc("Application options");
	std::string opConf;
	desc.add_options()
		("help,h", "Show help")
		("config,c", boost::program_options::value<std::string>(&opConf)->required(), "Configuration file name")
	;
	try {
		boost::program_options::parsed_options parsed = boost::program_options
			::command_line_parser(argc, argv).options(desc).run();
		boost::program_options::variables_map vm;
		boost::program_options::store(parsed, vm);
		if (argc <= 1 || vm.count("help")) {
			throw Utils::Error(Utils::ErrorCode::OK, "help has been requested");
		} else {
			boost::program_options::notify(vm);
			if (vm.count("config")) {
				Configuration::get().main.configFileName = opConf;
			}
		}
	} catch (std::exception& e) {
		// Print help
		*mLog.info() << desc;
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Options));
	}
}

} /* namespace Utils */
