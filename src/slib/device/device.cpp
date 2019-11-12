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

#include "slib/device/device.h"

#include "slib/network/url.h"
#include "slib/social/contact.h"

namespace slib
{

#if !defined(SLIB_PLATFORM_IS_IOS)
	void Device::setAudioCategory(DeviceAudioCategory category)
	{
	}
#endif
	
#if !defined(SLIB_PLATFORM_IS_ANDROID)
	DeviceAudioMode Device::getAudioMode()
	{
		return DeviceAudioMode::Default;
	}

	void Device::setAudioMode(DeviceAudioMode mode)
	{
	}

	DeviceRingerMode Device::getRingerMode()
	{
		return DeviceRingerMode::Normal;
	}

	void Device::setRingerMode(DeviceRingerMode mode)
	{
	}

	float Device::getVolume(AudioStreamType stream)
	{
		return 0;
	}

	void Device::setVolume(float volume, AudioStreamType stream, const DeviceSetVolumeFlags& flags)
	{
	}

	sl_bool Device::isMicrophoneMute()
	{
		return sl_false;
	}

	void Device::setMicrophoneMute(sl_bool flag)
	{
	}

	sl_bool Device::isSpeakerOn()
	{
		return sl_false;
	}

	void Device::setSpeakerOn(sl_bool flag)
	{
	}

	sl_bool Device::isBluetoothScoOn()
	{
		return sl_false;
	}

	void Device::setBluetoothScoOn(sl_bool flag)
	{
	}
#endif

	void Device::setVolume(float volume, AudioStreamType stream)
	{
		setVolume(volume, stream, 0);
	}

#if !defined(SLIB_PLATFORM_IS_IOS) && !defined(SLIB_PLATFORM_IS_ANDROID)
	void Device::vibrate(sl_uint32 durationMillis)
	{
	}
#endif

	String Device::getIMEI()
	{
		return getIMEIs().getValueAt(0);
	}

	String Device::getPhoneNumber()
	{
		return getPhoneNumbers().getValueAt(0);
	}

#if !defined(SLIB_PLATFORM_IS_ANDROID)
	List<String> Device::getIMEIs()
	{
		return sl_null;
	}

	List<String> Device::getPhoneNumbers()
	{
		return sl_null;
	}

	sl_uint32 Device::getSimSlotsCount()
	{
#ifdef SLIB_PLATFORM_IS_MOBILE
		return 1;
#else
		return 0;
#endif
	}

	String Device::getPhoneNumber(sl_uint32 indexSIM)
	{
		return sl_null;
	}
#endif

#if !defined(SLIB_PLATFORM_IS_ANDROID) && !defined(SLIB_PLATFORM_IS_IOS) && !defined(SLIB_PLATFORM_IS_MACOS) && !defined(SLIB_PLATFORM_IS_TIZEN)
	String Device::getDeviceId()
	{
		return sl_null;
	}
#endif

	sl_uint32 Device::getScreenWidth()
	{
		return (sl_uint32)(getScreenSize().x);
	}

	sl_uint32 Device::getScreenHeight()
	{
		return (sl_uint32)(getScreenSize().y);
	}

#if !defined(SLIB_PLATFORM_IS_ANDROID)
	void Device::openDial(const String& phoneNumber)
	{
		openUrl("tel:" + Url::encodePercent(phoneNumber));
	}

	void Device::callPhone(const String& phoneNumber)
	{
		openDial(phoneNumber);
	}

	void Device::callPhone(const String& phoneNumber, sl_uint32 indexSIM)
	{
		openDial(phoneNumber);
	}

	void Device::answerCall(const String& callId)
	{
	}

	void Device::endCall(const String& callId)
	{
	}
#endif

#if !defined(SLIB_PLATFORM_IS_ANDROID) && !defined(SLIB_PLATFORM_IS_IOS)
	List<Contact> Device::getAllContacts()
	{
		return sl_null;
	}
#endif

}
