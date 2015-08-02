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

/* Internal Includes */
#include "XBeeFrame.h"
/* External Includes */
/* System Includes */


///////////////////// XBeeFrameDelimiter /////////////////////
XBeeFrameDelimiter::XBeeFrameDelimiter(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		decode(cursor, buffer);
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameDelimiter));
	}
}

void XBeeFrameDelimiter::decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		if (std::distance(cursor, buffer.end())<1) {
			throw Utils::Error("Buffer is too short");
		}
		uint8_t b1 = *(cursor++);
		if (b1!=VALUE) {
			throw Utils::Error("Wrong value");
		}
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameDelimiter));
	}
}

void XBeeFrameDelimiter::encode(XBeeBuffer& buffer) {
	buffer.push_back(getValue());
}

///////////////////// XBeeFrameLength /////////////////////
XBeeFrameLength::XBeeFrameLength(type v)
throw (Utils::Error)
:
	mValue(v)
{
}

XBeeFrameLength::XBeeFrameLength(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
:
	mValue(0)
{
	try {
		decode(cursor, buffer);
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameLength));
	}
}

void XBeeFrameLength::decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		if (std::distance(cursor, buffer.end())<2) {
			throw Utils::Error("Buffer is too short");
		}
		uint8_t b1 = *(cursor++);
		uint8_t b2 = *(cursor++);
		mValue = ((((uint16_t)b1)<<8) & 0xFF00) | (b2 & 0x00FF);
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameLength));
	}
}

void XBeeFrameLength::encode(XBeeBuffer& buffer) {
	buffer.push_back((mValue>>8) & 0x00FF);
	buffer.push_back(mValue      & 0x00FF);
}

///////////////////// XBeeFrameApiId /////////////////////
XBeeFrameApiId::XBeeFrameApiId(type v)
throw (Utils::Error)
:
	mValue(v)
{
	try {
		check();
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameApiId));
	}
}

XBeeFrameApiId::XBeeFrameApiId(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		decode(cursor, buffer);
		check();
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameApiId));
	}
}

void XBeeFrameApiId::check()
throw (Utils::Error)
{
	try {
		switch(mValue) {
			case ZB_TX_REQ:
			case ZB_RX_RSP:
				break;
			default:
				throw Utils::Error("Not implemented");
		}
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameApiId));
	}
}

void XBeeFrameApiId::decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		if (std::distance(cursor, buffer.end())<1) {
			throw Utils::Error("Buffer is too short");
		}
		mValue = (type) (*(cursor++));
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameApiId));
	}
}

void XBeeFrameApiId::encode(XBeeBuffer& buffer) {
	buffer.push_back(mValue);
}

///////////////////// XBeeFrameId /////////////////////
XBeeFrameId::XBeeFrameId(type v)
throw (Utils::Error)
:
	mValue(v)
{
}

XBeeFrameId::XBeeFrameId(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		decode(cursor, buffer);
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameId));
	}
}

void XBeeFrameId::decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		if (std::distance(cursor, buffer.end())<1) {
			throw Utils::Error("Buffer is too short");
		}
		mValue = (type) (*(cursor++));
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameId));
	}
}

void XBeeFrameId::encode(XBeeBuffer& buffer) {
	buffer.push_back(mValue);
}

///////////////////// XBeeFrameAddr64 /////////////////////
XBeeFrameAddr64::XBeeFrameAddr64(type v)
throw (Utils::Error)
:
	mValue(v)
{
}

XBeeFrameAddr64::XBeeFrameAddr64(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
:
	mValue(0)
{
	try {
		decode(cursor, buffer);
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameAddr64));
	}
}

void XBeeFrameAddr64::decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		if (std::distance(cursor, buffer.end())<8) {
			throw Utils::Error("Buffer is too short");
		}
		uint8_t b1 = *(cursor++);
		uint8_t b2 = *(cursor++);
		uint8_t b3 = *(cursor++);
		uint8_t b4 = *(cursor++);
		uint8_t b5 = *(cursor++);
		uint8_t b6 = *(cursor++);
		uint8_t b7 = *(cursor++);
		uint8_t b8 = *(cursor++);
		mValue =
		((((uint64_t)b1)<<56) & 0xFF00000000000000) |
		((((uint64_t)b2)<<48) & 0x00FF000000000000) |
		((((uint64_t)b3)<<40) & 0x0000FF0000000000) |
		((((uint64_t)b4)<<32) & 0x000000FF00000000) |
		((((uint64_t)b5)<<24) & 0x00000000FF000000) |
		((((uint64_t)b6)<<16) & 0x0000000000FF0000) |
		((((uint64_t)b7)<< 8) & 0x000000000000FF00) |
		((((uint64_t)b8)    ) & 0x00000000000000FF)
		;
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameAddr64));
	}
}

