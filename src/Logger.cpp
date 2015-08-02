/*
 *******************************************************************************
 *
 * Purpose: Utils. Logger endpoint implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2015.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Logger.h"
#include "LogManager.h"
/* External Includes */
/* System Includes */

namespace Utils {

LogStream& LogStream::flush() {
	std::string log(str());
	if (!log.empty()) {
		LogManager::get().log(getLevel(), getName(), log);
		str(std::string());
	}
	return *this;
}

} /* namespace Utils */
