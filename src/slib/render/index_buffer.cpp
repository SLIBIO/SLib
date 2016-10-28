#include "../../../inc/slib/render/index_buffer.h"

SLIB_RENDER_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(IndexBufferInstance, RenderBaseObjectInstance)

void IndexBufferInstance::notifyUpdated(sl_size offset, sl_size size)
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

SLIB_DEFINE_OBJECT(IndexBuffer, RenderBaseObject)

IndexBuffer::IndexBuffer()
{
	m_flagStatic = sl_true;
}

Ref<IndexBuffer> IndexBuffer::create(const Memory& mem)
{
	if (mem.isNotEmpty()) {
		Ref<IndexBuffer> ret = new IndexBuffer();
		if (ret.isNotNull()) {
			ret->m_mem = mem;
			return ret;
		}
	}
	return Ref<IndexBuffer>::null();
}

Ref<IndexBuffer> IndexBuffer::create(const void* buf, sl_size size)
{
	return IndexBuffer::create(Memory::create(buf, size));
}

sl_size IndexBuffer::getSize()
{
	return m_mem.getSize();
}

sl_uint8* IndexBuffer::getBuffer()
{
	return (sl_uint8*)(m_mem.getData());
}

sl_bool IndexBuffer::isStatic()
{
	return m_flagStatic;
}

void IndexBuffer::setStatic(sl_bool flag)
{
	m_flagStatic = flag;
}

void IndexBuffer::update(sl_size offset, sl_size size)
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
			((IndexBufferInstance*)(instance.ptr))->notifyUpdated(offset, size);
		}
	}
}

void IndexBuffer::update()
{
	update(0, getSize());
}

Ref<IndexBufferInstance> IndexBuffer::getInstance(RenderEngine* engine)
{
	return Ref<IndexBufferInstance>::from(RenderBaseObject::getInstance(engine));
}

SLIB_RENDER_NAMESPACE_END