void XBeeFrameAddr64::encode(XBeeBuffer& buffer) {
	buffer.push_back((mValue>>56) & 0xFF);
	buffer.push_back((mValue>>48) & 0xFF);
	buffer.push_back((mValue>>40) & 0xFF);
	buffer.push_back((mValue>>32) & 0xFF);
	buffer.push_back((mValue>>24) & 0xFF);
	buffer.push_back((mValue>>16) & 0xFF);
	buffer.push_back((mValue>> 8) & 0xFF);
	buffer.push_back((mValue    ) & 0xFF);
}

///////////////////// XBeeFrameAddr16 /////////////////////
XBeeFrameAddr16::XBeeFrameAddr16(type v)
throw (Utils::Error)
:
	mValue(v)
{
}

XBeeFrameAddr16::XBeeFrameAddr16(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
:
	mValue(0)
{
	try {
		decode(cursor, buffer);
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameAddr16));
	}
}

void XBeeFrameAddr16::decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		if (std::distance(cursor, buffer.end())<2) {
			throw Utils::Error("Buffer is too short");
		}
		uint8_t b1 = *(cursor++);
		uint8_t b2 = *(cursor++);
		mValue =
		((((uint16_t)b1)<<8) & 0xFF00) |
		((((uint16_t)b2)   ) & 0x00FF)
		;
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameAddr16));
	}
}

void XBeeFrameAddr16::encode(XBeeBuffer& buffer) {
	buffer.push_back((mValue>> 8) & 0xFF);
	buffer.push_back((mValue    ) & 0xFF);
}

///////////////////// XBeeFrameOptions /////////////////////
XBeeFrameOptions::XBeeFrameOptions()
throw (Utils::Error)
:
	mValue(0)
{
}

XBeeFrameOptions::XBeeFrameOptions(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
:
	mValue(0)
{
	try {
		decode(cursor, buffer);
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameOptions));
	}
}

void XBeeFrameOptions::decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		if (std::distance(cursor, buffer.end())<1) {
			throw Utils::Error("Buffer is too short");
		}
		mValue = *(cursor++);
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameOptions));
	}
}

void XBeeFrameOptions::encode(XBeeBuffer& buffer) {
	buffer.push_back(mValue);
}

///////////////////// XBeeFrameRadius /////////////////////
XBeeFrameRadius::XBeeFrameRadius(type v)
throw (Utils::Error)
:
	mValue(v)
{
}

XBeeFrameRadius::XBeeFrameRadius(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		decode(cursor, buffer);
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameRadius));
	}
}

void XBeeFrameRadius::decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		if (std::distance(cursor, buffer.end())<1) {
			throw Utils::Error("Buffer is too short");
		}
		mValue = (type) (*(cursor++));
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameRadius));
	}
}

void XBeeFrameRadius::encode(XBeeBuffer& buffer) {
	buffer.push_back(mValue);
}

///////////////////// XBeeFrameData /////////////////////
XBeeFrameData::XBeeFrameData(std::unique_ptr<XBeeBuffer> buffer)
{
	mValue = std::move(*buffer);
}

XBeeFrameData::XBeeFrameData(XBeeBuffer::const_iterator begin, XBeeBuffer::const_iterator end)
{
	mValue.assign(begin, end);
}

XBeeFrameData::XBeeFrameData(XBeeBuffer::const_iterator& cursor, XBeeFrameLength::type length, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		decode(cursor, length, buffer);
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameData));
	}
}

void XBeeFrameData::decode(XBeeBuffer::const_iterator& cursor, XBeeFrameLength::type length, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		if (std::distance(cursor, buffer.end())<length) {
			throw Utils::Error("Buffer is too short");
		}
		auto begin = cursor;
		cursor += length;
		mValue.assign(begin, cursor);
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrameData));
	}
}

