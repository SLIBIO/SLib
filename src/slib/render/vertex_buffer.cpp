#include "../../../inc/slib/render/vertex_buffer.h"
#include "../../../inc/slib/render/engine.h"

SLIB_RENDER_NAMESPACE_BEGIN

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
