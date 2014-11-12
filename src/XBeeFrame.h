/*
 *******************************************************************************
 *
 * Purpose: XBee network Frame
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

#ifndef XBEE_FRAME_H_
#define XBEE_FRAME_H_

/* Internal Includes */
#include "Error.h"
/* External Includes */
/* System Includes */
#include <stdint.h>
#include <vector>
#include <memory>

#define XBEE_FRAME_LENGTH_MAX 512

typedef std::vector<uint8_t> XBeeBuffer;

class XBeeFrameDelimiter {
public:
	typedef uint8_t type;
	static const type VALUE = 0x7E;

	XBeeFrameDelimiter() {}
	XBeeFrameDelimiter(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
	void encode(XBeeBuffer&);
	type getValue() const { return VALUE; }
private:
	void decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
};

class XBeeFrameLength {
public:
	typedef uint16_t type;

	XBeeFrameLength(type) throw (Utils::Error);
	XBeeFrameLength(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
	void encode(XBeeBuffer&);
	type getValue() const { return mValue; }
private:
	type		mValue;

	void decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
};

class XBeeFrameApiId {
public:
	enum type {
		ZB_TX_REQ			= 0x10,
		ZB_RX_RSP			= 0x90,
	};

	XBeeFrameApiId(type) throw (Utils::Error);
	XBeeFrameApiId(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
	void encode(XBeeBuffer&);
	type getValue() const { return mValue; }
private:
	type		mValue;

	void check() throw (Utils::Error);
	void decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
};

class XBeeFrameId {
public:
	typedef uint8_t type;
	static const type NO_RSP = 0; // no response is requested

	XBeeFrameId(type) throw (Utils::Error);
	XBeeFrameId(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
	void encode(XBeeBuffer&);
	type getValue() const { return mValue; }
private:
	type		mValue;

	void decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
};

class XBeeFrameAddr64 {
public:
	typedef uint64_t type;

	XBeeFrameAddr64(type) throw (Utils::Error);
	XBeeFrameAddr64(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
	void encode(XBeeBuffer&);
	type getValue() const { return mValue; }
private:
	type		mValue;

	void decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
};

class XBeeFrameAddr64Src: public XBeeFrameAddr64 {
public:
	static const type UNKNOWN = 0xFFFFFFFFFFFFFFFF;

	XBeeFrameAddr64Src(type v) throw (Utils::Error)
		:XBeeFrameAddr64(v) {}
	XBeeFrameAddr64Src(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer) throw (Utils::Error)
		:XBeeFrameAddr64(cursor, buffer) {}
};

class XBeeFrameAddr64Dst: public XBeeFrameAddr64 {
public:
	static const type COORDINATOR = 0x0000000000000000;
	static const type BROADCAST = 0x000000000000FFFF;

	XBeeFrameAddr64Dst(type v) throw (Utils::Error)
		:XBeeFrameAddr64(v) {}
	XBeeFrameAddr64Dst(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer) throw (Utils::Error)
		:XBeeFrameAddr64(cursor, buffer) {}
};

class XBeeFrameAddr16 {
public:
	typedef uint16_t type;
	static const type UNKNOWN = 0xFFFE; // address is unknown

	XBeeFrameAddr16(type) throw (Utils::Error);
	XBeeFrameAddr16(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
	void encode(XBeeBuffer&);
	type getValue() const { return mValue; }
private:
	type		mValue;

	void decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
};

class XBeeFrameAddr16Src: public XBeeFrameAddr16 {
public:
	XBeeFrameAddr16Src(type v) throw (Utils::Error)
		:XBeeFrameAddr16(v) {}
	XBeeFrameAddr16Src(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer) throw (Utils::Error)
		:XBeeFrameAddr16(cursor, buffer) {}
};

class XBeeFrameAddr16Dst: public XBeeFrameAddr16 {
public:
	static const type TO_ALL_ROUTERS = 0xFFFC; // broadcast to all routers
	static const type TO_ALL_NON_SLEPPY = 0xFFFD; // broadcast to all non-sleepy devices;
	static const type BROADCAST = 0xFFFF; // broadcast to all devices including sleepy ED.

	XBeeFrameAddr16Dst(type v) throw (Utils::Error)
		:XBeeFrameAddr16(v) {}
	XBeeFrameAddr16Dst(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer) throw (Utils::Error)
		:XBeeFrameAddr16(cursor, buffer) {}
};

class XBeeFrameOptions {
public:
	void encode(XBeeBuffer&);
protected:
	typedef uint8_t type;
	void set(type v) { mValue |= v; }
	void unset(type v) { mValue &= (~v); }
	bool isSet(type v) const { return mValue & v; }

	XBeeFrameOptions() throw (Utils::Error);
	XBeeFrameOptions(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
private:
	type		mValue;

	void decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
};

class XBeeFrameOptionsRecv: public XBeeFrameOptions {
public:
	enum bit {
		PKT_ACKED			= 0x01, // Packet Acknowledged
		PKT_BROADCAST		= 0x02, // Packet was a broadcast packet
		PKT_ENCRYPTED_APS	= 0x20, // Packet encrypted with APS encryption
		PKT_FROM_ENDDEV		= 0x40, // Packet was sent from an end device (if known)
	};
	void set(bit v) { XBeeFrameOptions::set(v); }
	void unset(bit v) { XBeeFrameOptions::unset(v); }
	bool isSet(bit v) const { return XBeeFrameOptions::isSet(v); }

