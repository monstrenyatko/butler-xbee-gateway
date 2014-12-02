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
#include <memory>


namespace Networking {

class Address {
public:
	virtual ~Address() {}

	Origin::Type getOrigin() const {return mOrigin;}
	virtual bool isEqual(const Address& v) const {return mOrigin==v.mOrigin;}
	virtual std::unique_ptr<Address> clone() const = 0;
protected:
	Address(Origin::Type o): mOrigin(o) {}
private:
	Origin::Type		mOrigin;
};

template<enum Origin::Type tOrigin, class tVal> class AddressImpl: public Address {
public:
	virtual ~AddressImpl() {}
	AddressImpl(const tVal& v): Address(tOrigin), mVal(v) {}

	const tVal& get() const {return mVal;}
	virtual std::unique_ptr<Address> clone() const
			{return std::unique_ptr<Address>(new AddressImpl(*this));}
	virtual bool isEqual(const Address& v) const {
		bool res = false;
		const AddressImpl* addr = dynamic_cast<const AddressImpl*>(&v);
		if (addr) { res = (Address::isEqual(v) && mVal==addr->mVal); }
		return res;
	}
private:
	tVal				mVal;
};

typedef std::string												AddressSerialValT;
typedef uint64_t												AddressXbeeValT;
typedef struct AddressTcpValT_ {
	AddressTcpValT_(const std::string& h, uint32_t p):host(h), port(p) {}
	bool operator==(const AddressTcpValT_& v) const {
		return host==v.host && port==v.port;
	}
	std::string		host;
	uint32_t		port;
} AddressTcpValT;

typedef AddressImpl<Origin::SERIAL, AddressSerialValT>			AddressSerial;
typedef AddressImpl<Origin::XBEE, AddressXbeeValT>				AddressXBeeNet;
typedef AddressImpl<Origin::TCP, AddressTcpValT>				AddressTcp;


} /* namespace Networking */

#endif /* NETWORKING_ADDRESS_H_ */
