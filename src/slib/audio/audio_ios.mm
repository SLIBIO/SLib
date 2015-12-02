#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/audio/object.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/pointer.h"
#include "../../../inc/slib/core/platform_apple.h"

#import <Foundation/Foundation.h>
#import <AudioUnit/AudioUnit.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>

SLIB_AUDIO_NAMESPACE_BEGIN

class _iOS_AudioRecorder: public AudioRecorder
{
private:
	_iOS_AudioRecorder()
	{
		m_flagOpened = sl_true;
		m_flagRunning = sl_false;
		m_flagInitializedUnit = sl_false;
	}
	
public:
	sl_bool m_flagRunning;
	sl_bool m_flagOpened;
	sl_bool m_flagInitializedUnit;
	
	AudioComponentInstance m_audioUnitInput;
	AudioConverterRef m_converter;
	
	AudioStreamBasicDescription m_formatSrc;
	AudioStreamBasicDescription m_formatDst;
	sl_uint32 m_nSamplesPerFrame;
	
	~_iOS_AudioRecorder()
	{
		release();
	}
	
	static void logError(String text)
	{
		SLIB_LOG_ERROR("AudioRecorder", text);
	}
	
	static Ref<_iOS_AudioRecorder> create(const AudioRecorderParam& param)
	{
		Ref<_iOS_AudioRecorder> ret;
		
		AudioComponentDescription desc;
		desc.componentType = kAudioUnitType_Output;
		desc.componentSubType = kAudioUnitSubType_RemoteIO;
		desc.componentManufacturer = kAudioUnitManufacturer_Apple;
		desc.componentFlags = 0;
		desc.componentFlagsMask = 0;
		
		AudioComponent comp = AudioComponentFindNext(NULL, &desc);
		if (! comp) {
			logError("Failed to find audio component");
			return ret;
		}
		
		OSStatus result;
		AudioComponentInstance audioUnitInput;
		result = AudioComponentInstanceNew(comp, &audioUnitInput);
		if (result == noErr) {
			UInt32 enableIO = 1;
			AudioUnitElement bus1 = 1;
			result = AudioUnitSetProperty(audioUnitInput,
										  kAudioOutputUnitProperty_EnableIO,
										  kAudioUnitScope_Input,
										  bus1, // input bus
										  &enableIO,
										  sizeof(enableIO));
			if (result == noErr) {
				
				AudioStreamBasicDescription formatSrc;
				UInt32 size = sizeof(formatSrc);
				
				result = AudioUnitGetProperty(audioUnitInput, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, bus1, &formatSrc, &size);
				if (result == noErr) {
					
#if defined(SLIB_PLATFORM_IS_IOS_SIMULATOR)
					formatSrc.mSampleRate = 44100;
#else
					formatSrc.mSampleRate = [[AVAudioSession sharedInstance] sampleRate];
#endif
					
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
					if (AudioConverterNew(&formatSrc, &formatDst, &converter) == noErr) {
						
						ret = new _iOS_AudioRecorder();
						
						if (ret.isNotNull()) {
							
							ret->m_audioUnitInput = audioUnitInput;
							ret->m_converter = converter;
							ret->m_nSamplesPerFrame = param.nSamplesPerSecond * param.nFrameLengthInMilliseconds / 1000;
							ret->m_queue.setQueueSize(param.nSamplesPerSecond * param.nBufferLengthInMilliseconds / 1000);
							ret->m_formatSrc = formatSrc;
							ret->m_formatDst = formatDst;
							ret->setListener(param.listener);
							
							AURenderCallbackStruct cs;
							cs.inputProc = CallbackInput;
							cs.inputProcRefCon = ret.getObject();
							
							result = AudioUnitSetProperty(audioUnitInput,
														  kAudioOutputUnitProperty_SetInputCallback,
														  kAudioUnitScope_Input,
														  bus1,
														  &cs,
														  sizeof(cs));
							if (result == noErr) {
								result = AudioUnitInitialize(audioUnitInput);
								if (result == noErr) {
									ret->m_flagInitializedUnit = sl_true;
									if (param.flagAutoStart) {
										ret->start();
									}
									return ret;
								} else {
									logError("Failed to initialize audio unit");
								}
							} else {
								logError("Failed to set callback");
							}
							
							return Ref<_iOS_AudioRecorder>::null();
						}
						
						AudioConverterDispose(converter);
						
					} else {
						logError("Failed to create audio converter");
					}
					
				} else {
					logError("Failed to get stream format");
				}
			} else {
				logError("Failed to enable output");
			}
			
			AudioComponentInstanceDispose(audioUnitInput);
			
		} else {
			logError("Failed to create audio component instance");
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
		if (m_flagInitializedUnit) {
			AudioUnitUninitialize(m_audioUnitInput);
		}
		AudioComponentInstanceDispose(m_audioUnitInput);
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
		if (AudioOutputUnitStart(m_audioUnitInput) != noErr) {
			logError("Failed to start audio unit");
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
		if (AudioOutputUnitStop(m_audioUnitInput) != noErr) {
			logError("Failed to stop audio unit");
		}
	}
	
	virtual sl_bool isRunning()
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
	
	static OSStatus ConverterInputProc(AudioConverterRef          inAudioConverter,
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
	
	void onFrame(AudioBufferList* data)
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
		bufferOutput.mBuffers[0].mDataByteSize = (UInt32)(nFrame * 2);
		
		UInt32 sizeFrame = (UInt32)nFrame;
		ConverterContext context;
		context.flagUsed = sl_false;
		context.data = data;
		context.nBytesPerPacket = m_formatSrc.mBytesPerPacket;
		OSStatus result = AudioConverterFillComplexBuffer(m_converter, ConverterInputProc, (void*)&context, &sizeFrame, &bufferOutput, NULL);
		if (result == noErr || result == 500) {
			onFrame(output, sizeFrame);
		}
	}
	
	Memory m_memInput;
	Memory getInputMemory(sl_size size)
	{
		Memory mem = m_memInput;
		if (mem.getSize() >= size) {
			return mem;
		}
		mem = Memory::create(size);
		m_memInput = mem;
		return mem;
	}
	
	static OSStatus CallbackInput(void *inRefCon,
								  AudioUnitRenderActionFlags *ioActionFlags,
								  const AudioTimeStamp *inTimeStamp,
								  UInt32 inBusNumber,
								  UInt32 inNumberFrames,
								  AudioBufferList *ioData)
	{
		_iOS_AudioRecorder* object = (_iOS_AudioRecorder*)(inRefCon);
		
		AudioStreamBasicDescription& format = object->m_formatSrc;
		UInt32 nChannels = format.mChannelsPerFrame;
		UInt32 nSize = format.mBytesPerFrame * inNumberFrames;
		
		Memory mem = object->getInputMemory(nSize * nChannels);
		if (mem.isNull()) {
			return 0;
		}
		
		char* buf = (char*)(mem.getBuf());
		
		char _abList[sizeof(AudioBufferList) * 2];
		AudioBufferList& abList = *((AudioBufferList*)_abList);
		
		if (nChannels == 2 && (format.mFormatFlags & kAudioFormatFlagIsNonInterleaved)) {
			abList.mNumberBuffers = 2;
			abList.mBuffers[0].mData = buf;
			abList.mBuffers[0].mDataByteSize = nSize;
			abList.mBuffers[0].mNumberChannels = 1;
			abList.mBuffers[1].mData = buf + nSize;
			abList.mBuffers[1].mDataByteSize = nSize;
			abList.mBuffers[1].mNumberChannels = 1;
		} else {
			abList.mNumberBuffers = 1;
			abList.mBuffers[0].mData = buf;
			abList.mBuffers[0].mDataByteSize = nSize;
			abList.mBuffers[0].mNumberChannels = 1;
		}
		
		OSStatus res = AudioUnitRender(object->m_audioUnitInput,
									   ioActionFlags, inTimeStamp,
									   inBusNumber, inNumberFrames, &abList);
		
		if (res == 0) {
			object->onFrame(&abList);
		}
		
		return 0;
	}
};

class _iOS_AudioPlayerBuffer : public AudioPlayerBuffer
{
private:
	_iOS_AudioPlayerBuffer()
	{
		m_flagInitialized = sl_false;
		m_flagOpened = sl_true;
		m_flagRunning = sl_false;
		m_lastValue = 0;
	}
	
	
public:
	sl_bool m_flagInitialized;
	sl_bool m_flagOpened;
	sl_bool m_flagRunning;
	
	AudioComponentInstance m_audioUnitOutput;
	AudioConverterRef m_converter;
	
	AudioStreamBasicDescription m_formatSrc;
	AudioStreamBasicDescription m_formatDst;
	sl_uint32 m_nSamplesPerFrame;
	sl_int16 m_lastValue;
	
	~_iOS_AudioPlayerBuffer()
	{
		release();
	}
	
	static void logError(String text)
	{
		SLIB_LOG_ERROR("AudioPlayer", text);
	}
	
	static Ref<_iOS_AudioPlayerBuffer> create(const AudioPlayerBufferParam& param)
	{
		Ref<_iOS_AudioPlayerBuffer> ret;
		
		AudioComponentDescription desc;
		desc.componentType = kAudioUnitType_Output;
		desc.componentSubType = kAudioUnitSubType_RemoteIO;
		desc.componentManufacturer = kAudioUnitManufacturer_Apple;
		desc.componentFlags = 0;
		desc.componentFlagsMask = 0;
		
		AudioComponent comp = AudioComponentFindNext(NULL, &desc);
		if (! comp) {
			logError("Failed to find audio component");
			return ret;
		}
		
		OSStatus result;
		AudioComponentInstance audioUnitOutput;
		result = AudioComponentInstanceNew(comp, &audioUnitOutput);
		if (result == noErr) {
			UInt32 enableOutput = 1;
			AudioUnitElement bus0 = 0;
			result = AudioUnitSetProperty(audioUnitOutput,
										  kAudioOutputUnitProperty_EnableIO,
										  kAudioUnitScope_Output,
										  bus0, // output bus
										  &enableOutput,
										  sizeof(enableOutput));
			if (result == noErr) {
				
				AudioStreamBasicDescription formatDst;
				UInt32 size = sizeof(formatDst);
				
				result = AudioUnitGetProperty(audioUnitOutput, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, bus0, &formatDst, &size);
				if (result == noErr) {
					
#if defined(SLIB_PLATFORM_IS_IOS_SIMULATOR)
					formatDst.mSampleRate = 44100;
#else
					formatDst.mSampleRate = [[AVAudioSession sharedInstance] sampleRate];
#endif
					
					AudioStreamBasicDescription formatSrc;
					formatSrc.mFormatID = kAudioFormatLinearPCM;
					formatSrc.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
					formatSrc.mSampleRate = param.nSamplesPerSecond;
					formatSrc.mBitsPerChannel = 16;
					formatSrc.mChannelsPerFrame = (UInt32)(param.nChannels);
					formatSrc.mBytesPerFrame = formatSrc.mChannelsPerFrame * formatSrc.mBitsPerChannel / 8;
					formatSrc.mFramesPerPacket = 1;
					formatSrc.mBytesPerPacket = formatSrc.mBytesPerFrame * formatSrc.mFramesPerPacket;
					
					AudioConverterRef converter;
					if (AudioConverterNew(&formatSrc, &formatDst, &converter) == noErr) {
						
						ret = new _iOS_AudioPlayerBuffer();
						
						if (ret.isNotNull()) {
							
							ret->m_audioUnitOutput = audioUnitOutput;
							ret->m_converter = converter;
							ret->m_nSamplesPerFrame = param.nSamplesPerSecond * param.nFrameLengthInMilliseconds / 1000;
							ret->m_queue.setQueueSize(param.nSamplesPerSecond * param.nBufferLengthInMilliseconds / 1000);
							ret->m_formatSrc = formatSrc;
							ret->m_formatDst = formatDst;
							ret->setListener(param.listener);
							
							AURenderCallbackStruct cs;
							cs.inputProc = CallbackOutput;
							cs.inputProcRefCon = ret.getObject();
							
							result = AudioUnitSetProperty(audioUnitOutput,
														  kAudioUnitProperty_SetRenderCallback,
														  kAudioUnitScope_Input,
														  bus0,
														  &cs,
														  sizeof(cs));
							if (result == noErr) {
								
								result = AudioUnitInitialize(audioUnitOutput);
								if (result == noErr) {
									ret->m_flagInitialized = sl_true;
									if (param.flagAutoStart) {
										ret->start();
									}
									return ret;
								} else {
									logError("Failed to initialize audio unit");
								}
								
							} else {
								logError("Failed to set callback");
							}
							
							return Ref<_iOS_AudioPlayerBuffer>::null();

						}
						
						AudioConverterDispose(converter);
						
					} else {
						logError("Failed to create audio converter");
					}
					
				} else {
					logError("Failed to get stream format");
				}
			} else {
				logError("Failed to enable output");
			}
			
			AudioComponentInstanceDispose(audioUnitOutput);
			
		} else {
			logError("Failed to create audio component instance");
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
		if (m_flagInitialized) {
			AudioUnitUninitialize(m_audioUnitOutput);
		}
		AudioComponentInstanceDispose(m_audioUnitOutput);
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
		if (AudioOutputUnitStart(m_audioUnitOutput) != noErr) {
			logError("Failed to start audio unit");
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
		if (AudioOutputUnitStop(m_audioUnitOutput) != noErr) {
			logError("Failed to stop audio unit");
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
		
		m_dataConvert = _getMemProcess_S16(nSamples);
		if (m_dataConvert.isNull()) {
			return;
		}
		
		sl_int16* s = (sl_int16*)(m_dataConvert.getBuf());
		_processFrame_S16(s, nSamples);

		data->mBuffers[0].mDataByteSize = (UInt32)nSamples * 2;
		data->mBuffers[0].mData = s;
		data->mBuffers[0].mNumberChannels = (UInt32)nChannels;
		m_lastValue = s[nSamples - 1];
	}
	
	static OSStatus ConverterProc(AudioConverterRef               inAudioConverter,
								  UInt32*                         ioNumberDataPackets,
								  AudioBufferList*                ioData,
								  AudioStreamPacketDescription**  outDataPacketDescription,
								  void*                           inUserData)
	{
		_iOS_AudioPlayerBuffer* object = (_iOS_AudioPlayerBuffer*)inUserData;
		object->onConvert(*ioNumberDataPackets, ioData);
		return noErr;
	}
	
	OSStatus onFrame(UInt32 nFrames, AudioBufferList* data)
	{
		UInt32 sizeFrame = nFrames;
		OSStatus result = AudioConverterFillComplexBuffer(m_converter, ConverterProc, this, &sizeFrame, data, NULL);
		return result;
	}
	
	static OSStatus CallbackOutput(void                        *inRefCon,
								   AudioUnitRenderActionFlags  *ioActionFlags,
								   const AudioTimeStamp        *inTimeStamp,
								   UInt32                      inBusNumber,
								   UInt32                      inNumberFrames,
								   AudioBufferList             *ioData)
	{
		_iOS_AudioPlayerBuffer* object = (_iOS_AudioPlayerBuffer*)(inRefCon);
		if (object) {
			return object->onFrame(inNumberFrames, ioData);
		} else {
			return 500;
		}
	}

};

class _iOS_AudioPlayer : public AudioPlayer
{
private:
	_iOS_AudioPlayer() {}
	
public:
	
	~_iOS_AudioPlayer()
	{
	}
	
	static void logError(String text)
	{
		SLIB_LOG_ERROR("AudioPlayer", text);
	}
	
	static Ref<_iOS_AudioPlayer> create(const AudioPlayerParam& param)
	{
		Ref<_iOS_AudioPlayer> ret = new _iOS_AudioPlayer();
		return ret;
	}
	
	Ref<AudioPlayerBuffer> createBuffer(const AudioPlayerBufferParam& param)
	{
		return Ref<AudioPlayerBuffer>::from(_iOS_AudioPlayerBuffer::create(param));
	}
};

Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
{
    return Ref<AudioRecorder>::from(_iOS_AudioRecorder::create(param));;
}

Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
{
	return Ref<AudioPlayer>::from(_iOS_AudioPlayer::create(param));
}

List<AudioDeviceInfo> AudioRecorder::getDevicesList()
{
	AudioDeviceInfo ret;
	SLIB_STATIC_STRING(s, "Internal Microphone");
	ret.name = s;
	return List<AudioDeviceInfo>::fromElement(ret);
}

List<AudioDeviceInfo> AudioPlayer::getDevicesList()
{
	AudioDeviceInfo ret;
	SLIB_STATIC_STRING(s, "Internal Speaker");
	ret.name = s;
	return List<AudioDeviceInfo>::fromElement(ret);
}

SLIB_AUDIO_NAMESPACE_END

#endif
