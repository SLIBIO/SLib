#include "media_platform_osx.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/core/scoped.h"
#include "../../../inc/slib/core/platform_apple.h"

SLIB_MEDIA_NAMESPACE_BEGIN

void OSX_AudioDeviceInfo::logError(String text)
{
	SLIB_LOG_ERROR("AudioDevice", text);
}
	
List<OSX_AudioDeviceInfo> OSX_AudioDeviceInfo::getAllDevices(sl_bool flagInput)
{
	List<OSX_AudioDeviceInfo> ret;
	
	AudioObjectPropertyAddress propDeviceListing;
	propDeviceListing.mSelector = kAudioHardwarePropertyDevices;
	propDeviceListing.mScope = kAudioObjectPropertyScopeGlobal;
	propDeviceListing.mElement = kAudioObjectPropertyElementMaster;
	UInt32 nSizeCountDevices = 0;
	if (AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &propDeviceListing, 0, NULL, &nSizeCountDevices) != kAudioHardwareNoError) {
		logError("Failed to get count of audio devices");
		return ret;
	}
	if (nSizeCountDevices % sizeof(AudioDeviceID) != 0) {
		logError("Invalid size of audio device count");
		return ret;
	}
	sl_uint32 nCountDevices = nSizeCountDevices / sizeof(AudioDeviceID);
	if (nCountDevices == 0) {
		logError("No devices");
		return ret;
	}
	
	SLIB_SCOPED_BUFFER(AudioDeviceID, 16, deviceIds, nCountDevices);
	if (!deviceIds) {
		return ret;
	}
	if (AudioObjectGetPropertyData(kAudioObjectSystemObject, &propDeviceListing, 0, NULL, &nSizeCountDevices, deviceIds) != kAudioHardwareNoError){
		logError("Failed to list audio device ids");
		return ret;
	}
	
	for (sl_uint32 i = 0; i < nCountDevices; i++) {
		OSX_AudioDeviceInfo info;
		if (info.getDeviceInfo(deviceIds[i], flagInput)) {
			ret.add_NoLock(info);
		}
	}
	return ret;
}

sl_bool OSX_AudioDeviceInfo::getDeviceInfo(AudioDeviceID deviceID, sl_bool flagInput)
{
	OSX_AudioDeviceInfo& info = *this;
	
	AudioObjectPropertyAddress propDeviceConfig;
	propDeviceConfig.mScope = flagInput ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput;
	propDeviceConfig.mElement = kAudioObjectPropertyElementMaster;
	UInt32 sizeValue;
	
	// Check for input channels
	propDeviceConfig.mSelector = kAudioDevicePropertyStreamConfiguration;
	sizeValue = 0;
	if (AudioObjectGetPropertyDataSize(deviceID, &propDeviceConfig, 0, NULL, &sizeValue) == kAudioHardwareNoError) {
		SLIB_SCOPED_BUFFER(char, 1024, memBufferList, sizeValue);
		if (!memBufferList) {
			return sl_false;
		}
		AudioBufferList* bufferList = (AudioBufferList*)(memBufferList);
		if (AudioObjectGetPropertyData(deviceID, &propDeviceConfig, 0, NULL, &sizeValue, bufferList) == kAudioHardwareNoError) {
			if (bufferList->mNumberBuffers == 0) {
				return sl_false;
			}
		} else {
			logError("Failed to get stream configuration - Dev" + String::fromUint32(deviceID));
			return sl_false;
		}
	} else {
		logError("Failed to get size of stream configuration - Dev" + String::fromUint32(deviceID));
		return sl_false;
	}
	
	info.id = deviceID;
	
	// Device UID
	CFStringRef deviceUID = NULL;
	propDeviceConfig.mSelector = kAudioDevicePropertyDeviceUID;
	sizeValue = sizeof(deviceUID);
	if (AudioObjectGetPropertyData(deviceID, &propDeviceConfig, 0, NULL, &sizeValue, &deviceUID) == kAudioHardwareNoError) {
		NSString* s = (__bridge NSString*)deviceUID;
		info.uid = Apple::getStringFromNSString(s);
		CFRelease(deviceUID);
	}
	
	// Device Name
	CFStringRef deviceName;
	propDeviceConfig.mSelector = kAudioDevicePropertyDeviceNameCFString;
	sizeValue = sizeof(deviceName);
	if (AudioObjectGetPropertyData(deviceID, &propDeviceConfig, 0, NULL, &sizeValue, &deviceName) == kAudioHardwareNoError) {
		NSString* s = (__bridge NSString*)deviceName;
		info.name = Apple::getStringFromNSString(s);
		CFRelease(deviceName);
	}
	
	// Device Manufacturer
	CFStringRef deviceManufacturer;
	propDeviceConfig.mSelector = kAudioDevicePropertyDeviceManufacturerCFString;
	sizeValue = sizeof(deviceManufacturer);
	if (AudioObjectGetPropertyData(deviceID, &propDeviceConfig, 0, NULL, &sizeValue, &deviceManufacturer) == kAudioHardwareNoError) {
		NSString* s = (__bridge NSString*)deviceManufacturer;
		info.manufacturer = Apple::getStringFromNSString(s);
		CFRelease(deviceManufacturer);
	}
	
	return sl_true;
}

sl_bool OSX_AudioDeviceInfo::getDefaultDevice(sl_bool flagInput)
{
	AudioObjectPropertyAddress propDeviceConfig;
	propDeviceConfig.mSelector = flagInput ? kAudioHardwarePropertyDefaultInputDevice : kAudioHardwarePropertyDefaultOutputDevice;
	propDeviceConfig.mScope = kAudioObjectPropertyScopeGlobal;
	propDeviceConfig.mElement = kAudioObjectPropertyElementMaster;
	
	AudioDeviceID dev;
	UInt32 size = sizeof(dev);
	if (AudioObjectGetPropertyData(kAudioObjectSystemObject, &propDeviceConfig, 0, NULL, &size, &dev) != kAudioHardwareNoError) {
		logError("Failed to get default device");
		return sl_false;
	}
	if (dev == kAudioDeviceUnknown) {
		logError("Unknown default device");
		return sl_false;
	}
	return getDeviceInfo(dev, flagInput);
}
	
sl_bool OSX_AudioDeviceInfo::selectDevice(sl_bool flagInput, String uid)
{
	if (uid.isEmpty()) {
		return getDefaultDevice(flagInput);
	} else {
		ListItems<OSX_AudioDeviceInfo> list(getAllDevices(flagInput));
		for (sl_size i = 0; i < list.count; i++) {
			OSX_AudioDeviceInfo& element = list[i];
			if (element.uid == uid) {
				*this = element;
				return sl_true;
			}
		}
		return sl_false;
	}
}

SLIB_MEDIA_NAMESPACE_END

#endif
