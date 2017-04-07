/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/pipe.h"
#include "slib/core/file.h"

#if defined(SLIB_PLATFORM_IS_UNIX)
#include <poll.h>
#endif

namespace slib
{

/********************************************
				Pipe
********************************************/

	SLIB_DEFINE_OBJECT(Pipe, Object)

	Pipe::Pipe()
	{
		m_hRead = SLIB_PIPE_INVALID_HANDLE;
		m_hWrite = SLIB_PIPE_INVALID_HANDLE;
	}

	Pipe::~Pipe()
	{
		close();
	}

	Ref<Pipe> Pipe::create()
	{
		sl_pipe hRead = SLIB_PIPE_INVALID_HANDLE;
		sl_pipe hWrite = SLIB_PIPE_INVALID_HANDLE;
		if (_open(hRead, hWrite)) {
			Ref<Pipe> ret = new Pipe;
			if (ret.isNotNull()) {
				ret->m_hRead = hRead;
				ret->m_hWrite = hWrite;
				return ret;
			}
			_close(hRead);
			_close(hWrite);
		}
		return sl_null;
	}

	sl_bool Pipe::isOpened() const
	{
		return m_hRead != SLIB_PIPE_INVALID_HANDLE;
	}

	sl_pipe Pipe::getReadHandle() const
	{
		return m_hRead;
	}

	sl_pipe Pipe::getWriteHandle() const
	{
		return m_hWrite;
	}

	void Pipe::close()
	{
		if (m_hRead != SLIB_PIPE_INVALID_HANDLE) {
			_close(m_hRead);
			m_hRead = SLIB_PIPE_INVALID_HANDLE;
		}
		if (m_hWrite != SLIB_PIPE_INVALID_HANDLE) {
			_close(m_hWrite);
			m_hWrite = SLIB_PIPE_INVALID_HANDLE;
		}
	}


/********************************************
				PipeEvent
********************************************/

	PipeEvent::PipeEvent()
	{
		m_flagSet = sl_false;
	}

	PipeEvent::~PipeEvent()
	{
	}

	Ref<PipeEvent> PipeEvent::create()
	{
		Ref<Pipe> pipe = Pipe::create();
		if (pipe.isNotNull()) {
			Ref<PipeEvent> ret = new PipeEvent;
			if (ret.isNotNull()) {
				ret->m_pipe = pipe;
#if defined(SLIB_PLATFORM_IS_UNIX)
				File::setNonBlocking((int)(pipe->getReadHandle()), sl_true);
				File::setNonBlocking((int)(pipe->getWriteHandle()), sl_true);
#endif
				return ret;
			}
		}
		return sl_null;
	}

	Ref<Pipe> PipeEvent::getPipe()
	{
		return m_pipe;
	}

	void PipeEvent::_native_set()
	{
		SpinLocker lock(&m_lock);
		if (m_flagSet) {
			return;
		}
		m_flagSet = sl_true;
		char c = 1;
		m_pipe->write(&c, 1);
	}

	void PipeEvent::_native_reset()
	{
		SpinLocker lock(&m_lock);
		if (!m_flagSet) {
			return;
		}
		m_flagSet = sl_false;
		while (1) {
			static char t[200];
			sl_reg n = m_pipe->read(t, 200);
			if (n < 200) {
				break;
			}
		}
	}

	sl_bool PipeEvent::_native_wait(sl_int32 timeout)
	{
#if defined(SLIB_PLATFORM_IS_UNIX)
		pollfd fd;
		Base::zeroMemory(&fd, sizeof(pollfd));
		fd.fd = (int)(getReadPipeHandle());
		fd.events = POLLIN | POLLPRI | POLLERR | POLLHUP;
		int t = timeout >= 0 ? (int)timeout : -1;
		int ret = ::poll(&fd, 1, t);
		if (ret > 0) {
			return sl_true;
		} else {
			return sl_false;
		}
#else
		static char t[200];
		m_pipe->read(t, 200);
		return sl_true;
#endif
	}

	sl_pipe PipeEvent::getReadPipeHandle()
	{
		return m_pipe->getReadHandle();
	}

	sl_pipe PipeEvent::getWritePipeHandle()
	{
		return m_pipe->getWriteHandle();
	}

}
