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

#if defined(SLIB_PLATFORM_IS_MACOS)

#include "slib/media/audio_player.h"

#include "slib/core/log.h"

#include "media_platform_macos.h"

namespace slib
{

	namespace priv
	{
		namespace audio_player
		{

			class AudioPlayerBufferImpl : public AudioPlayerBuffer
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
				AudioPlayerBufferImpl()
				{
					m_flagOpened = sl_true;
					m_flagRunning = sl_false;
					m_callback = sl_null;
				}

				~AudioPlayerBufferImpl()
				{
					release();
				}

			public:
				static void logError(String text)
				{
					LogError("AudioPlayer", text);
				}
				
				static Ref<AudioPlayerBufferImpl> create(const AudioPlayerBufferParam& param, const AudioDeviceID deviceID)
				{
					Ref<AudioPlayerBufferImpl> ret;
					
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
							
							ret = new AudioPlayerBufferImpl();
							
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
				
				void release() override
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
				
				sl_bool isOpened() override
				{
					return m_flagOpened;
				}
				
				void start() override
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
				
				void stop() override
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
				
				sl_bool isRunning() override
				{
					return m_flagRunning;
				}
				
				AtomicArray<sl_int16> m_dataConvert;
				void onConvert(sl_uint32 nFrames, AudioBufferList* data)
				{
					sl_uint32 nChannels = m_param.channelsCount;
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
					AudioPlayerBufferImpl* object = (AudioPlayerBufferImpl*)inUserData;
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
					AudioPlayerBufferImpl* object = (AudioPlayerBufferImpl*)(clientData);
					
					return object->onFrame(outputData);
				}
			};

			class AudioPlayerImpl : public AudioPlayer
			{
			public:
				AudioDeviceID m_deviceID;
				
			public:
				AudioPlayerImpl()
				{
					m_deviceID = 0;
				}

				~AudioPlayerImpl()
				{
				}
				
			public:
				static void logError(String text)
				{
					LogError("AudioPlayer", text);
				}
				
				static Ref<AudioPlayerImpl> create(const AudioPlayerParam& param)
				{
					priv::media_platform::AudioDeviceInfo deviceInfo;
					if (!(deviceInfo.selectDevice(sl_false, param.deviceId))) {
						logError("Failed to find audio ouptut device - " + param.deviceId);
						return sl_null;
					}

					Ref<AudioPlayerImpl> ret = new AudioPlayerImpl();
					if (ret.isNotNull()) {
						ret->m_deviceID = deviceInfo.id;
					}		
					return ret;
				}
				
				Ref<AudioPlayerBuffer> createBuffer(const AudioPlayerBufferParam& param) override
				{
					if (m_deviceID != 0) {
						return AudioPlayerBufferImpl::create(param, m_deviceID);
					}
					return sl_null;
				}
				
			};

		}
	}

	Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
	{
		return priv::audio_player::AudioPlayerImpl::create(param);
	}

	List<AudioPlayerInfo> AudioPlayer::getPlayersList()
	{
		ListElements<priv::media_platform::AudioDeviceInfo> list(priv::media_platform::AudioDeviceInfo::getAllDevices(sl_false));
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

}

#endif
