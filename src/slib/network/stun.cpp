/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/network/stun.h"

#include "slib/core/mio.h"
#include "slib/core/log.h"

#include "slib/crypto/hmac.h"
#include "slib/crypto/sha1.h"
#include "slib/crypto/md5.h"
#include "slib/crypto/zlib.h"

namespace slib
{
	
#define ALIGN4(n) ((((n) - 1) | 3) + 1)

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(StunAttributes)
	
	StunAttributes::StunAttributes()
	{
		errorCode = StunErrorCode::Success;
		flagHaveMessageIntegrity = sl_false;
		offsetMessageIntegrityAttribute = 0;
		flagHaveFingerprint = sl_false;
	}
	
	void StunAttributes::initialize()
	{
		mappedAddress.setNone();
		xorMappedAddress.setNone();
		userName.setNull();
		password.setNull();
		realm.setNull();
		nonce.setNull();
		errorCode = StunErrorCode::Success;
		errorReasonPhase.setNull();
		unknownAttributes.setNull();
		unknownAttributesDuringParsing.setNull();
		software.setNull();
		alternateServers.setNull();
		flagHaveMessageIntegrity = sl_false;
		offsetMessageIntegrityAttribute = 0;
		flagHaveFingerprint = sl_false;
	}
	
	StunMessageClass StunPacket::getMessageClass() const
	{
		return (StunMessageClass)(((_messageType[0] & 1) << 1) | ((_messageType[1] >> 4) & 1));
	}
	
	void StunPacket::setMessageClass(StunMessageClass _class)
	{
		sl_uint8 n = (sl_uint8)(_class);
		_messageType[0] = (_messageType[0] & 0xFE) | ((n >> 1) & 1);
		_messageType[1] = (_messageType[1] & 0xEF) | ((n & 1) << 4);
	}
	
	StunMethod StunPacket::getMethod() const
	{
		return (StunMethod)(((_messageType[0] >> 1) << 7) | ((_messageType[1] >> 5) << 4) | (_messageType[1] & 0xF));
	}
	
	void StunPacket::setMethod(StunMethod method)
	{
		sl_uint8 n = (sl_uint8)(method);
		_messageType[0] = (_messageType[0] & 1) | ((n >> 7) << 1);
		_messageType[1] = (_messageType[1] & 0x10) | (((n >> 4) & 7) << 5) | (n & 0xF);
	}
	
	sl_uint16 StunPacket::getMessageLength() const
	{
		return MIO::readUint16BE(_messageLength);
	}
	
	void StunPacket::setMessageLength(sl_uint16 length)
	{
		MIO::writeUint16BE(_messageLength, length);
	}
	
	sl_uint32 StunPacket::getMagicCookie() const
	{
		return MIO::readUint32BE(_magicCookie);
	}
	
	void StunPacket::setMagicCookie(sl_uint32 cookie)
	{
		MIO::writeUint32BE(_magicCookie, cookie);
	}
	
	const sl_uint8* StunPacket::getTransactionID() const
	{
		return _transactionID;
	}

	sl_uint8* StunPacket::getTransactionID()
	{
		return _transactionID;
	}
	
	const void* StunPacket::getAttributes() const
	{
		return (const sl_uint8*)this + HeaderSize;
	}

	void* StunPacket::getAttributes()
	{
		return (sl_uint8*)this + HeaderSize;
	}

	sl_bool StunPacket::checkHeader(const void* _packet, sl_size size)
	{
		if (size < HeaderSize) {
			return sl_false;
		}
		StunPacket* packet = (StunPacket*)_packet;
		if ((packet->_messageType[0] & 0xC0) != 0) {
			return sl_false;
		}
		if ((packet->_messageLength[1] & 3) != 0) {
			return sl_false;
		}
		if (packet->getMagicCookie() != SLIB_NETWORK_STUN_MAGIC_COOKIE) {
			return sl_false;
		}
		return sl_true;
	}
	
