#include "../../../inc/slib/render/index_buffer.h"
#include "../../../inc/slib/render/engine.h"

SLIB_RENDER_NAMESPACE_BEGIN
IndexBuffer::IndexBuffer()
{
	setStatic(sl_true);
}

Ref<IndexBuffer> IndexBuffer::create(const Memory& _mem)
{
	Ref<IndexBuffer> ret;
	Memory mem = _mem;
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