	XBeeFrameOptionsRecv() throw (Utils::Error) {}
	XBeeFrameOptionsRecv(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer) throw (Utils::Error)
			:XBeeFrameOptions(cursor, buffer) {}
private:
	type		mValue;

	void decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
};

class XBeeFrameOptionsSend: public XBeeFrameOptions {
public:
	typedef uint8_t type;
	enum bit {
		DISABLE_RETRIES_REPAIR	= 0x01, // Disable retries and route repair
		ENABLE_ENCRYPTION_APS	= 0x20, // Enable APS encryption. (if EE=1)
										// Enabling APS encryption decreases the maximum number
										// of RF payload bytes by 4 (below the value reported by NP).
		USE_EXTENDED_TX_TIMEOUT	= 0x40, // Use the extended transmission timeout for this destination
	};
	void set(bit v) { XBeeFrameOptions::set(v); }
	void unset(bit v) { XBeeFrameOptions::unset(v); }
	bool isSet(bit v) const { return XBeeFrameOptions::isSet(v); }

	XBeeFrameOptionsSend() throw (Utils::Error) {}
	XBeeFrameOptionsSend(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer) throw (Utils::Error)
			:XBeeFrameOptions(cursor, buffer) {}
private:
	type		mValue;

	void decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
};

class XBeeFrameRadius {
public:
	typedef uint8_t type;
	static const type MAX = 0; // the network maximum hops value will be used

	XBeeFrameRadius(type) throw (Utils::Error);
	XBeeFrameRadius(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
	void encode(XBeeBuffer&);
	type getValue() const { return mValue; }
private:
	type		mValue;

	void decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
};

class XBeeFrameData {
public:
	XBeeFrameData(std::unique_ptr<XBeeBuffer>);
	XBeeFrameData(XBeeBuffer::const_iterator begin, XBeeBuffer::const_iterator end);
	XBeeFrameData(XBeeBuffer::const_iterator& cursor, XBeeFrameLength::type length, const XBeeBuffer&) throw (Utils::Error);
	void encode(XBeeBuffer&);
	const XBeeBuffer& getValue() const { return mValue; }
private:
	XBeeBuffer		mValue;

	void decode(XBeeBuffer::const_iterator& cursor, XBeeFrameLength::type length, const XBeeBuffer&) throw (Utils::Error);
};

/**
 * XBee network frame
 */
class XBeeFrame {
public:
	/**
	 * Constructor
	 *
	 * @param apiId XBee network Frame API Id
	 */
	XBeeFrame(std::unique_ptr<XBeeFrameApiId> apiId);

	/**
	 * Constructor
	 *
	 * @param buffer XBee network buffer in API 2 mode
	 */
	XBeeFrame(const XBeeBuffer& frame) throw (Utils::Error);

	/**
	 * Encodes the XBee network Frame to the given buffer
	 */
	void encode(XBeeBuffer&) throw (Utils::Error);

	// Getters
	const XBeeFrameApiId* getApiId() const { return mApiId.get(); }
	const XBeeFrameAddr64Src* getAddr64Src() const { return mAddr64Src.get(); }
	const XBeeFrameAddr16Src* getAddr16Src() const { return mAddr16Src.get(); }
	const XBeeFrameData* getData() const { return mData.get(); }
	// Setters
	void setId(std::unique_ptr<XBeeFrameId> v) { mId = std::move(v); }
	void setRadius(std::unique_ptr<XBeeFrameRadius> v) { mRadius = std::move(v); }
	void setAddr64Dst(std::unique_ptr<XBeeFrameAddr64Dst> v) { mAddr64Dst = std::move(v); }
	void setAddr16Dst(std::unique_ptr<XBeeFrameAddr16Dst> v) { mAddr16Dst = std::move(v); }
	void setOptionsSend(std::unique_ptr<XBeeFrameOptionsSend> v) { mOptionsSend = std::move(v); }
	void setData(std::unique_ptr<XBeeFrameData> v) { mData = std::move(v); }

private:
	std::unique_ptr<XBeeFrameDelimiter>				mDelimiter;
	std::unique_ptr<XBeeFrameLength>				mLength;
	std::unique_ptr<XBeeFrameApiId>					mApiId;
	std::unique_ptr<XBeeFrameId>					mId;
	std::unique_ptr<XBeeFrameRadius>				mRadius;
	std::unique_ptr<XBeeFrameAddr64Src>				mAddr64Src;
	std::unique_ptr<XBeeFrameAddr16Src>				mAddr16Src;
	std::unique_ptr<XBeeFrameOptionsRecv>			mOptionsRecv;
	std::unique_ptr<XBeeFrameAddr64Dst>				mAddr64Dst;
	std::unique_ptr<XBeeFrameAddr16Dst>				mAddr16Dst;
	std::unique_ptr<XBeeFrameOptionsSend>			mOptionsSend;
	std::unique_ptr<XBeeFrameData>					mData;

	void decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer&) throw (Utils::Error);
	void checkNull(void*, const std::string& name) throw (Utils::Error);
};

#endif /* XBEE_FRAME_H_ */