	sl_bool StunPacket::parseAttributes(sl_size packetSize, StunAttributes& attributes) const
	{
		if (!(checkHeader(this, packetSize))) {
			return sl_false;
		}
		attributes.initialize();
		sl_uint16 len = getMessageLength();
		if (len == 0) {
			return sl_true;
		}
		if (HeaderSize + (sl_size)len > packetSize) {
			return sl_false;
		}
		const sl_uint8* pAttrs = (const sl_uint8*)this + HeaderSize;
		sl_size startAttr = 0;
		while (startAttr < len) {
			if (startAttr + 4 > len) {
				return sl_false;
			}
			StunAttributeType type = (StunAttributeType)(MIO::readUint16BE(pAttrs + startAttr));
			sl_uint16 lenValue = MIO::readUint16BE(pAttrs + startAttr + 2);
			sl_size startValue = startAttr + 4;
			if (startValue + lenValue > len) {
				return sl_false;
			}
			switch (type) {
				case StunAttributeType::MappedAddress:
					if (!(readMappedAddressAttributeValue(pAttrs + startValue, lenValue, attributes.mappedAddress))) {
						return sl_false;
					}
					break;
				case StunAttributeType::XorMappedAddress:
					if (!(readXorMappedAddressAttributeValue(pAttrs + startValue, lenValue, attributes.xorMappedAddress))) {
						return sl_false;
					}
				case StunAttributeType::UserName:
					attributes.userName = String((sl_char8*)(pAttrs + startValue), lenValue);
					break;
				case StunAttributeType::Realm:
					attributes.realm = String((sl_char8*)(pAttrs + startValue), lenValue);
					break;
				case StunAttributeType::Nonce:
					attributes.nonce = String((sl_char8*)(pAttrs + startValue), lenValue);
					break;
				case StunAttributeType::ErrorCode:
					if (!(readErrorCodeAttributeValue(pAttrs + startValue, lenValue, attributes.errorCode, attributes.errorReasonPhase))) {
						return sl_false;
					}
					break;
				case StunAttributeType::UnknownAttributes:
					if (!(readUnknownAttributesAttributeValue(pAttrs + startValue, lenValue, attributes.unknownAttributes))) {
						return sl_false;
					}
					break;
				case StunAttributeType::Software:
					attributes.software = String((sl_char8*)(pAttrs + startValue), lenValue);
					break;
				case StunAttributeType::AlternateServer:
					{
						SocketAddress alternateServer;
						if (!(readMappedAddressAttributeValue(pAttrs + startValue, lenValue, alternateServer))) {
							return sl_false;
						}
						attributes.alternateServers.add(alternateServer);
					}
					break;
				case StunAttributeType::MessageIntegrity:
					{
						if (lenValue != 20) {
							return sl_false;
						}
						attributes.flagHaveMessageIntegrity = sl_true;
						attributes.offsetMessageIntegrityAttribute = HeaderSize + startAttr;
					}
					break;
				case StunAttributeType::Fingerprint:
					{
						if (lenValue != 4) {
							return sl_false;
						}
						sl_uint8 crc[4];
						calculateFingerprint(crc, this, HeaderSize + startAttr);
						if (Base::compareMemory(crc, pAttrs + startValue, 4)) {
							return sl_false;
						}
						if (startValue + 4 < len) {
							return sl_false;
						}
						attributes.flagHaveFingerprint = sl_true;
					}
					break;
				default:
					attributes.unknownAttributesDuringParsing.add((sl_uint16)type);
					break;
			}
			startAttr = startValue + ALIGN4(lenValue);
		}
		return sl_true;
	}
	
	sl_bool StunPacket::checkMessageIntegrity(const StunAttributes& attributes) const
	{
		if (!(attributes.flagHaveMessageIntegrity)) {
			return sl_true;
		}
		const sl_uint8* packet = (const sl_uint8*)this;
		sl_uint8 messageIntegrity[20];
		calculateMessageIntegrity(messageIntegrity, packet, attributes.offsetMessageIntegrityAttribute, attributes.userName, attributes.realm, attributes.password);
		return Base::compareMemory(messageIntegrity, packet + attributes.offsetMessageIntegrityAttribute + 4, 20) == 0;
	}
	
