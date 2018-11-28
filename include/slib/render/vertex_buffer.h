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

#ifndef CHECKHEADER_SLIB_RENDER_VERTEX_BUFFER
#define CHECKHEADER_SLIB_RENDER_VERTEX_BUFFER

#include "definition.h"

#include "base.h"

#include "../core/memory.h"

namespace slib
{

	class SLIB_EXPORT VertexBufferInstance : public RenderBaseObjectInstance
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		VertexBufferInstance();
		
		~VertexBufferInstance();
		
	public:
		virtual void notifyUpdated(sl_size offset, sl_size size);
		
	protected:
		sl_size m_updatedOffset;
		sl_size m_updatedSize;
		
	};
	
	class SLIB_EXPORT VertexBuffer : public RenderBaseObject
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		VertexBuffer();
		
		~VertexBuffer();
		
	public:
		static Ref<VertexBuffer> create(const Memory& mem);
		
		static Ref<VertexBuffer> create(const void* buf, sl_size size);
		
	public:
		sl_size getSize();
		
		sl_uint8* getBuffer();
		
		sl_bool isStatic();
		
		void setStatic(sl_bool flag);
		
		void update(sl_size offset, sl_size size);
		
		void update();
		
		Ref<VertexBufferInstance> getInstance(RenderEngine* engine);
		
	protected:
		Memory m_mem;
		sl_bool m_flagStatic;
		
	};

}

#endif

