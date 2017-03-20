/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/render/vertex_buffer.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(VertexBufferInstance, RenderBaseObjectInstance)
	
	VertexBufferInstance::VertexBufferInstance()
	{
	}
	
	VertexBufferInstance::~VertexBufferInstance()
	{
	}
	
	void VertexBufferInstance::notifyUpdated(sl_size offset, sl_size size)
	{
		ObjectLocker lock(this);
		if (m_flagUpdated) {
			sl_size oldLeft = m_updatedOffset;
			sl_size oldRight = oldLeft + m_updatedSize;
			sl_size newRight = offset + size;
			if (oldLeft < offset) {
				offset = oldLeft;
			}
			if (oldRight > newRight) {
				newRight = oldRight;
			}
			m_updatedOffset = offset;
			m_updatedSize = newRight - offset;
		} else {
			m_updatedOffset = offset;
			m_updatedSize = size;
			m_flagUpdated = sl_true;
		}
	}
	
	
	SLIB_DEFINE_OBJECT(VertexBuffer, RenderBaseObject)
	
	VertexBuffer::VertexBuffer()
	{
		m_flagStatic = sl_true;
	}
	
	VertexBuffer::~VertexBuffer()
	{
	}
	
	Ref<VertexBuffer> VertexBuffer::create(const Memory& mem)
	{
		if (mem.isNotEmpty()) {
			Ref<VertexBuffer> ret = new VertexBuffer();
			if (ret.isNotNull()) {
				ret->m_mem = mem;
				return ret;
			}
		}
		return sl_null;
	}
	
	Ref<VertexBuffer> VertexBuffer::create(const void* buf, sl_size size)
	{
		return VertexBuffer::create(Memory::create(buf, size));
	}
	
	sl_size VertexBuffer::getSize()
	{
		return m_mem.getSize();
	}
	
	sl_uint8* VertexBuffer::getBuffer()
	{
		return (sl_uint8*)(m_mem.getData());
	}
	
	sl_bool VertexBuffer::isStatic()
	{
		return m_flagStatic;
	}
	
	void VertexBuffer::setStatic(sl_bool flag)
	{
		m_flagStatic = flag;
	}
	
	void VertexBuffer::update(sl_size offset, sl_size size)
	{
		sl_size total = m_mem.getSize();
		if (offset >= total) {
			return;
		}
		if (size > total - offset) {
			size = total - offset;
		}
		for (int i = 0; i < SLIB_MAX_RENDER_ENGINE_COUNT_PER_OBJECT; i++) {
			Ref<RenderBaseObjectInstance> instance = m_instances[i];
			if (instance.isNotNull()) {
				((VertexBufferInstance*)(instance.get()))->notifyUpdated(offset, size);
			}
		}
	}
	
	void VertexBuffer::update()
	{
		update(0, getSize());
	}
	
	Ref<VertexBufferInstance> VertexBuffer::getInstance(RenderEngine* engine)
	{
		return Ref<VertexBufferInstance>::from(RenderBaseObject::getInstance(engine));
	}
	
}
