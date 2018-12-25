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

#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_RECORDER
#define CHECKHEADER_SLIB_MEDIA_AUDIO_RECORDER

#include "definition.h"

#include "audio_data.h"

#include "../core/object.h"
#include "../core/event.h"
#include "../core/loop_queue.h"
#include "../core/string.h"
#include "../core/function.h"

namespace slib
{
	class SLIB_EXPORT AudioRecorderInfo
	{
	public:
		String id;
		String name;
		String description;
		
	public:
		AudioRecorderInfo();
		
		~AudioRecorderInfo();
		
	};
	
	class AudioRecorder;
	
	class SLIB_EXPORT AudioRecorderParam
	{
	public:
		String deviceId;
		sl_uint32 samplesPerSecond;
		sl_uint32 channelsCount;
		sl_uint32 frameLengthInMilliseconds;
		sl_uint32 bufferLengthInMilliseconds;
		
		sl_bool flagAutoStart;
		
		Function<void(AudioRecorder*, AudioData const&)> onRecordAudio;
		Ref<Event> event;
		
	public:
		AudioRecorderParam();
		
		~AudioRecorderParam();
		
	};
	
	class SLIB_EXPORT AudioRecorder : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		AudioRecorder();
		
		~AudioRecorder();
		
	public:
		static Ref<AudioRecorder> create(const AudioRecorderParam& param);
		
		static List<AudioRecorderInfo> getRecordersList();
		
	public:
		virtual void release() = 0;
		
		virtual sl_bool isOpened() = 0;
		
		virtual void start() = 0;
		
		virtual void stop() = 0;
		
		virtual sl_bool isRunning() = 0;
		
	public:
		sl_bool read(const AudioData& audio);
		
	protected:
		void _init(const AudioRecorderParam& param);
		
		Array<sl_int16> _getProcessData(sl_uint32 count);
		
		void _processFrame(sl_int16* s, sl_uint32 count);
		
	protected:
		LoopQueue<sl_int16> m_queue;
		sl_uint32 m_nChannels;
		AtomicArray<sl_int16> m_processData;
		
		Function<void(AudioRecorder*, AudioData const&)> m_onRecordAudio;
		Ref<Event> m_event;
		
	};	
}

#endif
