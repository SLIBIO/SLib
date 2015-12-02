#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/audio/object.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/scoped_pointer.h"
#include "../../../inc/slib/core/platform_apple.h"

#import <Foundation/Foundation.h>
#import <CoreAudio/CoreAudio.h>
#import <AudioToolbox/AudioConverter.h>

SLIB_AUDIO_NAMESPACE_BEGIN

class _OSX_AudioDeviceInfo
{
public:
    AudioDeviceID id;
	String uid;
    String name;
	String manufacturer;
    
public:
	static void logError(String text)
	{
		SLIB_LOG_ERROR("AudioDeviceInfo", text);
	}
	
    static List<_OSX_AudioDeviceInfo> _getAllDevices(sl_bool flagInput)
    {
		List<_OSX_AudioDeviceInfo> ret;
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
			_OSX_AudioDeviceInfo info;
			if (_getDeviceInfo(deviceIds[i], flagInput, info)) {
				ret.add(info);
			}
		}
		
		return ret;
    }
	
	static sl_bool _getDeviceInfo(AudioDeviceID deviceID, sl_bool flagInput, _OSX_AudioDeviceInfo& info)
	{
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
	
	static List<AudioDeviceInfo> _getDeviceInfos(sl_bool flagInput)
	{
		ListLocker<_OSX_AudioDeviceInfo> list(_getAllDevices(flagInput));
		List<AudioDeviceInfo> ret;
		for (sl_size i = 0; i < list.count(); i++) {
			AudioDeviceInfo info;
			info.id = list[i].uid;
			info.name = list[i].name;
			info.description = list[i].manufacturer;
			ret.add(info);
		}
		return ret;
	}
	
	static sl_bool _getDefaultDevice(sl_bool flagInput, _OSX_AudioDeviceInfo& info)
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
		return _getDeviceInfo(dev, flagInput, info);
	}
	
	static sl_bool _selectDevice(sl_bool flagInput, String uid, _OSX_AudioDeviceInfo& info)
	{
		if (uid.isEmpty()) {
			return _getDefaultDevice(flagInput, info);
		} else {
			ListLocker<_OSX_AudioDeviceInfo> list(_getAllDevices(flagInput));
			for (sl_size i = 0; i < list.count(); i++) {
				_OSX_AudioDeviceInfo& element = list[i];
				if (element.uid == uid) {
					info = element;
					return sl_true;
				}
			}
			return sl_false;
		}
	}

};


class _OSX_AudioRecorder : public AudioRecorder
{
private:
	_OSX_AudioRecorder()
	{
		m_flagOpened = sl_true;
		m_flagRunning = sl_false;
		
		m_callback = sl_null;
	}

public:
	sl_bool m_flagOpened;
	sl_bool m_flagRunning;
	
	AudioDeviceID m_deviceID;
	AudioConverterRef m_converter;
	AudioDeviceIOProcID m_callback;
	
	AudioStreamBasicDescription m_formatSrc;
	AudioStreamBasicDescription m_formatDst;
	sl_uint32 m_nSamplesPerFrame;
	
	~_OSX_AudioRecorder()
	{
		release();
	}
	
	static void logError(String text)
	{
		SLIB_LOG_ERROR("AudioRecorder", text);
	}
	
