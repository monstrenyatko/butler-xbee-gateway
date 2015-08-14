/*
 *******************************************************************************
 *
 * Purpose: Utils. Error class implementation.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef UTILS_ERROR_H_
#define UTILS_ERROR_H_

/* Internal Includes */
#include "Strings.h"
/* External Includes */
/* System Includes */
#include <string>


namespace Utils {

namespace ErrorCode {
	enum Type {
		OK,
		TIME_OUT,
		UNSPECIFIED,
	};
}

class Error: public std::exception
{
private:
	const char*				mSep = " -> ";
public:
	Error(const std::string& msg):mCode(ErrorCode::UNSPECIFIED), mWhat(msg) {}
	Error(ErrorCode::Type code, const std::string& msg):mCode(code), mWhat(msg) {}
	Error(const Error& e):mCode(e.mCode), mWhat(e.what()) {}
	Error(const Error& e, const std::string& msg):mCode(e.mCode), mWhat(msg+mSep+e.what()) {}
	Error(const std::exception& e):mWhat(e.what()) {
		mCode = isMe(e) ? ((Error&)e).mCode : ErrorCode::UNSPECIFIED;
	}
	Error(const std::exception& e, const std::string& msg):mWhat(msg+mSep+e.what()) {
		mCode = isMe(e) ? ((Error&)e).mCode : ErrorCode::UNSPECIFIED;
	}
	Error(ErrorCode::Type code, const std::exception& e, const std::string& msg):mCode(code), mWhat(msg+mSep+e.what()) {}
	virtual ~Error() throw() {}
	const char* what() const throw() {
		return mWhat.c_str();
	}
	ErrorCode::Type getCode(void) const {return mCode;}
private:
	ErrorCode::Type			mCode;
	std::string				mWhat;

	inline bool isMe(const std::exception& e) {
		const Error* me = dynamic_cast<const Error*> (&e);
		return me != NULL;
	}
};

} /* namespace Utils */

#endif /* UTILS_ERROR_H_ */