	Memory StunPacket::buildPacket(StunMessageClass _class, StunMethod method, const void* transactionID, const StunAttributes& attributes)
	{
		sl_size sizeMessage = 0;
		sl_size i;

		ListElements<SocketAddress> alternateServers(attributes.alternateServers);

		sizeMessage += ALIGN4(writeMappedAddressAttribute(StunAttributeType::MappedAddress, attributes.mappedAddress, sl_null));
		sizeMessage += ALIGN4(writeMappedAddressAttribute(StunAttributeType::XorMappedAddress, attributes.xorMappedAddress, sl_null));
		sizeMessage += ALIGN4(writeStringAttribute(StunAttributeType::UserName, attributes.userName, sl_null));
		sizeMessage += ALIGN4(writeStringAttribute(StunAttributeType::Realm, attributes.realm, sl_null));
		sizeMessage += ALIGN4(writeStringAttribute(StunAttributeType::Nonce, attributes.nonce, sl_null));
		sizeMessage += ALIGN4(writeErrorCodeAttribute(attributes.errorCode, attributes.errorReasonPhase, sl_null));
		sizeMessage += ALIGN4(writeUnknownAttributesAttribute(attributes.unknownAttributes, sl_null));
		sizeMessage += ALIGN4(writeStringAttribute(StunAttributeType::Software, attributes.software, sl_null));
		for (i = 0; i < alternateServers.count; i++) {
			sizeMessage += ALIGN4(writeMappedAddressAttribute(StunAttributeType::AlternateServer, alternateServers[i], sl_null));
		}
		if (attributes.flagHaveMessageIntegrity) {
			sizeMessage += (4 + 20);
		}
		if (attributes.flagHaveFingerprint) {
			sizeMessage += (4 + 4);
		}

		if (sizeMessage > 0xFFFF - HeaderSize) {
			return sl_null;
		}
		
		Memory memPacket = Memory::create(HeaderSize + sizeMessage);
		if (memPacket.isNull()) {
			return sl_null;
		}
		
		StunPacket* packet = (StunPacket*)(memPacket.getData());
		packet->setMessageClass(_class);
		packet->setMethod(method);
		packet->setMessageLength((sl_uint16)sizeMessage);
		packet->setMagicCookie();
		Base::copyMemory(packet->getTransactionID(), transactionID, 12);
		
		sl_uint8* pAttrs = (sl_uint8*)(memPacket.getData()) + HeaderSize;
		sl_size pos = 0;
		
		pos += ALIGN4(writeMappedAddressAttribute(StunAttributeType::MappedAddress, attributes.mappedAddress, pAttrs + pos));
		pos += ALIGN4(packet->writeXorMappedAddressAttribute(attributes.xorMappedAddress, pAttrs + pos));
		pos += ALIGN4(writeStringAttribute(StunAttributeType::UserName, attributes.userName, pAttrs + pos));
		pos += ALIGN4(writeStringAttribute(StunAttributeType::Realm, attributes.realm, pAttrs + pos));
		pos += ALIGN4(writeStringAttribute(StunAttributeType::Nonce, attributes.nonce, pAttrs + pos));
		pos += ALIGN4(writeErrorCodeAttribute(attributes.errorCode, attributes.errorReasonPhase, pAttrs + pos));
		pos += ALIGN4(writeUnknownAttributesAttribute(attributes.unknownAttributes, pAttrs + pos));
		pos += ALIGN4(writeStringAttribute(StunAttributeType::Software, attributes.software, pAttrs + pos));
		for (i = 0; i < alternateServers.count; i++) {
			pos += ALIGN4(writeMappedAddressAttribute(StunAttributeType::AlternateServer, alternateServers[i], pAttrs + pos));
		}
		if (attributes.flagHaveMessageIntegrity) {
			writeAttributeHeader(StunAttributeType::MessageIntegrity, 20, pAttrs + pos);
			calculateMessageIntegrity(pAttrs + pos + 4, packet, HeaderSize + pos, attributes.userName, attributes.realm, attributes.password);
			pos += (4 + 20);
		}
		if (attributes.flagHaveFingerprint) {
			writeAttributeHeader(StunAttributeType::Fingerprint, 4, pAttrs + pos);
			calculateFingerprint(pAttrs + pos + 4, packet, HeaderSize + pos);
		}
		return memPacket;
	}
	
	void StunPacket::writeAttributeHeader(StunAttributeType type, sl_size len, void* _data)
	{
		sl_uint8* data = (sl_uint8*)_data;
		MIO::writeUint16BE(data, (sl_uint16)type);
		MIO::writeUint16BE(data + 2, (sl_uint16)len);
	}
	
