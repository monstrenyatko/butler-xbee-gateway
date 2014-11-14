/*
 *******************************************************************************
 *
 * Purpose: Networking. The Address representation
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef NETWORKING_ADDRESS_H_
#define NETWORKING_ADDRESS_H_

/* Internal Includes */
#include "NetworkingDefs.h"
/* External Includes */
/* System Includes */
#include <string>
#include <stdint.h>

namespace Networking {

class Address {
public:
	virtual ~Address() {}

	Origin::Type getOrigin() {return mOrigin;}
protected:
	Address(Origin::Type o): mOrigin(o) {}
private:
	Origin::Type		mOrigin;
};

template<enum Origin::Type tOrigin, class tVal> class AddressImpl: public Address {
public:
	virtual ~AddressImpl() {}
	AddressImpl(const tVal& v): Address(tOrigin), mVal(v) {}

	tVal& get() {return mVal;}
private:
	tVal				mVal;
};

typedef AddressImpl<Origin::SERIAL, std::string>			AddressSerial;
typedef AddressImpl<Origin::XBEE_NET, uint64_t>				AddressXBeeNet;

} /* namespace Networking */

#endif /* NETWORKING_ADDRESS_H_ */
