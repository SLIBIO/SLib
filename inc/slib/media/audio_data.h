/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_DATA
#define CHECKHEADER_SLIB_MEDIA_AUDIO_DATA

#include "definition.h"

#include "audio_format.h"

#include "../core/ref.h"

namespace slib
{
	class SLIB_EXPORT AudioChannelBuffer
	{
	public:
		void* data;
		sl_size count; // count of samples
		sl_int32 stride; // bytes offset between samples
		Ref<Referable> ref;
	};
	
	class SLIB_EXPORT AudioData
	{
	public:
		sl_size count; // count of samples per channel
		AudioFormat format;
		
		// main data
		void* data; // samples
		Ref<Referable> ref; // reference for samples
		
		// additional data for non-interleaved formats
		void* data1; // samples
		Ref<Referable> ref1; // reference for samples
		
	public:
		AudioData();
		
		AudioData(const AudioData& other);
		
		~AudioData();
		
	public:
		sl_size getSizeForChannel() const;
		
		sl_size getTotalSize() const;
		
		// returns the number of channels
		sl_uint32 getChannelBuffers(AudioChannelBuffer* buffers) const;
		
		void copySamplesFrom(const AudioData& other, sl_size count) const;
		
		void copySamplesFrom(const AudioData& other) const;
		
	public:
		AudioData& operator=(const AudioData& other);
		
	};	
}

#endif