	sl_size StunPacket::writeStringAttribute(StunAttributeType type, const String& str, void* _data)
	{
		sl_uint8* data = (sl_uint8*)_data;
		if (str.isNull()) {
			return 0;
		}
		if (data) {
			sl_size n = str.getLength();
			writeAttributeHeader(type, n, data);
			Base::copyMemory(data + 4, str.getData(), n);
			return 4 + n;
		} else {
			return 4 + str.getLength();
		}
	}
	
	sl_bool StunPacket::readMappedAddressAttributeValue(const void* _data, sl_size size, SocketAddress& mappedAddress)
	{
		const sl_uint8* data = (const sl_uint8*)_data;
		if (size < 4) {
			return sl_false;
		}
		sl_uint8 family = data[1];
		if (family == 1) {
			// IPv4
			if (size != 8) {
				return sl_false;
			}
			mappedAddress.port = MIO::readUint16BE(data + 2);
			mappedAddress.ip.setIPv4(IPv4Address(data[4], data[5], data[6], data[7]));
		} else if (family == 2) {
			// IPv6
			if (size != 20) {
				return sl_false;
			}
			mappedAddress.port = MIO::readUint16BE(data + 2);
			mappedAddress.ip.setIPv6(IPv6Address(data + 4));
		} else {
			return sl_false;
		}
		return sl_true;
	}
	
	sl_size StunPacket::writeMappedAddressAttribute(StunAttributeType type, const SocketAddress& mappedAddress, void* _data)
	{
		sl_uint8* header = (sl_uint8*)_data;
		if (mappedAddress.ip.isNone()) {
			return 0;
		}
		if (!header) {
			if (mappedAddress.ip.isIPv4()) {
				return 4 + 4 + 4;
			} else if (mappedAddress.ip.isIPv6()) {
				return 4 + 4 + 16;
			} else {
				return 0;
			}
		}
		sl_uint8* data = header + 4;
		if (mappedAddress.ip.isIPv4()) {
			writeAttributeHeader(type, 4 + 4, header);
			data[0] = 0;
			data[1] = 1; // family
			MIO::writeUint16BE(data + 2, (sl_uint16)(mappedAddress.port));
			mappedAddress.ip.getIPv4().getBytes(data + 4);
			return 4 + 4 + 4;
		} else if (mappedAddress.ip.isIPv6()) {
			writeAttributeHeader(type, 4 + 16, header);
			data[0] = 0;
			data[1] = 2; // family
			MIO::writeUint16BE(data + 2, (sl_uint16)(mappedAddress.port));
			mappedAddress.ip.getIPv6().getBytes(data + 4);
			return 4 + 4 + 16;
		} else {
			return 0;
		}
	}
	
	sl_bool StunPacket::readXorMappedAddressAttributeValue(const void* _data, sl_size size, SocketAddress& mappedAddress) const
	{
		const sl_uint8* data = (const sl_uint8*)_data;
		if (size < 4) {
			return sl_false;
		}
		sl_uint8 family = data[1];
		if (family == 1) {
			// IPv4
			if (size != 8) {
				return sl_false;
			}
			mappedAddress.port = SLIB_MAKE_WORD(data[2]^_magicCookie[0], data[3]^_magicCookie[1]);
			mappedAddress.ip.setIPv4(IPv4Address(data[4]^_magicCookie[0], data[5]^_magicCookie[1], data[6]^_magicCookie[2], data[7]^_magicCookie[3]));
		} else if (family == 2) {
			// IPv6
			if (size != 20) {
				return sl_false;
			}
			mappedAddress.port = SLIB_MAKE_WORD(data[2]^_magicCookie[0], data[3]^_magicCookie[1]);
			IPv6Address addr(data + 4);
			sl_size i;
			for (i = 0; i < 4; i++) {
				addr.m[i] ^= _magicCookie[i];
			}
			for (i = 0; i < 12; i++) {
				addr.m[i + 4] ^= _transactionID[i];
			}
			mappedAddress.ip.setIPv6(addr);
		} else {
			return sl_false;
		}
		return sl_true;
	}
	
