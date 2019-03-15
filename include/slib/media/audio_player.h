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

#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_PLAYER
#define CHECKHEADER_SLIB_MEDIA_AUDIO_PLAYER

#include "definition.h"

#include "audio_data.h"

#include "../core/object.h"
#include "../core/event.h"
#include "../core/array.h"
#include "../core/loop_queue.h"
#include "../core/string.h"
#include "../core/memory.h"
#include "../core/function.h"

namespace slib
{
	class SLIB_EXPORT AudioPlayerInfo
	{
	public:
		String id;
		String name;
		String description;
		
	public:
		AudioPlayerInfo();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(AudioPlayerInfo)
		
	};
	
	class AudioPlayerBuffer;
	
	class SLIB_EXPORT AudioPlayerBufferParam
	{
	public:
		sl_uint32 samplesPerSecond;
		sl_uint32 channelsCount;
		sl_uint32 bufferLengthInMilliseconds;
		sl_uint32 frameLengthInMilliseconds;
		
		sl_bool flagAutoStart;
		
		Function<void(AudioPlayerBuffer*, sl_uint32 requestedSamplesCount)> onRequireAudioData;
		Ref<Event> event;
		
	public:
		AudioPlayerBufferParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(AudioPlayerBufferParam)
		
	};
	
	class SLIB_EXPORT AudioPlayerBuffer : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		AudioPlayerBuffer();
		
		~AudioPlayerBuffer();
		
	public:
		virtual void release() = 0;
		
		virtual sl_bool isOpened() = 0;
		
		virtual void start() = 0;
		
		virtual void stop() = 0;
		
		virtual sl_bool isRunning() = 0;
		
	public:
		const AudioPlayerBufferParam& getParam();
		
		void write(const AudioData& audioPlay);
		
		void flush();
		
		sl_size getSamplesCountInQueue();
		
	protected:
		void _init(const AudioPlayerBufferParam& param);
		
		Array<sl_int16> _getProcessData(sl_uint32 count);
		
		void _processFrame(sl_int16* s, sl_uint32 count);
		
	protected:
		AudioPlayerBufferParam m_param;
		
		LoopQueue<sl_int16> m_queue;
		sl_int16 m_lastSample;
		AtomicArray<sl_int16> m_processData;
	};
	
	class SLIB_EXPORT AudioPlayerParam
	{
	public:
		String deviceId;
		
	public:
		AudioPlayerParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(AudioPlayerParam)
		
	};
	
	class SLIB_EXPORT AudioPlayer : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		AudioPlayer();
		
		~AudioPlayer();
		
	public:
		static Ref<AudioPlayer> create(const AudioPlayerParam& param);
		
		static Ref<AudioPlayer> create();
		
		static List<AudioPlayerInfo> getPlayersList();
		
	public:
		virtual Ref<AudioPlayerBuffer> createBuffer(const AudioPlayerBufferParam& param) = 0;
		
	};	
}

#endif
