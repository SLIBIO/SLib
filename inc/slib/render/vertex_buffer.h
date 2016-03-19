#ifndef CHECKHEADER_SLIB_RENDER_VERTEX_BUFFER
#define CHECKHEADER_SLIB_RENDER_VERTEX_BUFFER

#include "definition.h"

#include "base.h"
#include "../core/memory.h"

SLIB_RENDER_NAMESPACE_BEGIN

class SLIB_EXPORT VertexBuffer : public RenderBaseObject
{
	SLIB_DECLARE_OBJECT
	
protected:
	VertexBuffer();
	
public:
	static Ref<VertexBuffer> create(const Memory& mem);
	static Ref<VertexBuffer> create(const void* buf, sl_size size);
	
public:
	sl_size getSize() const;
	
	sl_uint8* getBuffer() const;

	void update(sl_size offset, sl_size size);
	
	void update();
	
public:
	SLIB_PROPERTY(sl_bool, Static)

protected:
	Memory m_mem;
	
};

SLIB_RENDER_NAMESPACE_END

#endif

