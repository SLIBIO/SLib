#include "../../../inc/slib/render/vertex_buffer.h"

#include "../../../inc/slib/render/engine.h"

SLIB_RENDER_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(VertexBuffer, RenderBaseObject)

VertexBuffer::VertexBuffer()
{
	setStatic(sl_true);
}

Ref<VertexBuffer> VertexBuffer::create(const Memory& mem)
{
	Ref<VertexBuffer> ret;
	if (mem.isNotEmpty()) {
		ret = new VertexBuffer();
		if (ret.isNotNull()) {
			ret->m_mem = mem;
		}
	}
	return ret;
}

Ref<VertexBuffer> VertexBuffer::create(const void* buf, sl_size size)
{
	return VertexBuffer::create(Memory::create(buf, size));
}

sl_size VertexBuffer::getSize() const
{
	return m_mem.getSize();
}

sl_uint8* VertexBuffer::getBuffer() const
{
	return (sl_uint8*)(m_mem.getData());
}

void VertexBuffer::update(sl_size offset, sl_size size)
{
	Ref<RenderEngine> engine = getEngine();
	if (engine.isNotNull()) {
		engine->_onUpdateVertexBuffer(this, offset, size);
	}
}

void VertexBuffer::update()
{
	update(0, getSize());
}

SLIB_RENDER_NAMESPACE_END
