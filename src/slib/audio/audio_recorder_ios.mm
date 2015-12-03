#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/audio/recorder.h"
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

Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
{
    return Ref<AudioRecorder>::from(_iOS_AudioRecorder::create(param));;
}

List<AudioRecorderInfo> AudioRecorder::getRecordersList()
{
	AudioRecorderInfo ret;
	SLIB_STATIC_STRING(s, "Internal Microphone");
	ret.name = s;
	return List<AudioRecorderInfo>::fromElement(ret);
}
SLIB_AUDIO_NAMESPACE_END

#endif
