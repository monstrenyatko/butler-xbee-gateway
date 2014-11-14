/*
 *******************************************************************************
 *
 * Purpose: Networking. The data unit to be routed between networks
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef NETWORKING_DATA_UNIT_H_
#define NETWORKING_DATA_UNIT_H_

/* Internal Includes */
#include "NetworkingDefs.h"
#include "NetworkingAddress.h"
/* External Includes */
/* System Includes */
#include <memory>
#include <vector>
#include <string>
#include <stdint.h>

namespace Networking {

class DataUnit {
public:
	virtual ~DataUnit() {}

	Origin::Type getOrigin() {return mOrigin;}
	Address* getFrom() {return mFrom.get();}
	Address* getTo() {return mTo.get();}
protected:
	DataUnit(Origin::Type o, std::unique_ptr<Address> from, std::unique_ptr<Address> to)
	: mOrigin(o), mFrom(std::move(from)), mTo(std::move(to)) {}
private:
	Origin::Type					mOrigin;
	std::unique_ptr<Address>		mFrom;
	std::unique_ptr<Address>		mTo;
};

template<enum Origin::Type tOrigin> class DataUnitImpl: public DataUnit {
public:
	virtual ~DataUnitImpl() {}
	DataUnitImpl(
			std::unique_ptr<Buffer> data,
			std::unique_ptr<Address> from, std::unique_ptr<Address> to)
	:
		DataUnit(tOrigin, std::move(from), std::move(to)),
		mData(std::move(data)) {}

	std::unique_ptr<Buffer> popData() {return std::move(mData);}
private:
	std::unique_ptr<Buffer>			mData;
};

typedef DataUnitImpl<Origin::SERIAL>				DataUnitSerial;
typedef DataUnitImpl<Origin::XBEE_ENCODER>			DataUnitXBeeEncoder;
typedef DataUnitImpl<Origin::XBEE_NET>				DataUnitXBeeNet;

} /* namespace Networking */

#endif /* NETWORKING_DATA_UNIT_H_ */