	sl_size StunPacket::writeXorMappedAddressAttribute(const SocketAddress& mappedAddress, void* _data) const
	{
		sl_uint8* header = (sl_uint8*)_data;
		if (mappedAddress.ip.isNone()) {
			return 0;
		}
		if (!header) {
			if (mappedAddress.ip.isIPv4()) {
				return 4 + 4 + 4;
			} else if (mappedAddress.ip.isIPv6()) {
				return 4 + 4 + 16;
			} else {
				return 0;
			}
		}
		sl_uint8* data = header + 4;
		if (mappedAddress.ip.isIPv4()) {
			writeAttributeHeader(StunAttributeType::XorMappedAddress, 4 + 4, header);
			data[0] = 0;
			data[1] = 1; // family
			MIO::writeUint16BE(data + 2, (sl_uint16)(mappedAddress.port));
			mappedAddress.ip.getIPv4().getBytes(data + 4);
			data[2] ^= _magicCookie[0];
			data[3] ^= _magicCookie[1];
			data[4] ^= _magicCookie[0];
			data[5] ^= _magicCookie[1];
			data[6] ^= _magicCookie[2];
			data[7] ^= _magicCookie[3];
			return 4 + 4 + 4;
		} else if (mappedAddress.ip.isIPv6()) {
			writeAttributeHeader(StunAttributeType::XorMappedAddress, 4 + 16, header);
			data[0] = 0;
			data[1] = 2; // family
			MIO::writeUint16BE(data + 2, (sl_uint16)(mappedAddress.port));
			mappedAddress.ip.getIPv6().getBytes(data + 4);
			data[2] ^= _magicCookie[0];
			data[3] ^= _magicCookie[1];
			data[4] ^= _magicCookie[0];
			data[5] ^= _magicCookie[1];
			data[6] ^= _magicCookie[2];
			data[7] ^= _magicCookie[3];
			for (sl_size i = 0; i < 12; i++) {
				data[i + 8] ^= _transactionID[i];
			}
			return 4 + 4 + 16;
		} else {
			return 0;
		}
	}
	
	sl_bool StunPacket::readErrorCodeAttributeValue(const void* _data, sl_size size, StunErrorCode& errorCode, String& errorReasonPhase)
	{
		const sl_uint8* data = (const sl_uint8*)_data;
		if (size < 4) {
			return sl_false;
		}
		sl_uint32 num = data[3];
		if (num >= 100) {
			return sl_false;
		}
		sl_uint32 cls = data[2] & 7;
		if (cls < 3 || cls > 6) {
			return sl_false;
		}
		errorCode = (StunErrorCode)(cls * 100 + num);
		errorReasonPhase = String((sl_char8*)(data + 4), size - 4);
		return sl_true;
	}
	
	sl_size StunPacket::writeErrorCodeAttribute(StunErrorCode _errorCode, const String& errorReasonPhase, void* _data)
	{
		sl_uint8* header = (sl_uint8*)_data;
		sl_uint32 errorCode = (sl_uint32)_errorCode;
		if (errorCode < 300 || errorCode >= 700) {
			return 0;
		}
		if (header) {
			sl_uint8* data = header + 4;
			sl_size n = errorReasonPhase.getLength();
			writeAttributeHeader(StunAttributeType::ErrorCode, 4 + n, header);
			data[0] = 0;
			data[1] = 0;
			data[2] = errorCode / 100;
			data[3] = errorCode % 100;
			Base::copyMemory(data + 4, errorReasonPhase.getData(), n);
			return 4 + 4 + n;
		} else {
			return 4 + 4 + errorReasonPhase.getLength();
		}
	}

	sl_bool StunPacket::readUnknownAttributesAttributeValue(const void* _data, sl_size size, List<sl_uint16>& unknownAttributes)
	{
		const sl_uint8* data = (const sl_uint8*)_data;
		if (size & 1) {
			return sl_false;
		}
		sl_size n = size >> 1;
		for (sl_size i = 0; i < n; i++) {
			unknownAttributes.add(MIO::readUint16BE(data));
			data += 2;
		}
		return sl_true;
	}
	
	sl_size StunPacket::writeUnknownAttributesAttribute(const List<sl_uint16>& unknownAttributes, void* _data)
	{
		sl_uint8* header = (sl_uint8*)_data;
		if (header) {
			sl_uint8* data = header + 4;
			ListElements<sl_uint16> list(unknownAttributes);
			if (list.count > 0) {
				writeAttributeHeader(StunAttributeType::UnknownAttributes, 4 + (list.count << 1), header);
				for (sl_size i = 0; i < list.count; i++) {
					MIO::writeUint16BE(data, list[i]);
					data += 2;
				}
				return 4 + (list.count << 1);
			} else {
				return 0;
			}
		} else {
			sl_size n = unknownAttributes.getCount();
			if (n > 0) {
				return 4 + (n << 1);
			} else {
				return 0;
			}
		}
	}
	
