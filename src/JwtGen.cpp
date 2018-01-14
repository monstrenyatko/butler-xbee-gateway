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
/* Internal Includes */
#include "JwtGen.h"
#include "Configuration.h"
/* External Includes */
#include "jwt.h"
/* System Includes */


namespace Utils {

JwtGen::JwtGen()
:
	mLog(__FUNCTION__)
{
}

JwtGen::~JwtGen()
{
}

std::string JwtGen::get(const std::string& username, uint32_t exp)
{
	*mLog.info() << "Generating token, user: " << username;
	std::string res;
	jwt_t *jwt = NULL;
	if (0 == jwt_new(&jwt)) {
		if (0 == jwt_add_grant(jwt, "user", username.c_str())) {
			if (0 == jwt_add_grant_int(jwt, "exp", exp)) {
				const std::string& key = Utils::Configuration::get().jwt.key;
				if (0 == jwt_set_alg(jwt, JWT_ALG_HS256, reinterpret_cast<const unsigned char*>(key.c_str()), static_cast<int>(key.length()))) {
					char *out = jwt_encode_str(jwt);
					res = out;
					free(out);
					jwt_free(jwt);
				} else {
					*mLog.error() << "Can't set JWY algorithm";
				}
			} else {
				*mLog.error() << "Can't add 'exp' field to JWT object";
			}
		} else {
			*mLog.error() << "Can't add 'user' field to JWT object";
		}
	} else {
		*mLog.error() << "Can't allocate JWT object";
	}
	return res;
}

} /* namespace Utils */
