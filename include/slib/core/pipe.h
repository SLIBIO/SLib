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

#ifndef CHECKHEADER_SLIB_CORE_PIPE
#define CHECKHEADER_SLIB_CORE_PIPE

#include "definition.h"

#include "io.h"
#include "event.h"

typedef sl_reg sl_pipe;
#define SLIB_PIPE_INVALID_HANDLE (sl_pipe)(-1)

namespace slib
{
	
	class SLIB_EXPORT Pipe : public Object, public IStream
	{
		SLIB_DECLARE_OBJECT

	private:
		Pipe();

		~Pipe();

	public:
		static Ref<Pipe> create();

		sl_bool isOpened() const;

		sl_pipe getReadHandle() const;

		sl_pipe getWriteHandle() const;

		sl_int32 read32(void* buf, sl_uint32 size) override;

		sl_int32 write32(const void* buf, sl_uint32 size) override;

		void close();

	protected:
		static sl_bool _open(sl_pipe& hRead, sl_pipe& hWrite);

		static void _close(sl_pipe handle);

	private:
		sl_pipe m_hRead;
		sl_pipe m_hWrite;

	};
	
	class SLIB_EXPORT PipeEvent : public Event
	{
	protected:
		PipeEvent();

		~PipeEvent();

	public:
		static Ref<PipeEvent> create();

		Ref<Pipe> getPipe();

		sl_pipe getReadPipeHandle();

		sl_pipe getWritePipeHandle();

	protected:
		void _native_set() override;

		void _native_reset() override;

		sl_bool _native_wait(sl_int32 timeout) override;

	protected:
		Ref<Pipe> m_pipe;
		sl_bool m_flagSet;
		SpinLock m_lock;

	};

}

#endif
