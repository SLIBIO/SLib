#ifndef CHECKHEADER_SLIB_RENDER_INDEX_BUFFER
#define CHECKHEADER_SLIB_RENDER_INDEX_BUFFER

#include "definition.h"

#include "base.h"

#include "../core/memory.h"

SLIB_RENDER_NAMESPACE_BEGIN

class SLIB_EXPORT IndexBuffer : public RenderBaseObject
{
	SLIB_DECLARE_OBJECT(IndexBuffer, RenderBaseObject)
protected:
	IndexBuffer();

public:
	static Ref<IndexBuffer> create(const Memory& mem);
	
	static Ref<IndexBuffer> create(const void* buf, sl_size size);

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