	void StunPacket::calculateMessageIntegrity(void* output, const void* packet, sl_size size, const String& userName, const String& realm, const String& password)
	{
		// HMAC-SHA1, long-term-credentials-key = MD5(username ":" realm ":" password), short-term-credentials-key=password
		sl_uint8 bufKey[16];
		const sl_uint8* key;
		sl_size lenKey;
		if (realm.isNotNull()) {
			String strKey = userName + ":" + realm + ":" + password;
			MD5::hash(strKey.getData(), strKey.getLength(), bufKey);
			key = bufKey;
			lenKey = 16;
		} else {
			key = (const sl_uint8*)(password.getData());
			lenKey = password.getLength();
		}
		HMAC<SHA1>::execute(key, lenKey, packet, size, output);
	}
	
	void StunPacket::calculateFingerprint(void* output /* 4 bytes */, const void* packet, sl_size size)
	{
		sl_uint32 crc = Zlib::crc32(packet, size);
		crc ^= 0x5354554e;
		MIO::writeUint32BE(output, crc);
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(StunServerParam)
	
	StunServerParam::StunServerParam()
	{
		port = SLIB_NETWORK_STUN_PORT;
		flagAutoStart = sl_true;
		flagLogging = sl_false;
	}
	
	
	SLIB_DEFINE_OBJECT(StunServer, Object)
	
	StunServer::StunServer()
	{
		m_flagInit = sl_false;
		m_flagRunning = sl_false;
		m_flagLogging = sl_false;
	}
	
	StunServer::~StunServer()
	{
		release();
	}
	
#define TAG_SERVER "StunServer"
	
	Ref<StunServer> StunServer::create(const StunServerParam& param)
	{
		Ref<StunServer> ret = new StunServer;
		
		if (ret.isNotNull()) {
			
			AsyncUdpSocketParam up;
			up.onReceiveFrom = SLIB_FUNCTION_WEAKREF(StunServer, _onReceiveFrom, ret);
			up.packetSize = 4096;
			up.ioLoop = param.ioLoop;
			up.flagAutoStart = sl_false;
			
			up.bindAddress.port = param.port;
			Ref<AsyncUdpSocket> socket = AsyncUdpSocket::create(up);
			if (socket.isNull()) {
				LogError(TAG_SERVER, "Failed to bind to port %d", param.port);
				return sl_null;
			}
			
			if (socket.isNotNull()) {
				
				ret->m_flagLogging = param.flagLogging;
				
				ret->m_udp = socket;
				
				ret->m_flagInit = sl_true;
				if (param.flagAutoStart) {
					ret->start();
				}
				return ret;
				
			}
			
		}
		return sl_null;
	}
	
	void StunServer::release()
	{
		ObjectLocker lock(this);
		if (!m_flagInit) {
			return;
		}
		m_flagInit = sl_false;
		
		m_flagRunning = sl_false;
		if (m_udp.isNotNull()) {
			m_udp->close();
		}
	}
	
	void StunServer::start()
	{
		ObjectLocker lock(this);
		if (!m_flagInit) {
			return;
		}
		if (m_flagRunning) {
			return;
		}
		if (m_udp.isNotNull()) {
			m_udp->start();
		}
		m_flagRunning = sl_true;
	}
	
	sl_bool StunServer::isRunning()
	{
		return m_flagRunning;
	}
	
	void StunServer::_onReceiveFrom(AsyncUdpSocket* socket, const SocketAddress& addressFrom, void* data, sl_uint32 size)
	{
		StunPacket* packet = (StunPacket*)data;
		StunAttributes attrs;
		if (packet->parseAttributes(size, attrs)) {
			if (packet->getMessageClass() == StunMessageClass::Request && packet->getMethod() == StunMethod::Binding) {
				if (m_flagLogging) {
					Log(TAG_SERVER, "Binding Request From: %s", addressFrom.toString());
				}
				attrs.initialize();
				attrs.xorMappedAddress = addressFrom;
				Memory mem = StunPacket::buildPacket(StunMessageClass::Response, StunMethod::Binding, packet->getTransactionID(), attrs);
				if (mem.isNotNull()) {
					socket->sendTo(addressFrom, mem);
				}
			}
		}
	}
	
}
