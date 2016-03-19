#include "../../../inc/slib/render/index_buffer.h"

#include "../../../inc/slib/render/engine.h"

SLIB_RENDER_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(IndexBuffer, RenderBaseObject)

IndexBuffer::IndexBuffer()
{
	setStatic(sl_true);
}

Ref<IndexBuffer> IndexBuffer::create(const Memory& mem)
{
	Ref<IndexBuffer> ret;
	if (mem.isNotEmpty()) {
		ret = new IndexBuffer();
		if (ret.isNotNull()) {
			ret->m_mem = mem;
		}
	}
	return ret;
}

Ref<IndexBuffer> IndexBuffer::create(const void* buf, sl_size size)
{
	return IndexBuffer::create(Memory::create(buf, size));
}

sl_size IndexBuffer::getSize() const
{
	return m_mem.getSize();
}

sl_uint8* IndexBuffer::getBuffer() const
{
	return (sl_uint8*)(m_mem.getData());
}

void IndexBuffer::update(sl_size offset, sl_size size)
{
	Ref<RenderEngine> engine = getEngine();
	if (engine.isNotNull()) {
		engine->_onUpdateIndexBuffer(this, offset, size);
	}
}

void IndexBuffer::update()
{
	update(0, getSize());
}

SLIB_RENDER_NAMESPACE_END
