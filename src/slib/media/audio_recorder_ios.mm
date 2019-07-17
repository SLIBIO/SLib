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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "slib/media/audio_recorder.h"

#include "slib/core/log.h"
#include "slib/core/platform_apple.h"

#import <Foundation/Foundation.h>
#import <AudioUnit/AudioUnit.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>

namespace slib
{
	
	namespace priv
	{
		namespace audio_recorder
		{
			
			class AudioRecorderImpl: public AudioRecorder
			{
			private:
				AudioRecorderImpl()
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
				
				~AudioRecorderImpl()
				{
					release();
				}
				
				static void logError(String text)
				{
					LogError("AudioRecorder", text);
				}
				
				static Ref<AudioRecorderImpl> create(const AudioRecorderParam& param)
				{
					Ref<AudioRecorderImpl> ret;
					
					if (param.channelsCount != 1 && param.channelsCount != 2) {
						return ret;
					}
					
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
								formatDst.mSampleRate = param.samplesPerSecond;
								formatDst.mBitsPerChannel = 16;
								formatDst.mChannelsPerFrame = param.channelsCount;
								formatDst.mBytesPerFrame = formatDst.mChannelsPerFrame * formatDst.mBitsPerChannel / 8;
								formatDst.mFramesPerPacket = 1;
								formatDst.mBytesPerPacket = formatDst.mBytesPerFrame * formatDst.mFramesPerPacket;
								
								AudioConverterRef converter;
								if (AudioConverterNew(&formatSrc, &formatDst, &converter) == noErr) {
									
									ret = new AudioRecorderImpl();
									
									if (ret.isNotNull()) {
										
										ret->m_audioUnitInput = audioUnitInput;
										ret->m_converter = converter;
										ret->m_formatSrc = formatSrc;
										ret->m_formatDst = formatDst;
										
										ret->_init(param);
										
										AURenderCallbackStruct cs;
										cs.inputProc = CallbackInput;
										cs.inputProcRefCon = ret.get();
										
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
										
										return sl_null;
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
					ObjectLocker lock(this);
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
					ObjectLocker lock(this);
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
					ObjectLocker lock(this);
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
					sl_uint32 nFrames = buffer.mDataByteSize / m_formatSrc.mBytesPerPacket * m_formatDst.mSampleRate / m_formatSrc.mSampleRate * 2; // double buffer to be enough to convert all source packets
					
					sl_uint32 nChannels = m_param.channelsCount;
					sl_uint32 nSamples = nFrames * nChannels;
					
					Array<sl_int16> arrData = _getProcessData(nSamples);
					if (arrData.isNull()) {
						return;
					}
					sl_int16* output = arrData.getData();

					AudioBufferList bufferOutput;
					bufferOutput.mNumberBuffers = 1;
					bufferOutput.mBuffers[0].mData = output;
					bufferOutput.mBuffers[0].mDataByteSize = (UInt32)(nFrames * 2);
					bufferOutput.mBuffers[0].mNumberChannels = nChannels;
					
					UInt32 sizeFrame = (UInt32)nFrames;
					ConverterContext context;
					context.flagUsed = sl_false;
					context.data = data;
					context.nBytesPerPacket = m_formatSrc.mBytesPerPacket;
					OSStatus result = AudioConverterFillComplexBuffer(m_converter, ConverterInputProc, (void*)&context, &sizeFrame, &bufferOutput, NULL);
					if (result == noErr || result == 500) {
						_processFrame(output, sizeFrame * nChannels);
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
					AudioRecorderImpl* object = (AudioRecorderImpl*)(inRefCon);
					
					AudioStreamBasicDescription& format = object->m_formatSrc;
					UInt32 nChannels = format.mChannelsPerFrame;
					UInt32 nSize = format.mBytesPerFrame * inNumberFrames;
					
					Memory mem = object->getInputMemory(nSize * nChannels);
					if (mem.isNull()) {
						return 0;
					}
					
					char* buf = (char*)(mem.getData());
					
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
			
		}
	}

	Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
	{
		return priv::audio_recorder::AudioRecorderImpl::create(param);
	}

	List<AudioRecorderInfo> AudioRecorder::getRecordersList()
	{
		AudioRecorderInfo ret;
		SLIB_STATIC_STRING(s, "Internal Microphone");
		ret.name = s;
		return List<AudioRecorderInfo>::createFromElement(ret);
	}

}

#endif
