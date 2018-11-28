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

namespace slib
{

	SLIB_INLINE MacAddress EthernetFrame::getDestinationAddress()
	{
		return {_macDestination[0], _macDestination[1], _macDestination[2], _macDestination[3], _macDestination[4], _macDestination[5]};
	}
	
	SLIB_INLINE void EthernetFrame::setDestinationAddress(const MacAddress& address)
	{
		_macDestination[0] = address.m[0];
		_macDestination[1] = address.m[1];
		_macDestination[2] = address.m[2];
		_macDestination[3] = address.m[3];
		_macDestination[4] = address.m[4];
		_macDestination[5] = address.m[5];
	}
	
	SLIB_INLINE MacAddress EthernetFrame::getSourceAddress()
	{
		return {_macSource[0], _macSource[1], _macSource[2], _macSource[3], _macSource[4], _macSource[5]};
	}
	
	SLIB_INLINE void EthernetFrame::setSourceAddress(const MacAddress& address)
	{
		_macSource[0] = address.m[0];
		_macSource[1] = address.m[1];
		_macSource[2] = address.m[2];
		_macSource[3] = address.m[3];
		_macSource[4] = address.m[4];
		_macSource[5] = address.m[5];
	}
	
	SLIB_INLINE NetworkLinkProtocol EthernetFrame::getProtocol() const
	{
		return (NetworkLinkProtocol)(((sl_uint16)(_etherType[0]) << 8) | ((sl_uint16)(_etherType[1])));
	}
	
	SLIB_INLINE void EthernetFrame::setProtocol(NetworkLinkProtocol _type)
	{
		sl_uint32 type = (sl_uint32)_type;
		_etherType[0] = (sl_uint8)(type >> 8);
		_etherType[1] = (sl_uint8)(type);
	}
	
	SLIB_INLINE const sl_uint8* EthernetFrame::getContent() const
	{
		return ((const sl_uint8*)this) + HeaderSize;
	}
	
	SLIB_INLINE sl_uint8* EthernetFrame::getContent()
	{
		return ((sl_uint8*)this) + HeaderSize;
	}

}