	static Ref<_OSX_AudioRecorder> create(const AudioRecorderParam& param)
	{
		Ref<_OSX_AudioRecorder> ret;
		
		_OSX_AudioDeviceInfo deviceInfo;
		if (!_OSX_AudioDeviceInfo::_selectDevice(sl_true, param.deviceId, deviceInfo)) {
			logError("Failed to find audio input device - " + param.deviceId);
			return ret;
		}
		
		AudioDeviceID deviceID = deviceInfo.id;
		
		// Get current stream description
		AudioObjectPropertyAddress prop;
		prop.mSelector = kAudioDevicePropertyStreamFormat;
		prop.mScope = kAudioDevicePropertyScopeInput;
		prop.mElement = kAudioObjectPropertyElementMaster;
		
		UInt32 sizeValue;
		AudioStreamBasicDescription formatSrc;
		Base::resetMemory(&formatSrc, 0, sizeof(formatSrc));
		sizeValue = sizeof(formatSrc);
		if (AudioObjectGetPropertyData(deviceID, &prop, 0, NULL, &sizeValue, &formatSrc) != kAudioHardwareNoError) {
			logError("Failed to get source input format");
			return ret;
		}
		
		prop.mSelector = kAudioDevicePropertyBufferSizeRange;
		AudioValueRange rangeBufferSize;
		sizeValue = sizeof(rangeBufferSize);
		if (AudioObjectGetPropertyData(deviceID, &prop, 0, NULL, &sizeValue, &rangeBufferSize) != kAudioHardwareNoError) {
			logError("Failed to get buffer size range");
			return ret;
		}
		
		UInt32 sizeFrame = param.nFrameLengthInMilliseconds * formatSrc.mSampleRate * formatSrc.mBytesPerFrame / 1000;
		if (sizeFrame < rangeBufferSize.mMinimum) {
			logError("Required frame size(" + String::fromUint32(sizeFrame) + ") is smaller than minimum " + String::fromUint32(rangeBufferSize.mMinimum));
			return ret;
		}
		if (sizeFrame > rangeBufferSize.mMaximum) {
			logError("Required frame size(" + String::fromUint32(sizeFrame) + ") is bigger than maximum " + String::fromUint32(rangeBufferSize.mMinimum));
			return ret;
		}
		
		// Create Audio Converter
		AudioStreamBasicDescription formatDst;
		formatDst.mFormatID = kAudioFormatLinearPCM;
		formatDst.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
		formatDst.mSampleRate = param.nSamplesPerSecond;
		formatDst.mBitsPerChannel = 16;
		formatDst.mChannelsPerFrame = 1;
		formatDst.mBytesPerFrame = formatDst.mChannelsPerFrame * formatDst.mBitsPerChannel / 8;
		formatDst.mFramesPerPacket = 1;
		formatDst.mBytesPerPacket = formatDst.mBytesPerFrame * formatDst.mFramesPerPacket;
		
		AudioConverterRef converter;
		if (AudioConverterNew(&formatSrc, &formatDst, &converter) == kAudioHardwareNoError) {
			
			prop.mSelector = kAudioDevicePropertyBufferSize;
			sizeValue = sizeof(sizeFrame);
			
			if (AudioObjectSetPropertyData(deviceID, &prop, 0, NULL, sizeValue, &sizeFrame) == kAudioHardwareNoError) {
					
				ret = new _OSX_AudioRecorder();
				
				if (ret.isNotNull()) {
					
					ret->m_deviceID = deviceID;
					ret->m_converter = converter;
					ret->m_nSamplesPerFrame = param.nSamplesPerSecond * param.nFrameLengthInMilliseconds / 1000;
					ret->m_queue.setQueueSize(param.nSamplesPerSecond * param.nBufferLengthInMilliseconds / 1000);
					ret->m_formatSrc = formatSrc;
					ret->m_formatDst = formatDst;
					ret->setListener(param.listener);

					AudioDeviceIOProcID callback;
					if (AudioDeviceCreateIOProcID(deviceID, DeviceIOProc, ret.getObject(), &callback) == kAudioHardwareNoError) {
						ret->m_callback = callback;
						if (param.flagAutoStart) {
							ret->start();
						}
						return ret;
					} else {
						logError("Failed to create io proc");
					}
					
					return Ref<_OSX_AudioRecorder>::null();
				}
			} else {
				logError("Failed to get set buffer size");
			}
			AudioConverterDispose(converter);
		} else {
			logError("Failed to create audio converter");
		}
		return ret;
	}
	
	void release()
	{
		MutexLocker lock(getLocker());
		
		if (!m_flagOpened) {
			return;
		}
		
		stop();
		
		m_flagOpened = sl_false;
		
		if (m_callback) {
			AudioDeviceDestroyIOProcID(m_deviceID, m_callback);
		}
		
		AudioConverterDispose(m_converter);

	}
	
	sl_bool isOpened()
	{
		return m_flagOpened;
	}
	
	void start()
	{
		MutexLocker lock(getLocker());
		
		if (!m_flagOpened) {
			return;
		}
		
		if (m_flagRunning) {
			return;
		}
		
		if (AudioDeviceStart(m_deviceID, m_callback) != kAudioHardwareNoError) {
			logError("Failed to start device");
		}
		
		m_flagRunning = sl_true;
	}

	void stop()
	{
		MutexLocker lock(getLocker());
		
		if (!m_flagOpened) {
			return;
		}
		
		if (!m_flagRunning) {
			return;
		}
		
		m_flagRunning = sl_false;
		
		if (AudioDeviceStop(m_deviceID, m_callback) != kAudioHardwareNoError) {
			logError("Failed to stop play device");
		}
	}
	
