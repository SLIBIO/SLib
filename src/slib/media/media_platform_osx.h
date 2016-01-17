#include "../../../inc/slib/media/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/core/list.h"
#include "../../../inc/slib/core/string.h"
#include "../../../inc/slib/core/log.h"

#import <Foundation/Foundation.h>
#import <CoreAudio/CoreAudio.h>
#import <AudioToolbox/AudioConverter.h>

SLIB_MEDIA_NAMESPACE_BEGIN

class OSX_AudioDeviceInfo
{
public:
    AudioDeviceID id;
	String uid;
    String name;
	String manufacturer;
    
public:
	sl_bool getDeviceInfo(AudioDeviceID deviceID, sl_bool flagInput);
	
	sl_bool getDefaultDevice(sl_bool flagInput);
	
	sl_bool selectDevice(sl_bool flagInput, String uid);

public:
	static List<OSX_AudioDeviceInfo> getAllDevices(sl_bool flagInput);
	
private:
	static void logError(String text);
	
};

SLIB_MEDIA_NAMESPACE_END

#endif
