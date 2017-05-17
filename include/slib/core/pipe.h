/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