void XBeeFrameData::encode(XBeeBuffer& buffer) {
	buffer.insert(buffer.end(), mValue.begin(), mValue.end());
}

///////////////////// XBeeFrame /////////////////////
XBeeFrame::XBeeFrame(std::unique_ptr<XBeeFrameApiId> apiId)
:
	mDelimiter(new XBeeFrameDelimiter),
	mApiId(std::move(apiId))
{
}

XBeeFrame::XBeeFrame(const XBeeBuffer& frame)
throw (Utils::Error)
{
	try {
		XBeeBuffer::const_iterator begin(frame.begin());
		decode(begin, frame);
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrame));
	}
}

void XBeeFrame::decode(XBeeBuffer::const_iterator& cursor, const XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		// Delimiter
		mDelimiter.reset(new XBeeFrameDelimiter(cursor, buffer));
		// Length
		mLength.reset(new XBeeFrameLength(cursor, buffer));
		// Save cursor place
		auto paylodStart = cursor;
		// API Id
		mApiId.reset(new XBeeFrameApiId(cursor, buffer));
		// API Data
		switch(mApiId->getValue()) {
			case XBeeFrameApiId::ZB_RX_RSP:
			{
				// Source Address 64
				mAddr64Src.reset(new XBeeFrameAddr64Src(cursor, buffer));
				// Source Address 16
				mAddr16Src.reset(new XBeeFrameAddr16Src(cursor, buffer));
				// Options
				mOptionsRecv.reset(new XBeeFrameOptionsRecv(cursor, buffer));
				// Calculate bytes till this point
				uint16_t bytesQty = std::distance(paylodStart, cursor);
				if (mLength->getValue() < bytesQty) {
					throw Utils::Error("Length value is less than must be");
				}
				// Data
				mData.reset(new XBeeFrameData(cursor, mLength->getValue()-bytesQty, buffer));
			}
				break;
			default:
				throw Utils::Error("API is not implemented");
		}
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrame));
	}
}

void XBeeFrame::encode(XBeeBuffer& buffer)
throw (Utils::Error)
{
	try {
		uint16_t checkSumStartPoint = 0;
		buffer.clear();
		// Delimiter
		checkNull(mDelimiter.get(), "Delimiter");
		mDelimiter->encode(buffer);
		// Skip Length temporary
		// Save the point
		uint16_t lengthStartPoint = buffer.size();
		// API Id
		checkNull(mApiId.get(), "ApiId");
		mApiId->encode(buffer);
		// API Data
		switch (mApiId->getValue()) {
			case XBeeFrameApiId::ZB_TX_REQ:
			{
				// Frame Id
				checkNull(mId.get(), "Id");
				mId->encode(buffer);
				// Destination Address 64
				checkNull(mAddr64Dst.get(), "Addr64Dst");
				mAddr64Dst->encode(buffer);
				// Destination Address 16
				checkNull(mAddr16Dst.get(), "Addr16Dst");
				mAddr16Dst->encode(buffer);
				// Radius
				checkNull(mRadius.get(), "Radius");
				mRadius->encode(buffer);
				// Options
				checkNull(mOptionsSend.get(), "OptionsSend");
				mOptionsSend->encode(buffer);
				// Data
				checkNull(mData.get(), "Data");
				mData->encode(buffer);
			}
				break;
			default:
				throw Utils::Error("API is not implemented");
		}
		// Length
		{
			XBeeBuffer tmp;
			XBeeFrameLength length(buffer.size()-lengthStartPoint);
			length.encode(tmp);
			// update checksum starting point => after length
			checkSumStartPoint=lengthStartPoint+tmp.size();
			// insert
			buffer.insert(buffer.begin()+lengthStartPoint, tmp.begin(), tmp.end());
		}
		// Checksum
		// 0xFF minus 8-bit SUM of bytes between the length and checksum fields
		{
			uint8_t checksum = 0;
			for(auto it = (buffer.begin()+checkSumStartPoint); it!=buffer.end(); it++) {
				checksum += (*it);
			}
			checksum = 0xFF - checksum;
			// insert
			buffer.push_back(checksum);
		}
	} catch (Utils::Error& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(XBeeFrame));
	}
}

void XBeeFrame::checkNull(void* v, const std::string& name)
throw (Utils::Error)
{
	if (!v) {
		throw Utils::Error(name + "is not available");
	}
}
