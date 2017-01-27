#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/media/audio_player.h"
#include "../../../inc/slib/core/log.h"

#include "media_platform_osx.h"

SLIB_MEDIA_NAMESPACE_BEGIN
class _OSX_AudioPlayerBuffer : public AudioPlayerBuffer
{
public:
	sl_bool m_flagOpened;
	sl_bool m_flagRunning;
	
    AudioDeviceID m_deviceID;
    AudioConverterRef m_converter;
    AudioDeviceIOProcID m_callback;
	
    AudioStreamBasicDescription m_formatSrc;
    AudioStreamBasicDescription m_formatDst;
	
public:
	_OSX_AudioPlayerBuffer()
	{
		m_flagOpened = sl_true;
		m_flagRunning = sl_false;
		m_callback = sl_null;
	}

	~_OSX_AudioPlayerBuffer()
	{
		release();
	}

public:
    static void logError(String text)
    {
        LogError("AudioPlayer", text);
    }
    
    static Ref<_OSX_AudioPlayerBuffer> create(const AudioPlayerBufferParam& param, const AudioDeviceID deviceID)
    {
        Ref<_OSX_AudioPlayerBuffer> ret;
		
		if (param.channelsCount != 1 && param.channelsCount != 2) {
			return ret;
		}
        
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
        
        UInt32 sizeFrame = param.frameLengthInMilliseconds * formatDst.mSampleRate * formatDst.mBytesPerFrame / 1000;
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
        formatSrc.mSampleRate = param.samplesPerSecond;
        formatSrc.mBitsPerChannel = 16;
        formatSrc.mChannelsPerFrame = param.channelsCount;
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
					ret->m_formatSrc = formatSrc;
					ret->m_formatDst = formatDst;
					
					ret->_init(param);
					
					AudioDeviceIOProcID callback;
					if (AudioDeviceCreateIOProcID(deviceID, DeviceIOProc, ret.get(), &callback) == kAudioHardwareNoError) {
						ret->m_callback = callback;
						if (param.flagAutoStart) {
							ret->start();
						}
						return ret;
					} else {
						logError("Failed to create io proc");
					}
					
					return sl_null;
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
	
	// override
	void release()
	{
		ObjectLocker lock(this);
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
	
	// override
	sl_bool isOpened()
	{
		return m_flagOpened;
	}
	
	// override
	void start()
	{
		ObjectLocker lock(this);
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
	
	// override
    void stop()
    {
		ObjectLocker lock(this);
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
	
	// override
    sl_bool isRunning()
    {
        return m_flagRunning;
    }
    
    AtomicArray<sl_int16> m_dataConvert;
    void onConvert(sl_uint32 nFrames, AudioBufferList* data)
    {
        sl_uint32 nChannels = m_nChannels;
        sl_uint32 nSamples = nFrames * nChannels;

		Array<sl_int16> dataConvert = _getProcessData(nSamples);
		m_dataConvert = dataConvert;
		if (dataConvert.isNull()) {
			return;
		}
		sl_int16* s = dataConvert.getData();
		_processFrame(s, nSamples);
		
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
public:
    AudioDeviceID m_deviceID;
	
public:
	_OSX_AudioPlayer()
	{
		m_deviceID = 0;
	}

	~_OSX_AudioPlayer()
    {
    }
	
public:
    static void logError(String text)
    {
        LogError("AudioPlayer", text);
    }
    
    static Ref<_OSX_AudioPlayer> create(const AudioPlayerParam& param)
    {
		OSX_AudioDeviceInfo deviceInfo;
		if (!(deviceInfo.selectDevice(sl_false, param.deviceId))) {
			logError("Failed to find audio ouptut device - " + param.deviceId);
			return sl_null;
		}

		Ref<_OSX_AudioPlayer> ret = new _OSX_AudioPlayer();
		if (ret.isNotNull()) {
			ret->m_deviceID = deviceInfo.id;
		}		
        return ret;
    }
	
	// override
    Ref<AudioPlayerBuffer> createBuffer(const AudioPlayerBufferParam& param)
    {
        if (m_deviceID != 0) {
            return _OSX_AudioPlayerBuffer::create(param, m_deviceID);
        }
        return sl_null;
    }
	
};

Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
{
    return _OSX_AudioPlayer::create(param);
}

List<AudioPlayerInfo> AudioPlayer::getPlayersList()
{
	ListElements<OSX_AudioDeviceInfo> list(OSX_AudioDeviceInfo::getAllDevices(sl_false));
	List<AudioPlayerInfo> ret;
	for (sl_size i = 0; i < list.count; i++) {
		AudioPlayerInfo info;
		info.id = list[i].uid;
		info.name = list[i].name;
		info.description = list[i].manufacturer;
		ret.add_NoLock(info);
	}
	return ret;
}

SLIB_MEDIA_NAMESPACE_END

#endif