	sl_bool isRunning()
	{
		return m_flagRunning;
	}
	
	void onFrame(sl_int16* buf, sl_uint32 n)
	{
		_processFrame_S16(buf, n);
	}
	
	struct ConverterContext
	{
		sl_uint32 nBytesPerPacket;
		const AudioBufferList* data;
		sl_bool flagUsed;
	};
	
	static OSStatus ConverterProc(AudioConverterRef               inAudioConverter,
								  UInt32*                         ioNumberDataPackets,
								  AudioBufferList*                ioData,
								  AudioStreamPacketDescription**  outDataPacketDescription,
								  void*                           inUserData)
	{
		ConverterContext* context = (ConverterContext*)inUserData;
		if (context->flagUsed) {
			return 500;
		} else {
			if (ioData->mNumberBuffers != context->data->mNumberBuffers) {
				return 501;
			}
			sl_uint32 nBuffers = ioData->mNumberBuffers;
			for (sl_uint32 i = 0; i < nBuffers; i++) {
				ioData->mBuffers[i].mData = context->data->mBuffers[i].mData;
				ioData->mBuffers[i].mDataByteSize = context->data->mBuffers[i].mDataByteSize;
				ioData->mBuffers[i].mNumberChannels = context->data->mBuffers[i].mNumberChannels;
			}
			*ioNumberDataPackets = ioData->mBuffers[0].mDataByteSize / context->nBytesPerPacket;
			context->flagUsed = sl_true;
		}
		return 0;
	}

	void onFrame(const AudioBufferList* data)
	{
		const AudioBuffer& buffer = data->mBuffers[0];
		sl_uint32 nFrame = buffer.mDataByteSize / m_formatSrc.mBytesPerPacket * m_formatDst.mSampleRate / m_formatSrc.mSampleRate * 2; // double buffer to be enough to convert all source packets
		
		Memory mem = _getMemProcess_S16(nFrame);
		if (mem.isNull()) {
			return;
		}
		sl_int16* output = (sl_int16*)(mem.getBuf());

		AudioBufferList bufferOutput;
		bufferOutput.mNumberBuffers = 1;
		bufferOutput.mBuffers[0].mData = output;
		bufferOutput.mBuffers[0].mDataByteSize = (UInt32)nFrame * 2;
		UInt32 sizeFrame = (UInt32)nFrame;
		ConverterContext context;
		context.flagUsed = sl_false;
		context.data = data;
		context.nBytesPerPacket = m_formatSrc.mBytesPerPacket;
		OSStatus result = AudioConverterFillComplexBuffer(m_converter, ConverterProc, (void*)&context, &sizeFrame, &bufferOutput, NULL);
		if (result == noErr || result == 500) {
			onFrame(output, sizeFrame);
		}
	}
	
	static OSStatus DeviceIOProc(AudioObjectID           inDevice,
								 const AudioTimeStamp*   inNow,
								 const AudioBufferList*  inInputData,
								 const AudioTimeStamp*   inInputTime,
								 AudioBufferList*        outOutputData,
								 const AudioTimeStamp*   inOutputTime,
								 void*                   inClientData)
	{
		_OSX_AudioRecorder* object = (_OSX_AudioRecorder*)(inClientData);
		object->onFrame(inInputData);
		return 0;
	}
	
};

class _OSX_AudioPlayerBuffer : public AudioPlayerBuffer
{
private:
    _OSX_AudioPlayerBuffer()
    {
        m_flagOpened = sl_true;
        m_flagRunning = sl_false;
		m_callback = sl_null;
    }
	
public:
	sl_bool m_flagOpened;
	sl_bool m_flagRunning;
	
    AudioDeviceID m_deviceID;
    AudioConverterRef m_converter;
    AudioDeviceIOProcID m_callback;
	
    AudioStreamBasicDescription m_formatSrc;
    AudioStreamBasicDescription m_formatDst;
	
	~_OSX_AudioPlayerBuffer()
	{
		release();
	}
    
    static void logError(String text)
    {
        SLIB_LOG_ERROR("AudioPlayer", text);
    }
    
