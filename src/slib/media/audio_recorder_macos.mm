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

#include "slib/media/audio_recorder.h"

#include "slib/core/log.h"

#include "media_platform_macos.h"

namespace slib
{

	class _priv_macOS_AudioRecorder : public AudioRecorder
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
		_priv_macOS_AudioRecorder()
		{
			m_flagOpened = sl_true;
			m_flagRunning = sl_false;
			
			m_callback = sl_null;
		}

		~_priv_macOS_AudioRecorder()
		{
			release();
		}
		
	public:
		static void logError(String text)
		{
			LogError("AudioRecorder", text);
		}
		
		static Ref<_priv_macOS_AudioRecorder> create(const AudioRecorderParam& param)
		{
			Ref<_priv_macOS_AudioRecorder> ret;
			
			if (param.channelsCount != 1 && param.channelsCount != 2) {
				return ret;
			}
			
			_priv_macOS_AudioDeviceInfo deviceInfo;
			if (!(deviceInfo.selectDevice(sl_true, param.deviceId))) {
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
			
			UInt32 sizeFrame = param.frameLengthInMilliseconds * formatSrc.mSampleRate * formatSrc.mBytesPerFrame / 1000;
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
			formatDst.mSampleRate = param.samplesPerSecond;
			formatDst.mBitsPerChannel = 16;
			formatDst.mChannelsPerFrame = param.channelsCount;
			formatDst.mBytesPerFrame = formatDst.mChannelsPerFrame * formatDst.mBitsPerChannel / 8;
			formatDst.mFramesPerPacket = 1;
			formatDst.mBytesPerPacket = formatDst.mBytesPerFrame * formatDst.mFramesPerPacket;
			
			AudioConverterRef converter;
			if (AudioConverterNew(&formatSrc, &formatDst, &converter) == kAudioHardwareNoError) {
				
				prop.mSelector = kAudioDevicePropertyBufferSize;
				sizeValue = sizeof(sizeFrame);
				
				if (AudioObjectSetPropertyData(deviceID, &prop, 0, NULL, sizeValue, &sizeFrame) == kAudioHardwareNoError) {
						
					ret = new _priv_macOS_AudioRecorder();
					
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
			
			if (AudioDeviceStart(m_deviceID, m_callback) != kAudioHardwareNoError) {
				logError("Failed to start device");
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
			
			if (AudioDeviceStop(m_deviceID, m_callback) != kAudioHardwareNoError) {
				logError("Failed to stop play device");
			}
		}
		
		sl_bool isRunning()
		{
			return m_flagRunning;
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
			bufferOutput.mBuffers[0].mDataByteSize = (UInt32)nSamples * 2;
			bufferOutput.mBuffers[0].mNumberChannels = nChannels;
			
			UInt32 sizeFrame = (UInt32)nFrames;
			ConverterContext context;
			context.flagUsed = sl_false;
			context.data = data;
			context.nBytesPerPacket = m_formatSrc.mBytesPerPacket;
			OSStatus result = AudioConverterFillComplexBuffer(m_converter, ConverterProc, (void*)&context, &sizeFrame, &bufferOutput, NULL);
			if (result == noErr || result == 500) {
				_processFrame(output, sizeFrame * nChannels);
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
			_priv_macOS_AudioRecorder* object = (_priv_macOS_AudioRecorder*)(inClientData);
			object->onFrame(inInputData);
			return 0;
		}
		
	};

	Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
	{
		return _priv_macOS_AudioRecorder::create(param);
	}

	List<AudioRecorderInfo> AudioRecorder::getRecordersList()
	{
		ListElements<_priv_macOS_AudioDeviceInfo> list(_priv_macOS_AudioDeviceInfo::getAllDevices(sl_true));
		List<AudioRecorderInfo> ret;
		for (sl_size i = 0; i < list.count; i++) {
			AudioRecorderInfo info;
			info.id = list[i].uid;
			info.name = list[i].name;
			info.description = list[i].manufacturer;
			ret.add_NoLock(info);
		}
		return ret;
	}

}

#endif
