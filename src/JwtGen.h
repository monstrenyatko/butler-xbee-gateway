/*
 *******************************************************************************
 *
 * Purpose: Utils. JWT generator.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2018.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef UTILS_JWT_GEN_H_
#define UTILS_JWT_GEN_H_

/* Internal Includes */
#include "Logger.h"
/* External Includes */
/* System Includes */
#include <string>


namespace Utils {

/**
 * JWT token generator.
 */
class JwtGen {
public:
	/**
	 * Constructor
	 */
	JwtGen();

	/**
	 * Destructor
	 */
	~JwtGen();
	/**
	 * Generates token value.
	 *
	 * @return token value
	 */
	std::string get(const std::string& username, uint32_t exp);
private:
	// Objects
	Utils::Logger									mLog;

	// Do not copy
	JwtGen(const JwtGen&);
	JwtGen &operator=(const JwtGen&);
};

} /* namespace Utils */

#endif /* UTILS_JWT_GEN_H_ */