    static Ref<_OSX_AudioPlayerBuffer> create(const AudioPlayerBufferParam& param, const AudioDeviceID deviceID)
    {
        Ref<_OSX_AudioPlayerBuffer> ret;
        
        AudioObjectPropertyAddress prop;
        prop.mSelector = kAudioDevicePropertyStreamFormat;
        prop.mScope = kAudioDevicePropertyScopeOutput;
        prop.mElement = kAudioObjectPropertyElementMaster;
        
        UInt32 sizeValue;
        AudioStreamBasicDescription formatDst;
        Base::resetMemory(&formatDst, 0, sizeof(formatDst));
        sizeValue = sizeof(formatDst);
        if (AudioObjectGetPropertyData(deviceID, &prop, 0, NULL, &sizeValue, &formatDst) != kAudioHardwareNoError) {
            logError("Failed to get source input format");
            return ret;
        }
        
        prop.mSelector = kAudioDevicePropertyBufferSizeRange;
        AudioValueRange rangeBufferSize;
        sizeValue = sizeof(rangeBufferSize);
        if (AudioObjectGetPropertyData(deviceID, &prop, 0, NULL, &sizeValue, &rangeBufferSize) != kAudioHardwareNoError) {
            logError("Failed to get buffer size range");
            return ret;
        }
        
        UInt32 sizeFrame = param.nFrameLengthInMilliseconds * formatDst.mSampleRate * formatDst.mBytesPerFrame / 1000;
        if (sizeFrame < rangeBufferSize.mMinimum) {
            logError("Required frame size(" + String::fromUint32(sizeFrame) + ") is smaller than minimum " + String::fromUint32(rangeBufferSize.mMinimum));
            return ret;
        }
        if (sizeFrame > rangeBufferSize.mMaximum) {
            logError("Required frame size(" + String::fromUint32(sizeFrame) + ") is bigger than maximum " + String::fromUint32(rangeBufferSize.mMinimum));
            return ret;
        }

        // Create Audio Converter
        AudioStreamBasicDescription formatSrc;
        formatSrc.mFormatID = kAudioFormatLinearPCM;
        formatSrc.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
        formatSrc.mSampleRate = param.nSamplesPerSecond;
        formatSrc.mBitsPerChannel = 16;
        formatSrc.mChannelsPerFrame = 1;
        formatSrc.mBytesPerFrame = formatSrc.mChannelsPerFrame * formatSrc.mBitsPerChannel / 8;
        formatSrc.mFramesPerPacket = 1;
        formatSrc.mBytesPerPacket = formatSrc.mBytesPerFrame * formatSrc.mFramesPerPacket;
        
        AudioConverterRef converter;
        if (AudioConverterNew(&formatSrc, &formatDst, &converter) == kAudioHardwareNoError) {
			
			prop.mSelector = kAudioDevicePropertyBufferSize;
			sizeValue = sizeof(sizeFrame);
			
			if (AudioObjectSetPropertyData(deviceID, &prop, 0, NULL, sizeValue, &sizeFrame) == kAudioHardwareNoError) {
				
				ret = new _OSX_AudioPlayerBuffer();
				
				if (ret.isNotNull()) {
					
					ret->m_deviceID = deviceID;
					ret->m_converter = converter;
					ret->m_queue.setQueueSize(param.nSamplesPerSecond * param.nBufferLengthInMilliseconds / 1000);
					ret->m_formatSrc = formatSrc;
					ret->m_formatDst = formatDst;
					ret->setListener(param.listener);
					
					AudioDeviceIOProcID callback;
					if (AudioDeviceCreateIOProcID(deviceID, DeviceIOProc, ret.getObject(), &callback) == kAudioHardwareNoError) {
						ret->m_callback = callback;
						if (param.flagAutoStart) {
							ret->start();
						}
						return ret;
					} else {
						logError("Failed to create io proc");
					}
					
					return Ref<_OSX_AudioPlayerBuffer>::null();
				}
			} else {
				logError("Failed to get set buffer size");
			}
			AudioConverterDispose(converter);
        } else {
            logError("Failed to create audio converter");
        }
        return ret;
    }
	
	void release()
	{
		MutexLocker lock(getLocker());
		if (!m_flagOpened) {
			return;
		}
		stop();
		m_flagOpened = sl_false;
		if (m_callback) {
			AudioDeviceDestroyIOProcID(m_deviceID, m_callback);
		}
		AudioConverterDispose(m_converter);
	}
	
	sl_bool isOpened()
	{
		return m_flagOpened;
	}
	
	void start()
	{
		MutexLocker lock(getLocker());
		if (!m_flagOpened) {
			return;
		}
		if (m_flagRunning) {
			return;
		}
		m_flagRunning = sl_true;
		if (AudioDeviceStart(m_deviceID, m_callback) != kAudioHardwareNoError) {
			logError("Failed to start device");
		}
	}
	
