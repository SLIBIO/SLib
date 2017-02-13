#ifndef CHECKHEADER_SLIB_RENDER_INDEX_BUFFER
#define CHECKHEADER_SLIB_RENDER_INDEX_BUFFER

#include "definition.h"

#include "base.h"

#include "../core/memory.h"

namespace slib
{
	class SLIB_EXPORT IndexBufferInstance : public RenderBaseObjectInstance
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		IndexBufferInstance();
		
		~IndexBufferInstance();
		
	public:
		virtual void notifyUpdated(sl_size offset, sl_size size);
		
	protected:
		sl_size m_updatedOffset;
		sl_size m_updatedSize;
		
	};
	
	class SLIB_EXPORT IndexBuffer : public RenderBaseObject
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		IndexBuffer();
		
		~IndexBuffer();
		
	public:
		static Ref<IndexBuffer> create(const Memory& mem);
		
		static Ref<IndexBuffer> create(const void* buf, sl_size size);
		
	public:
		sl_size getSize();
		
		sl_uint8* getBuffer();
		
		sl_bool isStatic();
		
		void setStatic(sl_bool flag);
		
		void update(sl_size offset, sl_size size);
		
		void update();
		
		Ref<IndexBufferInstance> getInstance(RenderEngine* engine);
		
	protected:
		Memory m_mem;
		sl_bool m_flagStatic;
		
	};	
}

#endif
