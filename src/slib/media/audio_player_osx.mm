#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/media/audio_player.h"

#include "media_platform_osx.h"

SLIB_MEDIA_NAMESPACE_BEGIN
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
					ret->m_queue.setQueueSize(param.samplesPerSecond * param.bufferLengthInMilliseconds / 1000);
					ret->m_formatSrc = formatSrc;
					ret->m_formatDst = formatDst;
					ret->setListener(param.listener);
					
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
		OSX_AudioDeviceInfo deviceInfo;
		if (!(deviceInfo.selectDevice(sl_false, param.deviceId))) {
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

Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
{
    return Ref<AudioPlayer>::from(_OSX_AudioPlayer::create(param));
}

List<AudioPlayerInfo> AudioPlayer::getPlayersList()
{
	ListLocker<OSX_AudioDeviceInfo> list(OSX_AudioDeviceInfo::getAllDevices(sl_false));
	List<AudioPlayerInfo> ret;
	for (sl_size i = 0; i < list.count(); i++) {
		AudioPlayerInfo info;
		info.id = list[i].uid;
		info.name = list[i].name;
		info.description = list[i].manufacturer;
		ret.add(info);
	}
	return ret;
}

SLIB_MEDIA_NAMESPACE_END

#endif