    void stop()
    {
		MutexLocker lock(getLocker());
		if (!m_flagOpened) {
			return;
		}
		if (!m_flagRunning) {
			return;
		}
		m_flagRunning = sl_false;
		if (AudioDeviceStop(m_deviceID, m_callback) != kAudioHardwareNoError) {
			logError("Failed to stop play device");
		}
    }
    
    sl_bool isRunning()
    {
        return m_flagRunning;
    }
    
    Memory m_dataConvert;
    void onConvert(sl_uint32 nFrames, AudioBufferList* data)
    {
        sl_uint32 nChannels = m_formatSrc.mChannelsPerFrame;
        sl_uint32 nSamples = nFrames * nChannels;

		m_dataConvert = _getMemProcess_S16(nFrames);
		if (m_dataConvert.isNull()) {
			return;
		}
		sl_int16* s = (sl_int16*)(m_dataConvert.getBuf());
		
		_processFrame_S16(s, nSamples);
		
		data->mBuffers[0].mDataByteSize = (UInt32)nSamples * 2;
        data->mBuffers[0].mData = s;
        data->mBuffers[0].mNumberChannels = (UInt32)nChannels;
    }
    
    static OSStatus ConverterProc(AudioConverterRef               inAudioConverter,
                                  UInt32*                         ioNumberDataPackets,
                                  AudioBufferList*                ioData,
                                  AudioStreamPacketDescription**  outDataPacketDescription,
                                  void*                           inUserData)
    {
        _OSX_AudioPlayerBuffer* object = (_OSX_AudioPlayerBuffer*)inUserData;
        object->onConvert(*ioNumberDataPackets, ioData);
        return noErr;
    }
    
    OSStatus onFrame(AudioBufferList *outputData)
    {		
		UInt32 size = outputData->mBuffers->mDataByteSize / m_formatDst.mBytesPerFrame;
        AudioConverterFillComplexBuffer(m_converter, ConverterProc, this, &size, outputData, NULL);
        return 0;
    }
    static OSStatus DeviceIOProc(AudioDeviceID, const AudioTimeStamp*,
                                           const AudioBufferList* inputData,
                                           const AudioTimeStamp* inputTime,
                                           AudioBufferList* outputData,
                                           const AudioTimeStamp* outputTime,
                                           void *clientData)
    {
        _OSX_AudioPlayerBuffer* object = (_OSX_AudioPlayerBuffer*)(clientData);
		
		return object->onFrame(outputData);
    }
};

class _OSX_AudioPlayer : public AudioPlayer
{
private:
    _OSX_AudioPlayer()
    {
        m_deviceID = sl_null;
    }
    
public:
    AudioDeviceID m_deviceID;
	
    ~_OSX_AudioPlayer()
    {
    }
    
    static void logError(String text)
    {
        SLIB_LOG_ERROR("AudioPlayer", text);
    }
    
    static Ref<_OSX_AudioPlayer> create(const AudioPlayerParam& param)
    {
		_OSX_AudioDeviceInfo deviceInfo;
		if (!_OSX_AudioDeviceInfo::_selectDevice(sl_false, param.deviceId, deviceInfo)) {
			logError("Failed to find audio ouptut device - " + param.deviceId);
			return Ref<_OSX_AudioPlayer>::null();
		}

		Ref<_OSX_AudioPlayer> ret = new _OSX_AudioPlayer();
		if (ret.isNotNull()) {
			ret->m_deviceID = deviceInfo.id;
		}		
        return ret;
    }
    
    Ref<AudioPlayerBuffer> createBuffer(const AudioPlayerBufferParam& param)
    {
        if (m_deviceID != sl_null) {
            return Ref<AudioPlayerBuffer>::from(_OSX_AudioPlayerBuffer::create(param, m_deviceID));
        } else {
            return Ref<AudioPlayerBuffer>::null();
        }
    }
};


Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
{
	return Ref<AudioRecorder>::from(_OSX_AudioRecorder::create(param));
}

Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
{
    return Ref<AudioPlayer>::from(_OSX_AudioPlayer::create(param));
}

List<AudioDeviceInfo> AudioRecorder::getDevicesList()
{
	return _OSX_AudioDeviceInfo::_getDeviceInfos(sl_true);
}

List<AudioDeviceInfo> AudioPlayer::getDevicesList()
{
	return _OSX_AudioDeviceInfo::_getDeviceInfos(sl_false);
}

SLIB_AUDIO_NAMESPACE_END

#endif
